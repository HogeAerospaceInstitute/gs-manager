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

		void getGroundStationName(std::string& _name )
		{
			_name = mGroundStationName;
		}

		void getLatitude(std::string& _latitude)
		{
			_latitude = mLatitude;
		}

		void getLongitude(std::string& _longitude)
		{
			_longitude = mLongitude;
		}


	private:

		GroundStationMgr() {
			mName = "GSMGR";
		}

		static GroundStationMgr* mInstance;


		GsmResult_e handleRefreshTasksReq(GsmMsg* _msg);
		GsmResult_e handleRefreshTasksRsp(GsmMsg* _msg);
		GsmResult_e handleClearTasksReq(GsmMsg* _msg);
		GsmResult_e handleGetGroundStationInfoReq(GsmMsg* _msg);
		GsmResult_e handleGetGroundStationInfoRsp(GsmMsg* _msg);

		GsmResult_e handleActivateTaskReq(GsmMsgActivateTaskReq* _msg);
		GsmResult_e handleDeactivateTaskReq(GsmMsgDeactivateTaskReq* _msg);

		GsmResult_e handleTrackSatelliteReq(GsmMsgTrackSatelliteReq* _msg);
		GsmResult_e handleTrackSatelliteRsp(GsmMsgTrackSatelliteRsp* _msg);
		GsmResult_e handleTrackSatelliteTimeout(GsmMsgTimeout* _msg);

		GsmResult_e handleGetSatellitePosReq(GsmMsgGetSatellitePosReq* _msg);
		GsmResult_e handleGetSatellitePosRsp(GsmMsgGetSatellitePosRsp* _msg);

		GsmResult_e handleMoveRotatorReq(GsmMsgMoveRotatorReq* _msg);
		GsmResult_e handleMoveRotatorRsp(GsmMsgMoveRotatorRsp* _msg);
		GsmResult_e handleMoveRotatorTimeout(GsmMsgTimeout* _msg);
		GsmResult_e handleGetRotatorPosRsp(GsmMsgGetRotatorPosRsp* _msg);
		GsmResult_e handleCheckRotatorDelayTimeout(GsmMsgTimeout* _msg);
		GsmResult_e handleRecordingDelayTimeout(GsmMsgTimeout* _msg);

		GsmResult_e handlePeriodicTimeout(GsmMsgTimeout* _msg);


		GsmResult_e handleReloadPredictDbRsp(GsmMsgReloadPredictDbRsp* _msg);
		GsmResult_e handleUploadFileRsp(GsmMsgHttpRsp* _msg);

		GsmResult_e sendCommand(const std::string& _cmd,
								std::string& _rsp);
		GsmResult_e sendCommandToRotctld(const std::string& _cmd);

		GsmResult_e writeTLEToPredictDb(const std::string& _tle);
		GsmResult_e writeTasksToFile(json& _tasks);

		GsmResult_e addSatellite(const string& _tle);

		GsmResult_e processRecordings();



	private:

	    std::map<string, GsmTask*> mTasks;
	    std::map<string, GsmSatellite*> mSatellites;

	    GsmTimer mPeriodicTimer;

	    std::string mGroundStationName;
	    std::string mLatitude;
	    std::string mLongitude;

};


#endif
