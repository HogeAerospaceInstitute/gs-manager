/* -----------------------------------------------------------------------------
 * Copyright (C) 2022, Hoge Aerospace Institute
 * This software is the confidential and proprietary information
 * of Hoge Aerospace Institute.
 *
 * ALL RIGHTS RESERVED
 *
 * Permission is hereby granted to licensees of Hoge Aerospace
 * Institute products to use or abstract this computer program for
 * the sole purpose of implementing a product based on Hoge
 * Aerospace Institute products.  No other rights to reproduce, use,
 * or disseminate this computer program, whether in part or in
 * whole, are granted.
 *
 * Hoge Aerospace Institute makes no representation or warranties
 * with respect to the performance of this computer program, and
 * specifically disclaims any responsibility for any damages,
 * special or consequential, connected with the use of this program.
 * -----------------------------------------------------------------------------
 */

/*
 * gsm_task_state_base.h
 *
 *  Created on: Aug 27, 2022
 *
 */

#ifndef _GSM_TASK_STATE_BASE_H_
#define _GSM_TASK_STATE_BASE_H_


#include "BaseStateMachine.h"

#include "gsm_globals.h"
#include "gsm_task.h"


//---------------------- Forward class declarations -------------------
class GsmTaskStateInactive;
class GsmTaskStateWfTrackingRsp;
class GsmTaskStateWfGetPosRsp;
class GsmTaskStateWfRotatorAligned;
class GsmTaskStateRotatorAligned;
class GsmTaskStateRecording;



//---------------------- Types/Macros ---------------------------------
typedef enum GsmTasktates
{
	GSM_TASK_STATE_INACTIVE = 0,
	GSM_TASK_STATE_WF_TRACKING_RSP,
	GSM_TASK_STATE_WF_GET_POS_RSP,
	GSM_TASK_STATE_WF_ROTATOR_ALIGNED,
	GSM_TASK_STATE_ROTATOR_ALIGNED,
	GSM_TASK_STATE_RECORDING
} GsmTaskStates_e;



class GsmTaskStateBase : public BaseState<GsmTask>
{
	friend class GsmTask;

	public:

		GsmTaskStateBase(unsigned int _id, const char* _name)
			: BaseState<GsmTask> (_id, _name)  {};


		virtual ~GsmTaskStateBase() {}


		virtual void enter(GsmTask& _task,
						   GsmTask::GsmTaskFSM_t& _fsm);

		virtual void exit(GsmTask& _task,
						  GsmTask::GsmTaskFSM_t& _fsm);

		virtual int onEvent(GsmTask& _task,
							GsmTask::GsmTaskFSM_t& _fsm,
						 	const BaseEvent* _event);

		GsmFSMResult_e invalidEvent(const GsmEvent& _event) const;


		virtual GsmFSMResult_e onActivate(GsmTask& _task,
										  GsmTask::GsmTaskFSM_t& _fsm,
										  const GsmEvent& _event ) const;

		virtual GsmFSMResult_e onDeactivate(GsmTask& _task,
										  GsmTask::GsmTaskFSM_t& _fsm,
										  const GsmEvent& _event ) const;

		virtual GsmFSMResult_e onTrackingRsp(GsmTask& _task,
										  	 GsmTask::GsmTaskFSM_t& _fsm,
											 const GsmEvent& _event ) const;

		virtual GsmFSMResult_e onGetPosRsp(GsmTask& _task,
										   GsmTask::GsmTaskFSM_t& _fsm,
									       const GsmEvent& _event ) const;

		virtual GsmFSMResult_e onReloadDbRsp(GsmTask& _task,
										     GsmTask::GsmTaskFSM_t& _fsm,
									         const GsmEvent& _event ) const;

		virtual GsmFSMResult_e onMoveRotatorRsp(GsmTask& _task,
										     GsmTask::GsmTaskFSM_t& _fsm,
									         const GsmEvent& _event ) const;

		virtual GsmFSMResult_e onMoveRotatorTimeout(GsmTask& _task,
										     GsmTask::GsmTaskFSM_t& _fsm,
									         const GsmEvent& _event ) const;

		virtual GsmFSMResult_e onGetRotatorPosRsp(GsmTask& _task,
										     GsmTask::GsmTaskFSM_t& _fsm,
									         const GsmEvent& _event ) const;

		virtual GsmFSMResult_e onCheckRotatorDelayTimeout(GsmTask& _task,
										 GsmTask::GsmTaskFSM_t& _fsm,
										 const GsmEvent& _event) const;

		virtual GsmFSMResult_e onRecordingDelayTimeout(GsmTask& _task,
										 GsmTask::GsmTaskFSM_t& _fsm,
										 const GsmEvent& _event) const;


	protected:

		static GsmTaskStateInactive mInactiveState;
		static GsmTaskStateWfTrackingRsp mWfTrackingRspState;
		static GsmTaskStateWfGetPosRsp mWfGetPosRspState;
		static GsmTaskStateWfRotatorAligned mWfRotatorAlignedState;
		static GsmTaskStateRotatorAligned mRotatorAlignedState;
		static GsmTaskStateRecording mRecordingState;


};

#endif
