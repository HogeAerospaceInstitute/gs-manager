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

#ifndef _GSM_TASK_H_
#define _GSM_TASK_H_

#include <string>
#include <json.hpp>

#include "BaseStateMachine.h"
#include "gsm_event.h"
#include "gsm_timer.h"


using namespace std;
using nlohmann::json;


class GsmTask
{

	public:

		/**
		 * Default constructor
		 */
		GsmTask();
		virtual ~GsmTask() 		{}


	public:

		typedef BaseFSM<GsmTask> GsmTaskFSM_t;

		GsmFSMResult_e onEvent(const GsmEvent& _event);

		unsigned int getCurrentStateId() const
							{ return mTaskFSM.getCurrentState()->getId(); }

		const BaseState<GsmTask>* getCurrentState() const
							{ return mTaskFSM.getCurrentState(); }


		void init(const string& _task);
		void print(string& _out);

		void getUuid(string& _uuid) const {
			_uuid = mUUID;
		}

		void getTLE(string& _tle) const {
			_tle = mTLE;
		}

		void getStatus(string& _status) const {
			_status = mStatus;
		}

		void setStatus(string& _status) {
			mStatus = _status;
		}

		GsmTimer& getWfGetPosRspTimer() { return mWfGetPosRspTimer; }
		GsmTimer& getRecordingDelayTimer() { return mRecordingDelayTimer; }


	private:

		string mUUID;
		string mTLE;

		time_t mStartTime;
		time_t mEndTime;

		double mFrequency;

		string mStatus;

		GsmTaskFSM_t mTaskFSM;

		GsmTimer mWfGetPosRspTimer;
		GsmTimer mWfTrackingRspTimer;
		GsmTimer mWfStartRecordingTimer;
		GsmTimer mWfQueryRotatorPosTimer;
		GsmTimer mRecordingDelayTimer;

};


#endif
