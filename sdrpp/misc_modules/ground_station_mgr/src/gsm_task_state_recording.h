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
 * gsm_task_state_recording.h
 *
 *  Created on: Aug 28, 2022
 *
 */

#ifndef _GSM_TASK_STATE_RECORDING_H_
#define _GSM_TASK_STATE_RECORDING_H_


#include "gsm_task.h"
#include "gsm_task_state_base.h"



class GsmTaskStateRecording : public GsmTaskStateBase
{
	public:

		GsmTaskStateRecording()
			: GsmTaskStateBase( GSM_TASK_STATE_RECORDING,
								   "GSM_TASK_STATE_RECORDING" )  {}

		virtual ~GsmTaskStateRecording() {}


		virtual GsmFSMResult_e onStopRecording(GsmTask& _task,
										  GsmTask::GsmTaskFSM_t& _fsm,
									      const GsmEvent& _event ) const;


	private:



};


#endif /* _GSM_TASK_STATE_RECORDING_H_ */
