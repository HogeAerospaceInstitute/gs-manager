/* -----------------------------------------------------------------------------
 * Copyright (C) 2022, Hoge Aerospace Institute
 * This software is the confidential and proprietary information of the
 * Hoge Aerospace Institute.
 *
 * ALL RIGHTS RESERVED
 *
 * Permission is hereby granted to licensees of Hoge Aerospace Institute
 * products to use or abstract this computer program for the sole purpose of
 * implementing a product based on Hoge Aerospace Institute products.  No
 * other rights to reproduce, use, or disseminate this computer program,
 * whether in part or in whole, are granted.
 *
 * Hoge Aerospace Institute makes no representation or warranties with respect
 * to the performance of this computer program, and specifically disclaims any
 * responsibility for any damages, special or consequential, connected with
 * the use of this program.
 * -----------------------------------------------------------------------------
 */

#include <imgui.h>
#include <module.h>
#include <gui/gui.h>
#include <gui/style.h>
#include <config.h>
#include <curl/curl.h>
#include <thread>
#include <filesystem>

#include <signal_path/signal_path.h>
#include <signal_path/source.h>

#include "gsm_globals.h"
#include "gsm_http_client.h"
#include "gsm_task.h"
#include "gsm_msg.h"
#include "gsm_rotator_controller.h"
#include "gsm_comm_mgr.h"
#include "ground_station_mgr.h"
#include "gsm_timer_mgr.h"


#define CONCAT(a, b) ((std::string(a) + b).c_str())


//extern ConfigManager gConfig;

SDRPP_MOD_INFO {
	/* Name:            */ "ground_station_mgr",
	/* Description:     */ "Ground Station Manager",
	/* Author:          */ "jrenkema",
	/* Version:         */ 0, 1, 0,
	/* Max instances    */ -1
};



class GsManagerModule : public ModuleManager::Instance
{
public:
	GsManagerModule(std::string name) {
		this->name = name;
		gui::menu.registerEntry(name, menuHandler, this, NULL);
	}

    virtual ~GsManagerModule() {
        gui::menu.removeEntry(name);
    }

