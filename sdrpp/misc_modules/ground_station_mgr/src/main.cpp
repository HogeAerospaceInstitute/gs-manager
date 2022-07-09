#include <imgui.h>
#include <module.h>
#include <gui/gui.h>
#include <gui/style.h>
#include <config.h>
#include <curl.h>
#include <thread>

#include "http_client.h"
#include "gsm_task.h"


#define CONCAT(a, b) ((std::string(a) + b).c_str())

static bool clientRunning = false;
static ConfigManager config;

// TODO: support multiple HTTP transactions
static GsmHttpTransaction_t httpTxn;


SDRPP_MOD_INFO {
	/* Name:            */ "gs_mgr",
	/* Description:     */ "Ground Station",
	/* Author:          */ "jrenkema",
	/* Version:         */ 0, 1, 0,
	/* Max instances    */ -1
};



class GsManagerModule : public ModuleManager::Instance {
public:
	GsManagerModule(std::string name) {
		this->name = name;
		gui::menu.registerEntry(name, menuHandler, this, NULL);
		memset(&httpTxn, 0, sizeof(GsmHttpTransaction_t));
	}

    virtual ~GsManagerModule() {
        gui::menu.removeEntry(name);
    }

    void postInit() {
        spdlog::info("GsManagerModule::postInit");
    }

    void enable() {
        spdlog::info("GsManagerModule::enable");
        enabled = true;
    }

    void disable() {
        spdlog::info("GsManagerModule::disable");
        enabled = false;
    }

    bool isEnabled() {
        return enabled;
    }

    static void httpClientThreadEntry() {

        spdlog::info("GsManagerModule::httpClientThreadEntry: entering...");

    	// instantiate new client object
    	GsmHttpClient* pClient = new GsmHttpClient();

        // "https://bowshock.onrender.com/api/ground_stations/ae78f216-d97a-4612-8f06-ab14d2dbc22a/tasks");

    	// Fill in HTTP transaction message
    	httpTxn.req.method = "GET";
    	std::string webServer = config.conf["web-server"];
    	std::string groundStationId = config.conf["ground-station-id"];
    	httpTxn.req.url = "https://" + webServer + "/api/ground_stations/" + groundStationId + "/tasks";

    	pClient->init(httpTxn);
    	pClient->send();
    	clientRunning = false;

    	spdlog::info("GsManagerModule::httpClientThreadEntry: exiting...");
    	return;
    }


private:

    void handleRefreshTasksReq() {
		spdlog::info("GsManagerModule::handleRefreshTasksReq: entered");

		if (clientRunning == false) {
			std::string webServer = config.conf["web-server"];
			spdlog::info("GsManagerModule::menuHandler: querying server={0}", webServer.c_str());
			clientRunning = true;
			static std::thread mHttpClientThread = std::thread(&GsManagerModule::httpClientThreadEntry);
		}
		else {
			spdlog::info("GsManagerModule::menuHandler: client thread already running");
		}
    }

    void handleHttpRsp() {

    	if (httpTxn.status == GSM_HTTP_TRANSACTION_STATE_COMPLETE) {
    		spdlog::info("GsManagerModule::handleHttpRsp: recv rsp");

    		// TODO: clear existing list of tasks

    		if (httpTxn.rsp.data[0] != '\0') {
    			string tasksStr = httpTxn.rsp.data;

    			// parse into json object
    			json tasks = json::parse(tasksStr);

    			// iterate the array
    			for (json::iterator it = tasks.begin(); it != tasks.end(); ++it) {

    				// allocate GsmTask
    				GsmTask* pTask = new GsmTask;

    				// fill in parameters
    				pTask->init(*it);
    				json task = *it;
    				string uuidStr = task["UUID"];
    				mTasks.insert(std::pair<std::string, GsmTask*>(uuidStr, pTask));
    			}
    		}
    		else {
        		spdlog::error("GsManagerModule::handleHttpRsp: no data in rsp!");
    		}

    		// reset httpTxn
    		memset(&httpTxn, 0, sizeof(GsmHttpTransaction_t));
    		httpTxn.status = GSM_HTTP_TRANSACTION_STATE_NULL;
    	}

    }

