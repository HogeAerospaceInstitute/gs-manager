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
 * gsm_task_state_wfrotatoraligned.cpp
 *
 *  Created on: Sep 3, 2022
 *
 */

#include <spdlog/spdlog.h>

#include "gsm_task_state_wfrotatoraligned.h"
#include "gsm_comm_mgr.h"
#include "gsm_msg.h"


/*********************************************************************
 *	Name:	onGetPosRsp
 *	Description:
 *	Parameters: NA
 *	Returns:	NA
 *	Notes:
 *********************************************************************/
GsmFSMResult_e
GsmTaskStateWfRotatorAligned::onMoveRotatorRsp(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{
//	GsmMsgMoveRotatorRsp* pRsp = (GsmMsgMoveRotatorRsp*)_event.getMsg();

	spdlog::info("GsmTaskStateWfRotatorAligned::onMoveRotatorRsp: entered");

	// TODO: check the result

	_task.getMoveRotatorRspTimer().stop();
	_task.getCheckRotatorDelayTimer().start(5000, GsmTimer::ONCE);

	return GSM_FSM_SUCCESS;
}


/*********************************************************************
 *	Name:	onGetPosRsp
 *	Description:
 *	Parameters: NA
 *	Returns:	NA
 *	Notes:
 *********************************************************************/
GsmFSMResult_e
GsmTaskStateWfRotatorAligned::onCheckRotatorDelayTimeout(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{
	std::string taskId;

	spdlog::info("GsmTaskStateWfRotatorAligned::onCheckRotatorDelayTimeout: entered...");

	_task.getCheckRotatorDelayTimer().onTimeout();
	_task.getUuid(taskId);

	GsmMsgGetRotatorPosReq* pMsg = new GsmMsgGetRotatorPosReq();
	pMsg->setDestination("ROTCTRL");
	pMsg->setType(GSM_MSG_TYPE_GET_ROTATOR_POS_REQ);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);
	pMsg->setTaskId(taskId);

	GsmCommMgr::getInstance()->sendMsg((GsmMsg*)pMsg);

	_task.getGetRotatePosRspTimer().start(10000, GsmTimer::ONCE);

	return GSM_FSM_SUCCESS;
}


/*********************************************************************
 *	Name:	onGetPosRsp
 *	Description:
 *	Parameters: NA
 *	Returns:	NA
 *	Notes:
 *********************************************************************/
GsmFSMResult_e
GsmTaskStateWfRotatorAligned::onGetRotatorPosRsp(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{
	std::string targetAzimuth;
	std::string targetElevation;
	std::string currentAzimuth;
	std::string currentElevation;

	GsmMsgGetRotatorPosRsp* pMsg = (GsmMsgGetRotatorPosRsp*)_event.getMsg();

	_task.getGetRotatePosRspTimer().stop();

	// check whether the position matches the expected position
	_task.getAzimuth(targetAzimuth);
	_task.getElevation(targetElevation);

	pMsg->getAzimuth(currentAzimuth);
	pMsg->getElevation(currentElevation);

	spdlog::info("GsmTaskStateWfRotatorAligned::onGetRotatorPosRsp: azimuth={0}, elevation={1}",
			currentAzimuth.c_str(), currentElevation.c_str());

	if ((currentAzimuth == targetAzimuth) &&
		(currentElevation == targetElevation))
	{
		// rotator is aligned
		_task.getRecordingDelayTimer().start(10000, GsmTimer::ONCE);
		_fsm.setState( (BaseState<GsmTask>*)&mRotatorAlignedState );
	}
	else
	{
		_task.getCheckRotatorDelayTimer().start(5000, GsmTimer::ONCE);
	}

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
GsmTaskStateWfRotatorAligned::onDeactivate(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{

	spdlog::info("GsmTaskStateWfRotatorAligned::onDeactivate: entered");

	_task.getGetRotatePosRspTimer().stop();
	_task.getCheckRotatorDelayTimer().stop();

	_fsm.setState( (BaseState<GsmTask>*)&mInactiveState );

	return GSM_FSM_SUCCESS;
}


