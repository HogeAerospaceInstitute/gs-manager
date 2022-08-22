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
 * ground_station_mgr.h
 *
 *  Created on: Jul 30, 2022
 *
 */

#ifndef _GROUND_STATION_MGR_H_
#define _GROUND_STATION_MGR_H_

#include "gsm_globals.h"
#include "gsm_comm_consumer.h"
#include "gsm_msg.h"
#include "gsm_task.h"
#include "gsm_satellite.h"


/*
 * GroundStationMgr
 *
 * The GroundStationMgr is a singleton class that implements the main management
 * functions for the ground-station application.  The class inherits from the
 * GsmCommConsumer class for handling messages from other threads.
 *
 */
class GroundStationMgr : public GsmCommConsumer
{

	public:

		static GroundStationMgr* getInstance();

		virtual ~GroundStationMgr() {}


		GsmResult_e onMessage(GsmMsg* _msg);
		GsmResult_e onStart();
		GsmResult_e onShutdown();

		void getTasks(std::map<string, GsmTask*> & _tasks) {
			_tasks = mTasks;
		}

		void getSatellites(std::map<string, GsmSatellite*> & _satellites) {
			_satellites = mSatellites;
		}


	private:

		GroundStationMgr() {
			mName = "GSMGR";
		}

		static GroundStationMgr* mInstance;


		GsmResult_e handleRefreshTasksReq(GsmMsg* _msg);
		GsmResult_e handleRefreshTasksRsp(GsmMsg* _msg);
		GsmResult_e handleClearTasksReq(GsmMsg* _msg);

		GsmResult_e handleActivateTaskReq(GsmMsgActivateTaskReq* _msg);

		GsmResult_e handleTrackSatelliteReq(GsmMsg* _msg);
		GsmResult_e handleGetSatellitePosReq(GsmMsg* _msg);
		GsmResult_e handleGetSatellitePosRsp(GsmMsg* _msg);


		GsmResult_e sendCommand(const std::string& _cmd,
								std::string& _rsp);
		GsmResult_e sendCommandToRotctld(const std::string& _cmd);

		GsmResult_e writeTLEToPredictDb(const std::string& _tle);
		GsmResult_e writeTasksToFile(json& _tasks);

		GsmResult_e addSatellite(const string& _tle);


	private:

	    std::map<string, GsmTask*> mTasks;
	    std::map<string, GsmSatellite*> mSatellites;


};


#endif
