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
 * gsm_task_state_recording.cpp
 *
 *  Created on: Aug 28, 2022
 *
 */

#include <core.h>
#include <recorder_interface.h>
#include <signal_path/signal_path.h>

#include <spdlog/spdlog.h>

#include "gsm_task_state_recording.h"
#include "gsm_comm_mgr.h"


/*********************************************************************
 *	Name:	onStopRecording
 *	Description:
 *	Parameters: NA
 *	Returns:	NA
 *	Notes:
 *********************************************************************/
GsmFSMResult_e
GsmTaskStateRecording::onStopRecording(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{

	spdlog::info("GsmTaskStateInactive::onStopRecording: entered...");


	sigpath::sourceManager.stop();
    core::modComManager.callInterface("recorder", RECORDER_IFACE_CMD_STOP, NULL, NULL);


	// TODO start timer


	_fsm.setState( (BaseState<GsmTask>*)&mRotatorAlignedState );

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
GsmTaskStateRecording::onDeactivate(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{

	spdlog::info("GsmTaskStateRecording::onDeactivate: entered...");

	// stop recording
	sigpath::sourceManager.stop();
    core::modComManager.callInterface("Recorder", RECORDER_IFACE_CMD_STOP, NULL, NULL);

	_fsm.setState( (BaseState<GsmTask>*)&mInactiveState );

	return GSM_FSM_SUCCESS;
}


