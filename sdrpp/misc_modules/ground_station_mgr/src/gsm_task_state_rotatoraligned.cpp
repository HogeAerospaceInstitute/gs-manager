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
 * gsm_task_state_rotatoraligned.cpp
 *
 *  Created on: Sep 3, 2022
 *
 */

#include <core.h>
#include <recorder_interface.h>
#include <signal_path/signal_path.h>

#include <spdlog/spdlog.h>

#include "gsm_task_state_rotatoraligned.h"
#include "gsm_comm_mgr.h"


/*********************************************************************
 *	Name:	onRecordingDelayTimeout
 *	Description:
 *	Parameters: NA
 *	Returns:	NA
 *	Notes:
 *********************************************************************/
GsmFSMResult_e
GsmTaskStateRotatorAligned::onRecordingDelayTimeout(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{

	spdlog::info("GsmTaskStateInactive::onRecordingDelayTimeout: entered...");

	_task.getRecordingDelayTimer().onTimeout();

	// start recording
	sigpath::sourceManager.start();
    core::modComManager.callInterface("Recorder", RECORDER_IFACE_CMD_START, NULL, NULL);

	_fsm.setState( (BaseState<GsmTask>*)&mRecordingState );

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
GsmTaskStateRotatorAligned::onDeactivate(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{

	spdlog::info("GsmTaskStateRotatorAligned::onDeactivate: entered...");

	_task.getRecordingDelayTimer().stop();

	_fsm.setState( (BaseState<GsmTask>*)&mInactiveState );

	return GSM_FSM_SUCCESS;
}

