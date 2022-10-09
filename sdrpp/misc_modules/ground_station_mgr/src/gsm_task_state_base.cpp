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
 * gsm_task_state_base.cpp
 *
 *  Created on: Aug 28, 2022
 *
 */

#include <spdlog/spdlog.h>

#include "gsm_task_state_base.h"
#include "gsm_task_state_inactive.h"
#include "gsm_task_state_wftrackingrsp.h"
#include "gsm_task_state_wfgetposrsp.h"
#include "gsm_task_state_wfrotatoraligned.h"
#include "gsm_task_state_rotatoraligned.h"
#include "gsm_task_state_recording.h"


GsmTaskStateInactive GsmTaskStateBase::mInactiveState;
GsmTaskStateWfTrackingRsp GsmTaskStateBase::mWfTrackingRspState;
GsmTaskStateWfGetPosRsp GsmTaskStateBase::mWfGetPosRspState;
GsmTaskStateWfRotatorAligned GsmTaskStateBase::mWfRotatorAlignedState;
GsmTaskStateRotatorAligned GsmTaskStateBase::mRotatorAlignedState;
GsmTaskStateRecording GsmTaskStateBase::mRecordingState;



int
GsmTaskStateBase::onEvent(GsmTask& _task,
						  GsmTask::GsmTaskFSM_t& _fsm,
						  const BaseEvent* _event)
{
	GsmFSMResult_e result = GSM_FSM_SUCCESS;
	GsmEvent* gsmEvent = (GsmEvent*)_event;

	switch ( _event->getId() )
	{
		case GSM_FSM_EVENT_ID_ACTIVATE_TASK:
		{
			result = onActivate(_task, _fsm, *gsmEvent);
			break;
		}
		case GSM_FSM_EVENT_ID_DEACTIVATE_TASK:
		{
			result = onDeactivate(_task, _fsm, *gsmEvent);
			break;
		}
		case GSM_FSM_EVENT_ID_TRACKING_RSP:
		{
			result = onTrackingRsp(_task, _fsm, *gsmEvent);
			break;
		}
		case GSM_FSM_EVENT_ID_GET_POS_RSP:
		{
			result = onGetPosRsp(_task, _fsm, *gsmEvent);
			break;
		}
		case GSM_FSM_EVENT_ID_RELOAD_DB_RSP:
		{
			result = onReloadDbRsp(_task, _fsm, *gsmEvent);
			break;
		}
		case GSM_FSM_EVENT_ID_MOVE_ROTATOR_RSP:
		{
			result = onMoveRotatorRsp(_task, _fsm, *gsmEvent);
			break;
		}
		case GSM_FSM_EVENT_ID_MOVE_ROTATOR_TIMEOUT:
		{
			result = onMoveRotatorTimeout(_task, _fsm, *gsmEvent);
			break;
		}
		case GSM_FSM_EVENT_ID_GET_ROTATOR_POS_RSP:
		{
			result = onGetRotatorPosRsp(_task, _fsm, *gsmEvent);
			break;
		}
		case GSM_FSM_EVENT_ID_CHECK_ROTATOR_DELAY_TIMEOUT:
		{
			result = onCheckRotatorDelayTimeout(_task, _fsm, *gsmEvent);
			break;
		}
		case GSM_FSM_EVENT_ID_RECORDING_DELAY_TIMEOUT:
		{
			result = onRecordingDelayTimeout(_task, _fsm, *gsmEvent);
			break;
		}
		default:
		{
			spdlog::error("GsmTaskStateBase::onEvent: invalid event={0}",
					_event->getId());
			break;
		}
	}

	return result;
}


void
GsmTaskStateBase::enter(GsmTask& _task, GsmTask::GsmTaskFSM_t& _fsm )
{
	spdlog::info( "GsmTaskFSM: ENTERING state={0}", getNameStr() );
}


void
GsmTaskStateBase::exit( GsmTask& _task,
		GsmTask::GsmTaskFSM_t& _fsm )
{
	spdlog::info( "GsmTaskFSM: EXITING state={0}", getNameStr() );
}


GsmFSMResult_e
GsmTaskStateBase::invalidEvent( const GsmEvent& _event ) const
{
	spdlog::error("GsmTaskStateBase::invalidEvent: invalid event={0} state={1}",
			_event.getId(), getNameStr());

	return GSM_FSM_FAILURE;
}


GsmFSMResult_e
GsmTaskStateBase::onActivate( GsmTask& _task,
							  GsmTask::GsmTaskFSM_t& _fsm,
								   const GsmEvent& _event ) const
{
	spdlog::error("GsmTaskStateBase::onActivate: event not handled!!");
	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onDeactivate( GsmTask& _task,
							  GsmTask::GsmTaskFSM_t& _fsm,
								   const GsmEvent& _event ) const
{
	spdlog::error("GsmTaskStateBase::onDeactivate: event not handled!!");
	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onTrackingRsp( GsmTask& _task,
							  GsmTask::GsmTaskFSM_t& _fsm,
								   const GsmEvent& _event ) const
{
	spdlog::error("GsmTaskStateBase::onTrackingRsp: event not handled!!");
	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onGetPosRsp( GsmTask& _task,
							  GsmTask::GsmTaskFSM_t& _fsm,
								   const GsmEvent& _event ) const
{
	spdlog::error("GsmTaskStateBase::onGetPosRsp: event not handled!!");
	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onReloadDbRsp( GsmTask& _task,
							  GsmTask::GsmTaskFSM_t& _fsm,
								   const GsmEvent& _event ) const
{
	spdlog::error("GsmTaskStateBase::onReloadDbRsp: event not handled!!");
	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onMoveRotatorRsp( GsmTask& _task,
							  	  GsmTask::GsmTaskFSM_t& _fsm,
								   const GsmEvent& _event ) const
{
	spdlog::error("GsmTaskStateBase::onMoveRotatorRsp: event not handled!!");
	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onMoveRotatorTimeout( GsmTask& _task,
							  	  GsmTask::GsmTaskFSM_t& _fsm,
								   const GsmEvent& _event ) const
{
	spdlog::error("GsmTaskStateBase::onMoveRotatorTimeout: event not handled!!");
	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onGetRotatorPosRsp( GsmTask& _task,
							  	  	  GsmTask::GsmTaskFSM_t& _fsm,
									  const GsmEvent& _event ) const
{
	spdlog::error("GsmTaskStateBase::onMoveRotatorRsp: event not handled!!");
	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onCheckRotatorDelayTimeout(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{
	spdlog::error("GsmTaskStateBase::onCheckRotatorDelayTimeout: event not handled!!");
	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onRecordingDelayTimeout(GsmTask& _task,
								 GsmTask::GsmTaskFSM_t& _fsm,
								 const GsmEvent& _event) const
{
	spdlog::error("GsmTaskStateBase::onRecordingDelayTimeout: event not handled!!");
	return invalidEvent( _event );
}

