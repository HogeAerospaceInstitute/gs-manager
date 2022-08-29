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
 * gsm_task.cpp
 *
 *  Created on: Aug 27, 2022
 *
 */

#include <sstream>

#include <spdlog/spdlog.h>

#include "gsm_task.h"
#include "gsm_task_state_base.h"


/**
 * Default constructor
 */
GsmTask::GsmTask()
	: mTaskFSM(*this,
				(BaseState<GsmTask>*)&GsmTaskStateBase::mInactiveState)

{
	mStartTime = 0;
	mEndTime = 0;
	mFrequency = 0;
	mStatus = "Idle";
}


/**
 * Initializes the task from a string input
 */
void GsmTask::init(const string& _task)
{
	struct tm tm;
	json task;

	spdlog::info("GsmTask::init: entered...");

    task = json::parse(_task);

	mUUID = task["uuid"];
	mTLE = task["tle"];

	// 2022-06-25T21:58:04.644Z
	// convert start time
	string startTime = task["time_start"];
	if (strptime(startTime.c_str(), "%Y-%m-%dT%H:%M:%S", &tm) == NULL) {
			spdlog::error("GsmTask::init: failed to convert start-time={0}...",
			startTime.c_str());
	}
	else
	{
		mStartTime = mktime(&tm);
	}

	// convert end time
	string endTime = task["time_end"];
	if (strptime(endTime.c_str(), "%Y-%m-%dT%H:%M:%S", &tm) == NULL)
	{
		spdlog::error("GsmTask::init: failed to convert end-time={0}...",
				endTime.c_str());
	}
	else
	{
		mEndTime = mktime(&tm);
	}

	// convert string to double
	string frequency = task["frequency"];
	mFrequency = std::stod(frequency);
}


void GsmTask::print(string& _out)
{
	std::stringstream buffer;
	string tmpStr;
	struct tm * timeinfo;
	char startTimeBuf[80];
	char endTimeBuf[80];
	timeinfo = localtime(&mStartTime);
	strftime(startTimeBuf,80,"%d %b %Y %H:%M:%S",timeinfo);

	timeinfo = localtime(&mEndTime);
	strftime(endTimeBuf,80,"%d %b %Y %H:%M:%S",timeinfo);

	buffer << "Id=" << mUUID << "\n" << "    tle=" << mTLE
		   << "\n    start-time=" << startTimeBuf << "\n    end-time="
		   << endTimeBuf << "\n    frequency=" << mFrequency;
		_out = buffer.str();
}


GsmFSMResult_e GsmTask::onEvent(const GsmEvent& _event)
{
	return (GsmFSMResult_e)mTaskFSM.onEvent( &_event );
}





