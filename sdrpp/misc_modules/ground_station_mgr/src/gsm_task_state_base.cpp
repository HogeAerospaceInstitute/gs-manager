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


#include "gsm_task_state_base.h"
#include "gsm_task_state_inactive.h"
#include "gsm_task_state_wftrackingrsp.h"
#include "gsm_task_state_wfgetposrsp.h"
//#include "gsm_task_state_wfrotatoraligned.h"
//#include "gsm_task_state_rotatoraligned.h"
//#include "gsm_task_state_recording.h"


GsmTaskStateInactive GsmTaskStateBase::mInactiveState;
GsmTaskStateWfTrackingRsp GsmTaskStateBase::mWfTrackingRspState;
GsmTaskStateWfGetPosRsp GsmTaskStateBase::mWfGetPosRspState;
//GsmTaskStateWfRotatorAligned GsmTaskStateBase::mWfRotatorAlignedState;
//GsmTaskStateRotatorAligned GsmTaskStateBase::mRotatorAlignedState;
//GsmTaskStateRecording GsmTaskStateBase::mRecordingState;



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

		default:
		{
			// invalid event type
			//meshLogErr( "::OnEvent: invalid event=%d",	meshEvent->getId() );
			break;
		}
	}

	return result;
}


void
GsmTaskStateBase::enter(GsmTask& _task, GsmTask::GsmTaskFSM_t& _fsm )
{
	// meshLogDbg( "GwNodeFSM: ENTERING state=%s", getNameStr() );
}


void
GsmTaskStateBase::exit( GsmTask& _task,
		GsmTask::GsmTaskFSM_t& _fsm )
{
	// meshLogDbg( "GwNodeFSM: EXITING state=%s", getNameStr() );
}


GsmFSMResult_e
GsmTaskStateBase::invalidEvent( const GsmEvent& _event ) const
{
//	meshLogErr( "GwNodeFSM: invalid event=%d rcv in state=%s",
//				_event.getId(), getNameStr() );

	return GSM_FSM_FAILURE;
}


GsmFSMResult_e
GsmTaskStateBase::onActivate( GsmTask& _task,
							  GsmTask::GsmTaskFSM_t& _fsm,
								   const GsmEvent& _event ) const
{
	// meshLogErr( "::OnRoleChange: recv msg" );

	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onTrackingRsp( GsmTask& _task,
							  GsmTask::GsmTaskFSM_t& _fsm,
								   const GsmEvent& _event ) const
{
	// meshLogErr( "::OnRoleChange: recv msg" );

	return invalidEvent( _event );
}


GsmFSMResult_e
GsmTaskStateBase::onGetPosRsp( GsmTask& _task,
							  GsmTask::GsmTaskFSM_t& _fsm,
								   const GsmEvent& _event ) const
{
	// meshLogErr( "::OnRoleChange: recv msg" );

	return invalidEvent( _event );
}





