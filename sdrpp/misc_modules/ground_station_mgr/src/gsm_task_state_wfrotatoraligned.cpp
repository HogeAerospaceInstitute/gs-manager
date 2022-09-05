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
	std::string taskId;
	std::string tle;
	std::string satelliteName;

	GsmMsgMoveRotatorRsp* pRsp = (GsmMsgMoveRotatorRsp*)_event.getMsg();

	spdlog::info("GsmTaskStateInactive::onMoveRotatorRsp: entered...");

	pRsp->getTaskId(taskId);

	// TODO: start timer to query rotator position


	GsmMsgGetRotatorPosReq* pMsg = new GsmMsgGetRotatorPosReq();
	pMsg->setDestination("ROTCTRL");
	pMsg->setType(GSM_MSG_TYPE_QUERY_ROTATOR_POS_REQ);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);
	pMsg->setTaskId(taskId);

	GsmCommMgr::getInstance()->sendMsg(pMsg);

	// TODO start timer


	// _fsm.setState( (BaseState<GsmTask>*)&mRotatorAlignedState );

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
GsmTaskStateWfRotatorAligned::onQueryRotatorPosRsp(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{
	std::string tle;
	std::string satelliteName;

	// GsmMsg* pMsg = _event.getMsg();

	spdlog::info("GsmTaskStateWfRotatorAligned::onQueryRotatorPosRsp: entered...");

	// TODO: check whether the position matches the expected position

	// TODO if not match start timer and remain in state

	// TODO if match start timer to start recording


#if 0
	GsmMsg* pMsg = new GsmMsg();
	pMsg->setDestination("ROTCTRL");
	pMsg->setType(GSM_MSG_TYPE_MOVE_ROTATOR_REQ);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);


	GsmCommMgr::getInstance()->sendMsg(pMsg);
#endif


	// TODO start timer


	_fsm.setState( (BaseState<GsmTask>*)&mRotatorAlignedState );

	return GSM_FSM_SUCCESS;
}