    static void menuHandler(void* ctx) {
    	GsManagerModule* _this = (GsManagerModule*)ctx;
        auto windowWidth = ImGui::GetWindowSize().x;
        float menuColumnWidth = ImGui::GetContentRegionAvailWidth();
        char inputBuf[256];

        // Auto-center configuration section
        auto textWidth   = ImGui::CalcTextSize("Configuration").x;
        ImGui::Separator();
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("Configuration");
        ImGui::Separator();

        // Configure ground-station-id
        memset(inputBuf, 0, 256);
        if (config.conf["ground-station-id"].is_null() == true) {
            spdlog::info("GsManagerModule::menuHandler: setting ground-station to default");
            config.acquire();
        	config.conf["ground-station-id"] = "ae78f216-d97a-4612-8f06-ab14d2dbc22a";
        	config.release(true);
        }
        std::string groundStationId = config.conf["ground-station-id"];
        std::strncpy(inputBuf, groundStationId.c_str(), 256);
        inputBuf[256] = '\0';
        ImGui::LeftLabel("Ground Station Id");
        if (ImGui::InputText("##Ground Station Id", inputBuf, 256)) {
            spdlog::info("GsManagerModule::menuHandler: set ground-station-id={0}", inputBuf);
            config.acquire();
            config.conf["ground-station-id"] = inputBuf;
            config.release(true);
        }

        // Configure web-server
        memset(inputBuf, 0, 256);
        if (!config.conf.contains("web-server")) {
            spdlog::info("GsManagerModule::menuHandler: setting web-server to default");
            config.acquire();
        	config.conf["web-server"] = "bowshock.onrender.com";
        	config.release(true);
        }
        std::string webServer = config.conf["web-server"];
        std::strncpy(inputBuf, webServer.c_str(), 256);
        inputBuf[256] = '\0';
        ImGui::LeftLabel("Web Server");
        if (ImGui::InputText("##Web Server", inputBuf, 256)) {
            spdlog::info("GsManagerModule::menuHandler: set web-server={0}", inputBuf);
            config.acquire();
            config.conf["web-server"] = inputBuf;
            config.release(true);
        }

        // Configure auto-refresh
        memset(inputBuf, 0, 256);
        ImGui::LeftLabel("Auto-Refresh Tasks");
        bool autoRefreshEnabled = true;
        if (ImGui::Checkbox("##Auto-Refresh Tasks", &autoRefreshEnabled)) {
            spdlog::info("GsManagerModule::menuHandler: set auto-refresh={0}", autoRefreshEnabled);
            config.acquire();
            config.conf["auto-refresh-tasks"] = autoRefreshEnabled;
            config.release(true);
        }

        textWidth   = ImGui::CalcTextSize("Tasks").x;
        ImGui::Separator();
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("Tasks");
        ImGui::Separator();

        // TODO: replace with GsmTask
        if (httpTxn.rsp.data[0] != '\0') {
        	ImGui::Text("Task List");
        	string tasks = httpTxn.rsp.data;
        	ImGui::TextWrapped(tasks.c_str());
        }
        else {
        	ImGui::TextWrapped("No tasks are defined.");
        }

        ImGui::Spacing();
//        if (ImGui::Button("Refresh Tasks", ImVec2(menuColumnWidth, 0))) {

        if (ImGui::Button("Refresh Tasks")) {
        	// confirm that web-server is configured
        	if (config.conf["web-server"].is_null() == true) {
        		ImGui::Text("Web server not configure!!");
        	} else {
    			_this->handleRefreshTasksReq();
        	}
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear Tasks")) {
        	memset(httpTxn.rsp.data, 0, GSM_MAX_HTTP_DATA_SIZE);
            spdlog::info("GsManagerModule::menuHandler: cleared tasks");
        }

        // Check if HTTP response has been received
        // TODO: need better mechanism
        _this->handleHttpRsp();
    }

    std::string name;
    bool enabled = true;

    std::map<string, GsmTask*> mTasks;

};

MOD_EXPORT void _INIT_() {
    // Nothing here
}

MOD_EXPORT ModuleManager::Instance* _CREATE_INSTANCE_(std::string name) {
    return new GsManagerModule(name);
}

MOD_EXPORT void _DELETE_INSTANCE_(void* instance) {
    delete (GsManagerModule*)instance;
}

MOD_EXPORT void _END_() {
    // Nothing here
}