    void postInit() {
        spdlog::info("GsManagerModule::postInit");

        // Initialize messaging service
        GsmCommMgr::getInstance()->init();

        // Start timer service
        GsmTimerMgr::getInstance()->start();

        // Start application services
        GsmRotatorController::getInstance()->start();
        GroundStationMgr::getInstance()->start();
        GsmHttpClient::getInstance()->start();

        // Register services with inter-thread communication manager
        std::string rot = "ROTCTRL";
        GsmCommMgr::getInstance()->registerConsumer(rot,
        		(GsmCommConsumer*)GsmRotatorController::getInstance());
        std::string gsm = "GSMGR";
        GsmCommMgr::getInstance()->registerConsumer(gsm,
        		(GsmCommConsumer*)GroundStationMgr::getInstance());
        std::string http = "HTTPCLIENT";
        GsmCommMgr::getInstance()->registerConsumer(http,
        		(GsmCommConsumer*)GsmHttpClient::getInstance());

        // send start message to manager
		GsmMsg* pMsg = new GsmMsg();
		pMsg->setDestination("GSMGR");
		pMsg->setType(GSM_MSG_TYPE_START);
		pMsg->setCategory(GsmMsg::GMS_MSG_CAT_COMM);

		GsmCommMgr::getInstance()->sendMsg(pMsg);
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


private:

    void onButtonPushRefreshTasks()
    {
		spdlog::info("GsManagerModule::onButtonPushRefreshTasks: entered");

		GsmMsg* pMsg = new GsmMsg();
		pMsg->setDestination("GSMGR");
		pMsg->setType(GSM_MSG_TYPE_REFRESH_TASKS_REQ);
		pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

		GsmCommMgr::getInstance()->sendMsg(pMsg);
    }


    void onButtonPushClearTasks()
    {
		spdlog::info("GsManagerModule::onButtonPushClearTasks: entered");

		GsmMsg* pMsg = new GsmMsg();
		pMsg->setDestination("GSMGR");
		pMsg->setType(GSM_MSG_TYPE_CLEAR_TASKS_REQ);
		pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

    	GsmCommMgr::getInstance()->sendMsg(pMsg);
    }


	void onButtonPushActivateTask(GsmTask* _task)
	{
		spdlog::info("GsManagerModule::onButtonPushActivateTask: entered");

		GsmMsgActivateTaskReq* pMsg = new GsmMsgActivateTaskReq();
		pMsg->setDestination("GSMGR");
		pMsg->setType(GSM_MSG_TYPE_ACTIVATE_TASK_REQ);
		pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

		std::string taskId;
		_task->getUuid(taskId);
		pMsg->setTaskId(taskId);

    	GsmCommMgr::getInstance()->sendMsg((GsmMsg*)pMsg);

	}

	void onButtonPushDeactivateTask(GsmTask* _task)
	{
		spdlog::info("GsManagerModule::onButtonPushDeactivateTask: entered");

		GsmMsgDeactivateTaskReq* pMsg = new GsmMsgDeactivateTaskReq();
		pMsg->setDestination("GSMGR");
		pMsg->setType(GSM_MSG_TYPE_DEACTIVATE_TASK_REQ);
		pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

		std::string taskId;
		_task->getUuid(taskId);
		pMsg->setTaskId(taskId);

    	GsmCommMgr::getInstance()->sendMsg((GsmMsg*)pMsg);

	}

	void onButtonPushTrackSatellite(GsmSatellite* _satellite)
	{
		spdlog::info("GsManagerModule::onButtonPushTrackSatellite: entered");

		GsmMsgTrackSatelliteReq* pMsg = new GsmMsgTrackSatelliteReq();
		pMsg->setDestination("GSMGR");
		pMsg->setType(GSM_MSG_TYPE_TRACK_SATELLITE_REQ);
		pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

		std::string satelliteName;
		_satellite->getName(satelliteName);
		pMsg->setSatelliteName(satelliteName);

    	GsmCommMgr::getInstance()->sendMsg((GsmMsg*)pMsg);
	}


    void onButtonPushGetSatellitePos(GsmSatellite* _satellite)
    {
		spdlog::info("GsManagerModule::onButtonPushGetSatellitePos: entered");

		GsmMsgGetSatellitePosReq* pMsg = new GsmMsgGetSatellitePosReq();
		pMsg->setDestination("GSMGR");
		pMsg->setType(GSM_MSG_TYPE_GET_SATELLITE_POS_REQ);
		pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

		std::string satelliteName;
		_satellite->getName(satelliteName);
		pMsg->setSatellite(satelliteName);

    	GsmCommMgr::getInstance()->sendMsg(pMsg);
    }


    void onButtonPushMoveRotator(GsmSatellite* _satellite)
    {
		spdlog::info("GsManagerModule::onButtonPushGetSatellitePos: entered");

		GsmMsgMoveRotatorReq* pMsg = new GsmMsgMoveRotatorReq();
		pMsg->setDestination("GSMGR");
		pMsg->setType(GSM_MSG_TYPE_MOVE_ROTATOR_REQ);
		pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

		std::string satelliteName;
		_satellite->getName(satelliteName);
		pMsg->setSatellite(satelliteName);

    	GsmCommMgr::getInstance()->sendMsg(pMsg);
    }

    void onButtonPushUpdateGroundStationInfo()
    {
		spdlog::info("GsManagerModule::onButtonPushUpdateGroundStationInfo: entered");

		GsmMsg* pMsg = new GsmMsg();
		pMsg->setDestination("GSMGR");
		pMsg->setType(GSM_MSG_TYPE_GET_GROUND_STATION_INFO_REQ);
		pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

    	GsmCommMgr::getInstance()->sendMsg(pMsg);
    }

    void onButtonPushUploadRecordings()
    {
		spdlog::info("GsManagerModule::onButtonPushUploadRecordings: entered");

		GsmMsg* pMsg = new GsmMsg();
		pMsg->setDestination("GSMGR");
		pMsg->setType(GSM_MSG_TYPE_UPLOAD_RECORDINGS_REQ);
		pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

    	GsmCommMgr::getInstance()->sendMsg(pMsg);
    }


    static void menuHandler(void* ctx)
    {
    	GsManagerModule* _this = (GsManagerModule*)ctx;
        auto windowWidth = ImGui::GetWindowSize().x;
        // float menuColumnWidth = ImGui::GetContentRegionAvailWidth();
#define GSM_MAX_INPUT_BUFFER_SIZE	1024
        char inputBuf[GSM_MAX_INPUT_BUFFER_SIZE+1];

        // Configuration Section
        auto textWidth   = ImGui::CalcTextSize("Configuration").x;
        ImGui::Separator();
        ImGui::Separator();
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("Configuration");
        ImGui::Separator();

        // Configure ground-station-id
        memset(inputBuf, 0, GSM_MAX_INPUT_BUFFER_SIZE+1);
        if (gConfig.conf["ground-station-id"].is_null() == true) {
            spdlog::info("GsManagerModule::menuHandler: set ground-station to default");
            gConfig.acquire();
            gConfig.conf["ground-station-id"] = "ae78f216-d97a-4612-8f06-ab14d2dbc22a";
            gConfig.release(true);
        }
        std::string groundStationId = gConfig.conf["ground-station-id"];
        std::strncpy(inputBuf, groundStationId.c_str(), GSM_MAX_INPUT_BUFFER_SIZE);
        inputBuf[GSM_MAX_INPUT_BUFFER_SIZE] = '\0';
        ImGui::LeftLabel("Ground Station Id");
        if (ImGui::InputText("##Ground Station Id", inputBuf, GSM_MAX_INPUT_BUFFER_SIZE)) {
            spdlog::info("GsManagerModule::menuHandler: set ground-station-id={0}",
            			  inputBuf);
            gConfig.acquire();
            gConfig.conf["ground-station-id"] = inputBuf;
            gConfig.release(true);
        }

        // Configure web-server
        memset(inputBuf, 0, GSM_MAX_INPUT_BUFFER_SIZE+1);
        if (!gConfig.conf.contains("web-server")) {
            spdlog::info("GsManagerModule::menuHandler: setting web-server to default");
            gConfig.acquire();
            gConfig.conf["web-server"] = "bowshock.onrender.com";
            gConfig.release(true);
        }
        std::string webServer = gConfig.conf["web-server"];
        std::strncpy(inputBuf, webServer.c_str(), GSM_MAX_INPUT_BUFFER_SIZE);
        inputBuf[GSM_MAX_INPUT_BUFFER_SIZE] = '\0';
        ImGui::LeftLabel("Web Server");
        if (ImGui::InputText("##Web Server", inputBuf, GSM_MAX_INPUT_BUFFER_SIZE)) {
            spdlog::info("GsManagerModule::menuHandler: set web-server={0}",
            			 inputBuf);
            gConfig.acquire();
            gConfig.conf["web-server"] = inputBuf;
            gConfig.release(true);
        }

        // Configure auto-refresh-tasks
        ImGui::LeftLabel("Auto-Refresh Tasks");
        if (!gConfig.conf.contains("auto-refresh-tasks")) {
            spdlog::info("GsManagerModule::menuHandler: setting auto-refresh-tasks to default");
            gConfig.acquire();
            gConfig.conf["auto-upload-recordings"] = true;
            gConfig.release(true);
        }

        bool autoRefreshEnabled = gConfig.conf["auto-refresh-tasks"];
        if (ImGui::Checkbox("##Auto-Refresh Tasks", &autoRefreshEnabled)) {
            spdlog::info("GsManagerModule::menuHandler: set auto-refresh={0}",
            			 autoRefreshEnabled);
            gConfig.acquire();
            gConfig.conf["auto-refresh-tasks"] = autoRefreshEnabled;
            gConfig.release(true);
        }

        // Configure auto-upload-recordings
        ImGui::LeftLabel("Auto-Upload Recordings");
        if (!gConfig.conf.contains("auto-upload-recordings")) {
            spdlog::info("GsManagerModule::menuHandler: setting auto-upload-records to default");
            gConfig.acquire();
            gConfig.conf["auto-upload-recordings"] = false;
            gConfig.release(true);
        }

        bool autoUploadRecordingsEnabled = gConfig.conf["auto-upload-recordings"];
        if (ImGui::Checkbox("##Auto-Upload Recordings", &autoUploadRecordingsEnabled)) {
            spdlog::info("GsManagerModule::menuHandler: set auto-upload-recordings={0}",
            		autoUploadRecordingsEnabled);
            gConfig.acquire();
            gConfig.conf["auto-upload-recordings"] = autoUploadRecordingsEnabled;
            gConfig.release(true);
        }
        ImGui::Separator();

        // Ground Station Details Section
        textWidth   = ImGui::CalcTextSize("Ground Station Info and Location").x;
        ImGui::Separator();
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("Ground Station Info and Location");
        ImGui::Separator();

        // Latitude and Longitude
        ImGui::LeftLabel("Name: ");
        std::string gsName;
        GroundStationMgr::getInstance()->getGroundStationName(gsName);
        ImGui::Text(gsName.c_str());

        // Latitude and Longitude
        ImGui::LeftLabel("Latitude: ");
        std::string latitude;
        GroundStationMgr::getInstance()->getLatitude(latitude);
        ImGui::Text(latitude.c_str());

        ImGui::LeftLabel("Longitude: ");
        std::string longitude;
        GroundStationMgr::getInstance()->getLongitude(longitude);
        ImGui::Text(longitude.c_str());

        ImGui::Separator();
        if (ImGui::Button("Update")) {
        	_this->onButtonPushUpdateGroundStationInfo();
        }
        ImGui::Separator();

        // Tasks Section
        textWidth   = ImGui::CalcTextSize("Tasks").x;
        ImGui::Separator();
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("Tasks");
        ImGui::Separator();

        std::map<string, GsmTask*> tasks;
        GroundStationMgr::getInstance()->getTasks(tasks);

        if (!tasks.empty())
        {
        	std::map<string,GsmTask*>::iterator it;
        	for (it = tasks.begin(); it != tasks.end(); ++it)
        	{
        		GsmTask* pTask = it->second;
        		std::string taskStr;
        		std::string uuid;

        		pTask->getUuid(uuid);
        		pTask->print(taskStr);
        		ImGui::TextWrapped(taskStr.c_str());

                std::string taskStatus;
                pTask->getStatus(taskStatus);
                if (taskStatus == "INACTIVE")
                {
                	if (ImGui::Button(CONCAT("Activate##_gsm_task_", uuid)))
                	{
                		_this->onButtonPushActivateTask(pTask);
                	}
                }
                else
                {
                	if (ImGui::Button(CONCAT("Deactivate##_gsm_task_", uuid)))
                	{
                		_this->onButtonPushDeactivateTask(pTask);
                	}
                }
                ImGui::SameLine();

                std::string status = "   State=" + taskStatus;
                ImGui::Text(status.c_str());

                std::string alertMsg;
                pTask->getAlertMsg(alertMsg);
                if (!alertMsg.empty())
                {
                    ImGui::SameLine();

                	std::string alert = "   Alert=" + alertMsg;
                	ImGui::Text(alert.c_str());
//                	ImGui::Separator();
                }

                ImGui::Separator();
        	}
        }
        else
        {
        	ImGui::TextWrapped("No tasks are defined.");
        }

        ImGui::Spacing();

        if (ImGui::Button("Refresh Tasks")) {
        	// confirm that web-server is configured
        	if (gConfig.conf["web-server"].is_null() == true) {
        		ImGui::Text("Web server not configured!!");
        	} else {
    			_this->onButtonPushRefreshTasks();
        	}
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear Tasks")) {
        	_this->onButtonPushClearTasks();
        }
        ImGui::Separator();

        // Satellites Section
        textWidth   = ImGui::CalcTextSize("Satellites").x;
        ImGui::Separator();
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("Satellites");
        ImGui::Separator();

        std::map<string, GsmSatellite*> satellites;
        GroundStationMgr::getInstance()->getSatellites(satellites);

        if (!satellites.empty())
        {
        	std::map<string,GsmSatellite*>::iterator it;
        	for (it = satellites.begin(); it != satellites.end(); ++it)
        	{
        		GsmSatellite* pSatellite = it->second;
        		std::string satelliteName;
        		std::string satelliteStr;

        		pSatellite->getName(satelliteName);
        		pSatellite->print(satelliteStr);
        		ImGui::TextWrapped(satelliteStr.c_str());

                if (ImGui::Button(CONCAT("Track##_gsm_sat_name_", satelliteName)))
                {
                    spdlog::info("GsManagerModule::menuHandler: track");
                	_this->onButtonPushTrackSatellite(pSatellite);
                }
                ImGui::SameLine();

                if (ImGui::Button(CONCAT("Get Position##_gsm_sat_name_", satelliteName)))
                {
                    spdlog::info("GsManagerModule::menuHandler: position");
                	_this->onButtonPushGetSatellitePos(pSatellite);
                }

                ImGui::SameLine();
                std::string az = std::to_string(pSatellite->getAzimuth());
                std::string el = std::to_string(pSatellite->getElevation());
                std::string position = "   Azimuth=" + az + ", Elevation=" + el;
                ImGui::Text(position.c_str());

                ImGui::SameLine();

                if (ImGui::Button(CONCAT("Move Rotator##_gsm_sat_name_", satelliteName)))
                {
                    spdlog::info("GsManagerModule::menuHandler: move rotator");
                	_this->onButtonPushMoveRotator(pSatellite);
                }

        		ImGui::Separator();
        	}
        }
        else
        {
        	ImGui::TextWrapped("No satellites are available.");
            ImGui::Separator();
        }

        // Recordings Section
        textWidth   = ImGui::CalcTextSize("Recordings").x;
        ImGui::Separator();
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("Recordings");
        ImGui::Separator();

        const std::filesystem::path recordings{GSM_RECORDING_DATA_DIR};
        int numFiles = 0;
        for (auto const& dir_entry : std::filesystem::directory_iterator{recordings})
        {
    		if (dir_entry.is_directory() == true)
    		{
    			continue;
    		}
    		ImGui::Text(dir_entry.path().c_str());
    		numFiles++;
        }

        if (numFiles == 0)
        {
        	ImGui::Text("No recordings are available.");
        }
        else
        {
            ImGui::Separator();
            if (ImGui::Button("Upload Recordings")) {
            	_this->onButtonPushUploadRecordings();
            }
            ImGui::Separator();
        }
    }

    std::string name;
    bool enabled = true;

};

MOD_EXPORT void _INIT_() {
	// Set default values
    json def = json({});

    def["ground-station-id"] = "ae78f216-d97a-4612-8f06-ab14d2dbc22a";
    def["web-server"] = "bowshock.onrender.com";
    def["auto-refresh-tasks"] = true;
    def["auto-upload-recordings"] = false;

    gConfig.setPath("/var/opt/hai/gsmgr/gsm_config.json");
    gConfig.load(def);
    gConfig.enableAutoSave();
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
