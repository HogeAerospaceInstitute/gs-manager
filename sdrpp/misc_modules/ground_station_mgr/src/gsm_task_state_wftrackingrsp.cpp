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
 * gsm_task_state_wftrackingrsp.cpp
 *
 *  Created on: Aug 27, 2022
 *
 */

#include <sstream>

#include <spdlog/spdlog.h>

#include "gsm_task_state_wftrackingrsp.h"
#include "gsm_comm_mgr.h"


/*********************************************************************
 *	Name:	onReloadDbRsp
 *	Description:
 *	Parameters: NA
 *	Returns:	NA
 *	Notes:
 *********************************************************************/
GsmFSMResult_e
GsmTaskStateWfTrackingRsp::onReloadDbRsp(GsmTask& _task,
										 GsmTask::GsmTaskFSM_t& _fsm,
										 const GsmEvent& _event) const
{
	std::string taskId;
	std::string tle;
	std::string satelliteName;

	spdlog::info("GsmTaskStateWfTrackingRsp::onReloadDbRsp: entered...");

	_task.getTrackSatelliteRspTimer().stop();

	_task.getUuid(taskId);
    _task.getTLE(tle);

	// get the satellite name from the tle
    // TODO: optimize
    std::stringstream tleSS(tle);
    std::istream_iterator<std::string> begin(tleSS);
    std::istream_iterator<std::string> end;
    std::vector<std::string> tokens(begin, end);
    satelliteName = tokens[0];

    // get the satellite position from predict.
    GsmMsgGetSatellitePosReq* pMsg = new GsmMsgGetSatellitePosReq();
	pMsg->setDestination("ROTCTRL");
	pMsg->setType(GSM_MSG_TYPE_GET_SATELLITE_POS_REQ);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);
	pMsg->setSatellite(satelliteName);
	pMsg->setTaskId(taskId);

	GsmCommMgr::getInstance()->sendMsg(pMsg);

	_task.getGetSatellitePosRspTimer().start(10000, GsmTimer::ONCE);

	_fsm.setState( (BaseState<GsmTask>*)&mWfGetPosRspState );

	return GSM_FSM_SUCCESS;
}

