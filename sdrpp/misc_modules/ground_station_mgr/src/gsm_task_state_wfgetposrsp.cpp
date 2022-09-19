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
 * gsm_task_state_wfgetposrsp.cpp
 *
 *  Created on: Aug 27, 2022
 *
 */

#include <spdlog/spdlog.h>

#include "gsm_task_state_wfgetposrsp.h"
#include "gsm_comm_mgr.h"


/*********************************************************************
 *	Name:	onGetPosRsp
 *	Description:
 *	Parameters: NA
 *	Returns:	NA
 *	Notes:
 *********************************************************************/
GsmFSMResult_e
GsmTaskStateWfGetPosRsp::onGetPosRsp(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{
	std::string taskId;
	std::string azimuth;
	std::string elevation;

	_task.getGetSatellitePosRspTimer().stop();

	_task.getUuid(taskId);

    // Get az/el from response message
    GsmMsgGetSatellitePosRsp* pRsp = (GsmMsgGetSatellitePosRsp*)_event.getMsg();
    pRsp->getAzimuth(azimuth);
    pRsp->getElevation(elevation);

	spdlog::info("GsmTaskStateWfGetPosRsp::onGetPosRsp: azimuth={0}, elevation={1}",
			azimuth.c_str(), elevation.c_str());

    // save the target az/el in the task
    _task.setAzimuth(azimuth);
    _task.setElevation(elevation);

    // send message to move rotator
    GsmMsgMoveRotatorReq* pMsg = new GsmMsgMoveRotatorReq();
	pMsg->setDestination("ROTCTRL");
	pMsg->setType(GSM_MSG_TYPE_MOVE_ROTATOR_REQ);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);
	pMsg->setTaskId(taskId);
	pMsg->setAzimuth(azimuth);
	pMsg->setElevation(elevation);

	GsmCommMgr::getInstance()->sendMsg((GsmMsg*)pMsg);

	_task.getMoveRotatorRspTimer().start(10000, GsmTimer::ONCE);

	_fsm.setState( (BaseState<GsmTask>*)&mWfRotatorAlignedState );

	return GSM_FSM_SUCCESS;
}


/*********************************************************************
 *	Name:	onDeactivate
 *	Description:
 *	Parameters: NA
 *	Returns:	NA
 *	Notes:
 *********************************************************************/
GsmFSMResult_e
GsmTaskStateWfGetPosRsp::onDeactivate(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{

	spdlog::info("GsmTaskStateWfGetPosRsp::onDeactivate: entered");

	_task.getGetSatellitePosRspTimer().stop();

	_fsm.setState( (BaseState<GsmTask>*)&mInactiveState );

	return GSM_FSM_SUCCESS;
}

