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
#include <curl.h>
#include <thread>

#include <signal_path/signal_path.h>
#include <signal_path/source.h>

#include "gsm_globals.h"
#include "gsm_http_client.h"
#include "gsm_task.h"
#include "gsm_msg.h"
#include "gsm_rotator_controller.h"
#include "gsm_comm_mgr.h"
#include "ground_station_mgr.h"


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

        GsmCommMgr::getInstance()->init();

        // Create and start services
        GsmRotatorController::getInstance()->start();
        GroundStationMgr::getInstance()->start();
        GsmHttpClient::getInstance()->start();
        //GsmTimerMgr::getInstance()->start();

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
		pMsg->setSatelliteName(satelliteName);

    	GsmCommMgr::getInstance()->sendMsg(pMsg);
    }


    static void menuHandler(void* ctx)
    {
    	GsManagerModule* _this = (GsManagerModule*)ctx;
        auto windowWidth = ImGui::GetWindowSize().x;
        float menuColumnWidth = ImGui::GetContentRegionAvailWidth();
        char inputBuf[256];

        // Configuration Section
        auto textWidth   = ImGui::CalcTextSize("Configuration").x;
        ImGui::Separator();
        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text("Configuration");
        ImGui::Separator();

        // Configure ground-station-id
        memset(inputBuf, 0, 256);
        if (gConfig.conf["ground-station-id"].is_null() == true) {
            spdlog::info("GsManagerModule::menuHandler: set ground-station to default");
            gConfig.acquire();
            gConfig.conf["ground-station-id"] = "ae78f216-d97a-4612-8f06-ab14d2dbc22a";
            gConfig.release(true);
        }
        std::string groundStationId = gConfig.conf["ground-station-id"];
        std::strncpy(inputBuf, groundStationId.c_str(), 256);
        inputBuf[256] = '\0';
        ImGui::LeftLabel("Ground Station Id");
        if (ImGui::InputText("##Ground Station Id", inputBuf, 256)) {
            spdlog::info("GsManagerModule::menuHandler: set ground-station-id={0}",
            			  inputBuf);
            gConfig.acquire();
            gConfig.conf["ground-station-id"] = inputBuf;
            gConfig.release(true);
        }

        // Configure web-server
        memset(inputBuf, 0, 256);
        if (!gConfig.conf.contains("web-server")) {
            spdlog::info("GsManagerModule::menuHandler: setting web-server to default");
            gConfig.acquire();
            gConfig.conf["web-server"] = "bowshock.onrender.com";
            gConfig.release(true);
        }
        std::string webServer = gConfig.conf["web-server"];
        std::strncpy(inputBuf, webServer.c_str(), 256);
        inputBuf[256] = '\0';
        ImGui::LeftLabel("Web Server");
        if (ImGui::InputText("##Web Server", inputBuf, 256)) {
            spdlog::info("GsManagerModule::menuHandler: set web-server={0}",
            			 inputBuf);
            gConfig.acquire();
            gConfig.conf["web-server"] = inputBuf;
            gConfig.release(true);
        }

        // Configure auto-refresh
        memset(inputBuf, 0, 256);
        ImGui::LeftLabel("Auto-Refresh Tasks");
        bool autoRefreshEnabled = true;
        if (ImGui::Checkbox("##Auto-Refresh Tasks", &autoRefreshEnabled)) {
            spdlog::info("GsManagerModule::menuHandler: set auto-refresh={0}",
            			 autoRefreshEnabled);
            gConfig.acquire();
            gConfig.conf["auto-refresh-tasks"] = autoRefreshEnabled;
            gConfig.release(true);
        }

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
        		string taskStr;
        		pTask->print(taskStr);
        		ImGui::TextWrapped(taskStr.c_str());
                if (ImGui::Button("Activate")) {
                	_this->onButtonPushActivateTask(pTask);
                }
                ImGui::SameLine();
                std::string taskStatus;
                pTask->getStatus(taskStatus);
                std::string status = "   Status=" + taskStatus;
                ImGui::Text(status.c_str());
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
        		string satelliteStr;
        		pSatellite->print(satelliteStr);
        		ImGui::TextWrapped(satelliteStr.c_str());

                if (ImGui::Button("Track"))
                {
                    spdlog::info("GsManagerModule::menuHandler: track");
                	_this->onButtonPushTrackSatellite(pSatellite);
                }
                ImGui::SameLine();

                if (ImGui::Button("Get Position"))
                {
                    spdlog::info("GsManagerModule::menuHandler: position");
                	_this->onButtonPushGetSatellitePos(pSatellite);
                }

                ImGui::SameLine();
                std::string az;
                pSatellite->getAzimuth(az);
                std::string el;
                pSatellite->getElevation(el);
                std::string position = "   Azimuth=" + az + ", Elevation=" + el;
                ImGui::Text(position.c_str());
        		ImGui::Separator();
        	}
        }
        else
        {
        	ImGui::TextWrapped("No tasks are available.");
        }
    }

    std::string name;
    bool enabled = true;

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
