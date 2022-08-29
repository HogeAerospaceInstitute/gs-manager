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
 * gsm_task_state_inactive.cpp
 *
 *  Created on: Aug 28, 2022
 *
 */

#include <fstream>

#include <spdlog/spdlog.h>

#include "gsm_task_state_inactive.h"
#include "gsm_comm_mgr.h"


/*********************************************************************
 *	Name:	onActivate
 *	Description:
 *	Parameters: NA
 *	Returns:	NA
 *	Notes:
 *********************************************************************/
GsmFSMResult_e
GsmTaskStateInactive::onActivate(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{
	std::string tle;
	std::string satelliteName;

	spdlog::info("GsmTaskStateInactive::onActivate: entered...");

    _task.getTLE(tle);

	// update predict data file with TLE
	writeTLEToPredictDb(tle);

	GsmMsg* pMsg = new GsmMsg();
	pMsg->setDestination("ROTCTRL");
	pMsg->setType(GSM_MSG_TYPE_RELOAD_PREDICT_DB_REQ);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

	GsmCommMgr::getInstance()->sendMsg(pMsg);

	// TODO start timer


	_fsm.setState( (BaseState<GsmTask>*)&mWfTrackingRspState );

	return GSM_FSM_SUCCESS;
}



GsmResult_e GsmTaskStateInactive::writeTLEToPredictDb(const std::string& _tle) const
{
	spdlog::info("GroundStationMgr::writeTLEToPredictDb: entered...");

	// TODO: make file location configurable
	std::ofstream file("/var/opt/hai/gsm/.predict/predict.tle");
	file << _tle;
	file.flush();

	return GSM_SUCCESS;
}


