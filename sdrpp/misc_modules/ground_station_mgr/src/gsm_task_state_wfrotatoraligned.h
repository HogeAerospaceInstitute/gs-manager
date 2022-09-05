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
 * gsm_task_state_wfrotatoraligned.h
 *
 *  Created on: Sep 3, 2022
 *      Author: jrenkema
 */

#ifndef _GSM_TASK_STATE_WFROTATORALIGNED_H_
#define _GSM_TASK_STATE_WFROTATORALIGNED_H_

#include "gsm_task.h"
#include "gsm_task_state_base.h"



class GsmTaskStateWfRotatorAligned : public GsmTaskStateBase
{
	public:

		GsmTaskStateWfRotatorAligned()
			: GsmTaskStateBase( GSM_TASK_STATE_WF_ROTATOR_ALIGNED,
								   "GSM_TASK_STATE_WF_ROTATOR_ALIGNED" )  {}

		virtual ~GsmTaskStateWfRotatorAligned() {}


		virtual GsmFSMResult_e onMoveRotatorRsp(GsmTask& _task,
										  GsmTask::GsmTaskFSM_t& _fsm,
									      const GsmEvent& _event ) const;

		virtual GsmFSMResult_e onQueryRotatorPosRsp(GsmTask& _task,
										  GsmTask::GsmTaskFSM_t& _fsm,
									      const GsmEvent& _event ) const;


	private:



};


#endif
