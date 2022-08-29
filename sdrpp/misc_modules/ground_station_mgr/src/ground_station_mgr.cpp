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


/*
 * ground_station_mgr.cpp
 *
 *  Created on: Aug 13, 2022
 *
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>

#include <config.h>
#include <spdlog/spdlog.h>

#include "ground_station_mgr.h"
#include "gsm_comm_mgr.h"


ConfigManager gConfig;


GroundStationMgr* GroundStationMgr::mInstance = NULL;


GroundStationMgr* GroundStationMgr::getInstance()
{
	if ( mInstance == NULL )
	{
		mInstance = new GroundStationMgr();
	}

	return mInstance;
}


GsmResult_e GroundStationMgr::onMessage(GsmMsg* _msg)
{
	GsmResult_e result = GSM_SUCCESS;

	spdlog::info("GroundStationMgr::onMessage: entered...");

	switch (_msg->getType())
	{
		case GSM_MSG_TYPE_REFRESH_TASKS_REQ:
		{
			result = handleRefreshTasksReq(_msg);
			break;
		}
		case GSM_MSG_TYPE_REFRESH_TASKS_RSP:
		{
			result = handleRefreshTasksRsp(_msg);
			break;
		}
		case GSM_MSG_TYPE_CLEAR_TASKS_REQ:
		{
			result = handleClearTasksReq(_msg);
			break;
		}
		case GSM_MSG_TYPE_ACTIVATE_TASK_REQ:
		{
			result = handleActivateTaskReq(dynamic_cast<GsmMsgActivateTaskReq*>(_msg));
			break;
		}
		case GSM_MSG_TYPE_DEACTIVATE_TASK_REQ:
		{
			result = handleDeactivateTaskReq(dynamic_cast<GsmMsgDeactivateTaskReq*>(_msg));
			break;
		}
		case GSM_MSG_TYPE_TRACK_SATELLITE_REQ:
		{
			result = handleTrackSatelliteReq(dynamic_cast<GsmMsgTrackSatelliteReq*>(_msg));
			break;
		}
		case GSM_MSG_TYPE_TRACK_SATELLITE_RSP:
		{
			result = handleTrackSatelliteRsp(dynamic_cast<GsmMsgTrackSatelliteRsp*>(_msg));
			break;
		}
		case GSM_MSG_TYPE_GET_SATELLITE_POS_REQ:
		{
			result = handleGetSatellitePosReq(dynamic_cast<GsmMsgGetSatellitePosReq*>(_msg));
			break;
		}
		case GSM_MSG_TYPE_GET_SATELLITE_POS_RSP:
		{
			result = handleGetSatellitePosRsp(dynamic_cast<GsmMsgGetSatellitePosRsp*>(_msg));
			break;
		}
		case GSM_MSG_TYPE_MOVE_ROTATOR_REQ:
		{
			result = handleMoveRotatorReq(dynamic_cast<GsmMsgMoveRotatorReq*>(_msg));
			break;
		}
		default:
		{
			// error
			break;
		}
	}

	return result;
}


GsmResult_e GroundStationMgr::onStart()
{
	json tasks;

	spdlog::info("GroundStationMgr::onStart: reading cached tasks...");

	std::ifstream ifs("/var/opt/hai/gsm/tasks.dat");

	try
	{
		tasks = json::parse(ifs);
	}
	catch (json::parse_error& ex)
	{
		spdlog::warn("GroundStationMgr::onStart: failed to parse cached file!");
		return GSM_FAILURE;
	}

	for (const auto& item : tasks.items())
	{
	  	std::stringstream buffer;
	   	string tmpStr;
	   	buffer << item.value();
	   	tmpStr = buffer.str();
       	spdlog::info("GroundStationMgr::onStart: item={0}", tmpStr.c_str());

       	// allocate GsmTask
		GsmTask* pTask = new GsmTask;

		// fill in parameters
		pTask->init(tmpStr);

		string uuidStr;
		pTask->getUuid(uuidStr);
		mTasks.insert(std::pair<std::string, GsmTask*>(uuidStr, pTask));

		// Check and create satellite object
		string tle;
		pTask->getTLE(tle);
		addSatellite(tle);
    }

	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::onShutdown()
{
	spdlog::info("GroundStationMgr::onShutdown: entered...");
	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::handleRefreshTasksReq(GsmMsg* _msg)
{
	spdlog::info("GroundStationMgr::handleRefreshTasksReq: entered...");

	GsmMsg* pMsg = new GsmMsg();
	pMsg->setDestination("HTTPCLIENT");
	pMsg->setType(GSM_MSG_TYPE_REFRESH_TASKS_REQ);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

	GsmCommMgr::getInstance()->sendMsg(pMsg);
	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::handleRefreshTasksRsp(GsmMsg* _msg)
{
	json tasks;

	spdlog::info("GroundStationMgr::handleRefreshTasksRsp: entered...");

	std::string tasksStr = _msg->getData();

	spdlog::info("GroundStationMgr::handleRefreshTasksRsp: tasks={0}",
				 tasksStr.c_str());

	try
	{
		tasks = json::parse(tasksStr);
	}
	catch (json::parse_error& ex)
	{
		spdlog::warn("GroundStationMgr::handleRefreshTasksRsp: failed to parse rsp");
		return GSM_FAILURE;
	}

	for (const auto& item : tasks.items())
	{
	  	std::stringstream buffer;
	   	string tmpStr;
	   	buffer << item.value();
	   	tmpStr = buffer.str();
       	spdlog::info("GroundStationMgr::handleRefreshTasksRsp: item={0}",
       				  tmpStr.c_str());

       	// allocate GsmTask
		GsmTask* pTask = new GsmTask;

		// fill in parameters
		pTask->init(tmpStr);

		string uuidStr;
		pTask->getUuid(uuidStr);
		mTasks.insert(std::pair<std::string, GsmTask*>(uuidStr, pTask));

		// Check and create satellite object
		string tle;
		pTask->getTLE(tle);

		// DEBUG code do not commit
		// tle = "ISS (ZARYA)\r\n1 44713C 19074A   22087.43454506 -.00034486  00000-0 -23100-2 0   873\r\n2 44713  53.0526 181.1476 0001373  69.1389  60.3646 15.06442553    11";

		// parse TLE
	    std::vector<std::string> lines;

	    std::stringstream ss(tle);
	    std::string line;
	    while (std::getline(ss, line, '\n')) {
	    	lines.push_back(line);
	    }

	    if (lines.size() != 3)
	    {
	       	spdlog::warn("GroundStationMgr::handleRefreshTasksRsp: tle does not have title!");
	    }
	    else
	    {
	    	// only use first word of title line for satellite name
	    	std::string titleLine = lines[0];
	       	spdlog::info("GroundStationMgr::handleRefreshTasksRsp: title={0}", titleLine.c_str());

	       	std::string newtitle = titleLine.substr(0, titleLine.find(" "));
	       	newtitle = newtitle + "\r\n";

	       	std::string newtle;
	       	lines[0] = newtitle;

	       	newtle = lines[0] + lines[1] + lines[2];
	       	spdlog::info("GroundStationMgr::handleRefreshTasksRsp: new-tle={0}", newtle.c_str());
	       	tle = newtle;
	    }

		addSatellite(tle);
    }

	writeTasksToFile(tasks);
	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::handleClearTasksReq(GsmMsg* _msg)
{
	spdlog::info("GroundStationMgr::handleClearTasksReq: entered...");

	mTasks.clear();

	// TODO stop any task related activity

	// TODO: remove tasks from data file

	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::handleActivateTaskReq(GsmMsgActivateTaskReq* _msg)
{
	GsmTask* pTask = NULL;

	spdlog::info("GroundStationMgr::handleActivateTaskReq: entered...");

	if (_msg == NULL)
	{
		spdlog::error("GroundStationMgr::handleActivateTaskReq: invalid msg!!");
		return GSM_FAILURE;
	}

	std::string taskId;
	_msg->getTaskId(taskId);

	// TODO: optimize
	bool bFound = false;
	std::map<string,GsmTask*>::iterator it;
	for (it = mTasks.begin(); it != mTasks.end(); ++it) {
		pTask = it->second;

		if (pTask == NULL)
		{
			continue;
		}

		// TODO: get id from message
		std::string taskUUID;
		pTask->getUuid(taskUUID);
		int compare = taskUUID.compare(taskId);
		if (compare == 0)
		{
			bFound = true;
			break;
		}
	}

	if (bFound == true)
	{
		std::string status = "Activated";
		pTask->setStatus(status);
	}

	// Send event to state machine
	GsmEvent* pEvent = new GsmEvent();
	pEvent->init(*_msg);
	pTask->onEvent(*pEvent);

	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::handleDeactivateTaskReq(GsmMsgDeactivateTaskReq* _msg)
{
	GsmTask* pTask = NULL;

	spdlog::info("GroundStationMgr::handleDeactivateTaskReq: entered...");

	if (_msg == NULL)
	{
		spdlog::error("GroundStationMgr::handleDeactivateTaskReq: invalid msg!!");
		return GSM_FAILURE;
	}

	std::string taskId;
	_msg->getTaskId(taskId);

	// TODO: optimize
	bool bFound = false;
	std::map<string,GsmTask*>::iterator it;
	for (it = mTasks.begin(); it != mTasks.end(); ++it) {
		pTask = it->second;

		if (pTask == NULL)
		{
			continue;
		}

		// TODO: get id from message
		std::string taskUUID;
		pTask->getUuid(taskUUID);
		int compare = taskUUID.compare(taskId);
		if (compare == 0)
		{
			bFound = true;
			break;
		}
	}

	if (bFound == true)
	{
		std::string status = "Idle";
		pTask->setStatus(status);
	}

	// TODO: stop recording

	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::handleTrackSatelliteReq(GsmMsgTrackSatelliteReq* _msg)
{
	std::string tle;
    std::string satelliteName;
	GsmSatellite* pSatellite = NULL;

	spdlog::info("GroundStationMgr::handleTrackSatelliteReq: entered...");

    _msg->getSatelliteName(satelliteName);

	// send message to predict to reload TLE db

    // find if satellite exists
    // TODO optimize, put into function
    bool bFound = false;
	std::map<std::string, GsmSatellite*>::iterator it;
	for (it = mSatellites.begin(); it != mSatellites.end(); ++it)
	{
		pSatellite = it->second;

		std::string satname;
		pSatellite->getName(satname);
		int compare = satelliteName.compare(satname);
		if (compare == 0)
		{
			bFound = true;
		}
	}

	if (bFound == true)
	{
		pSatellite->getTLE(tle);
	}

	// update predict data file with TLE
	writeTLEToPredictDb(tle);

	GsmMsg* pMsg = new GsmMsg();
	pMsg->setDestination("ROTCTRL");
	pMsg->setType(GSM_MSG_TYPE_RELOAD_PREDICT_DB_REQ);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

	GsmCommMgr::getInstance()->sendMsg(pMsg);

	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::handleTrackSatelliteRsp(GsmMsgTrackSatelliteRsp* _msg)
{
	spdlog::info("GroundStationMgr::handleTrackSatelliteRsp: entered...");


	// TODO get task Id from message

	GsmEvent* pEvent = new GsmEvent();
	pEvent->init(*_msg);
//	pTask->onEvent(*pEvent);

	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::handleGetSatellitePosReq(GsmMsgGetSatellitePosReq* _msg)
{
	std::string satelliteName;

	_msg->getSatelliteName(satelliteName);

	spdlog::info("GroundStationMgr::handleGetSatellitePosReq: satellite={0}",
			satelliteName.c_str());

	GsmMsgGetSatellitePosReq* pMsg = new GsmMsgGetSatellitePosReq();

	pMsg->setDestination("ROTCTRL");
	pMsg->setType(GSM_MSG_TYPE_GET_SATELLITE_POS_REQ);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);
	pMsg->setSatelliteName(satelliteName);

	GsmCommMgr::getInstance()->sendMsg(pMsg);

	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::handleMoveRotatorReq(GsmMsgMoveRotatorReq* _msg)
{
	GsmSatellite* pSatellite = NULL;
	std::string satelliteName;
	std::string azimuth;
	std::string elevation;

	_msg->getSatelliteName(satelliteName);

	spdlog::info("GroundStationMgr::handleMoveRotatorReq: satellite={0}",
			satelliteName.c_str());

    // find if satellite exists
    // TODO optimize, put into function
    bool bFound = false;
	std::map<std::string, GsmSatellite*>::iterator it;
	for (it = mSatellites.begin(); it != mSatellites.end(); ++it)
	{
		pSatellite = it->second;

		std::string satname;
		pSatellite->getName(satname);
		int compare = satelliteName.compare(satname);
		if (compare == 0)
		{
			bFound = true;
		}
	}

	if (bFound == true)
	{
		pSatellite->getName(satelliteName);
		pSatellite->getAzimuth(azimuth);
		pSatellite->getElevation(elevation);
	}

	GsmMsgMoveRotatorReq* pMsg = new GsmMsgMoveRotatorReq();

	pMsg->setDestination("ROTCTRL");
	pMsg->setType(GSM_MSG_TYPE_MOVE_ROTATOR_REQ);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);
	pMsg->setSatelliteName(satelliteName);
	pMsg->setAzimuth(azimuth);
	pMsg->setElevation(elevation);

	GsmCommMgr::getInstance()->sendMsg(pMsg);

	return GSM_SUCCESS;
}



GsmResult_e GroundStationMgr::handleGetSatellitePosRsp(GsmMsgGetSatellitePosRsp* _msg)
{
	GsmSatellite* pSatellite = NULL;
	std::string satelliteName;

	spdlog::info("GroundStationMgr::handleGetSatellitePosRsp: entered...");

    // parse the response
	std::string rsp = _msg->getData();
    std::stringstream responseSS(rsp);
    std::istream_iterator<std::string> begin(responseSS);
    std::istream_iterator<std::string> end;
    std::vector<std::string> tokens(begin, end);

    std::string az = tokens[5];
    std::string el = tokens[4];

    _msg->getSatelliteName(satelliteName);

    // find if satellite exists
    // TODO optimize, put into function
    bool bFound = false;
	std::map<std::string, GsmSatellite*>::iterator it;
	for (it = mSatellites.begin(); it != mSatellites.end(); ++it)
	{
		pSatellite = it->second;

		std::string satname;
		pSatellite->getName(satname);
		int compare = satelliteName.compare(satname);
		if (compare == 0)
		{
			bFound = true;
		}
	}

	if (bFound == true)
	{
		pSatellite->setAzimuth(az);
		pSatellite->setElevation(el);
	}

	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::writeTasksToFile(json& _tasks)
{
	spdlog::info("GroundStationMgr::writeTasksToFile: entered...");

	std::ofstream file("/var/opt/hai/gsm/tasks.dat");
	file << _tasks;
	file.flush();

	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::writeTLEToPredictDb(const std::string& _tle)
{
	spdlog::info("GroundStationMgr::writeTLEToPredictDb: entered...");

	// TODO: make file location configurable
	std::ofstream file("/var/opt/hai/gsm/.predict/predict.tle");
	file << _tle;
	file.flush();

	return GSM_SUCCESS;
}


GsmResult_e GroundStationMgr::addSatellite(const string& _tle)
{
	spdlog::info("GroundStationMgr::addSatellite: entered...");

    // parse the response
    std::stringstream tleSS(_tle);
    std::istream_iterator<std::string> begin(tleSS);
    std::istream_iterator<std::string> end;
    std::vector<std::string> tokens(begin, end);

    std::string satelliteName = tokens[0];

    // find if satellite exists
    // TODO optimize
    bool bFound = false;
	std::map<std::string, GsmSatellite*>::iterator it;
	for (it = mSatellites.begin(); it != mSatellites.end(); ++it)
	{
		GsmSatellite* pSatellite = NULL;
		pSatellite = it->second;

		std::string satname;
		pSatellite->getName(satname);
		int compare = satelliteName.compare(satname);
		if (compare == 0)
		{
			bFound = true;
		}
	}

	if (bFound == false)
	{
       	// allocate GsmTask
		GsmSatellite* pSatellite = new GsmSatellite;
		pSatellite->setName(satelliteName);
		pSatellite->setTLE(_tle);
		mSatellites.insert(std::pair<std::string, GsmSatellite*>(satelliteName, pSatellite));
		spdlog::info("GroundStationMgr::addSatellite: num-satellites={0}", mSatellites.size());
	}

	return GSM_SUCCESS;
}


#if 0
void handleHttpRsp() {

	if (httpTxn.status == GSM_HTTP_TRANSACTION_STATE_COMPLETE) {
		spdlog::info("GsManagerModule::handleHttpRsp: recv rsp");

		// TODO: clear existing list of tasks

		if (httpTxn.rsp.data[0] != '\0') {
			string tasksStr = httpTxn.rsp.data;

			// parse into json object
			json tasks = json::parse(tasksStr);

		    for (const auto& item : tasks.items())
		    {
		    	std::stringstream buffer;
		    	string tmpStr;
		    	buffer << item.value();
		    	tmpStr = buffer.str();
	        	spdlog::info("GsManagerModule::handleHttpRsp: item={0}", tmpStr.c_str());

	        	// allocate GsmTask
				GsmTask* pTask = new GsmTask;

				// fill in parameters
				pTask->init(tmpStr);

				string uuidStr;
				pTask->getUuid(uuidStr);
				mTasks.insert(std::pair<std::string, GsmTask*>(uuidStr, pTask));

		        //std::cout << item.key() << "\n";
		        //for (const auto& val : item.value().items())
		        //{
		        	//spdlog::info("GsManagerModule::handleHttpRsp: item={0}", item.value());
		           // std::cout << "  " << val.key() << ": " << val.value() << "\n";
		        //}
		    }
#if 0
			// iterate the array
			for (json::iterator it = tasks.begin(); it != tasks.end(); ++it) {

				// allocate GsmTask
				GsmTask* pTask = new GsmTask;

				// fill in parameters
				pTask->init(it->second);
				json task = *it;
				string uuidStr = task["UUID"];
				mTasks.insert(std::pair<std::string, GsmTask*>(uuidStr, pTask));
			}
#endif
		}
		else {
    		spdlog::error("GsManagerModule::handleHttpRsp: no data in rsp!");
		}

		// reset httpTxn
		memset(&httpTxn, 0, sizeof(GsmHttpTransaction_t));
		httpTxn.status = GSM_HTTP_TRANSACTION_STATE_NULL;
	}

}
#endif

