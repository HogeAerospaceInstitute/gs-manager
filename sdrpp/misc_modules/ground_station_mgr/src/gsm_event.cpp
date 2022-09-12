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
 * gsm_event.cpp
 *
 *  Created on: Aug 27, 2022
 *
 */

#include <spdlog/spdlog.h>

#include "gsm_event.h"


typedef struct GsmEventIdStr
{
	char idStr[32];
} GsmEventIdStr_t;


GsmEventIdStr_t eventIdStr[GSM_FSM_EVENT_ID_MAX] =
{
	{ "INVALID" },
	{ "ACTIVATE_TASK" },
	{ "DEACTIVATE_TASK" },
	{ "TRACKING_RSP" },
	{ "TRACKING_TIMEOUT" },
	{ "GET_POSITION_RSP" },
	{ "GET_POSITION_TIMEOUT" },
	{ "RELOAD_DB_RSP" },
	{ "RELOAD_DB_TIMEOUT" },
	{ "MOVE_ROTATOR_RSP" },
	{ "MOVE_ROTATOR_TIMEOUT" },
	{ "GET_ROTATOR_POS_RSP" },
	{ "GET_ROTATOR_POS_TIMEOUT" },
	{ "CHECK_ROTATOR_DELAY_TIMEOUT" },
	{ "RECORDING_DELAY_TIMEOUT" }
};


GsmEvent::GsmEvent() : BaseEvent()
{
	mData = NULL;
}


GsmEvent::GsmEvent( const GsmMsg& _msg ) : BaseEvent()
{
	mId = convertMsgTypeToEventId( _msg.getType() );
	mName = convertEventIdToStr( (GsmFSMEventId_e)mId );
	mData = (void*)&_msg;
}


GsmResult_e
GsmEvent::init(const GsmMsg& _msg)
{
	mId = convertMsgTypeToEventId( _msg.getType() );
	mName = convertEventIdToStr((GsmFSMEventId_e)mId);
	mData = (void*)&_msg;

	return GSM_SUCCESS;
}

GsmResult_e
GsmEvent::init(GsmFSMEventId_e _id, std::string _name)
{
	mId = _id;
	mName = _name;

	return GSM_SUCCESS;
}


GsmFSMEventId_e
GsmEvent::convertMsgTypeToEventId( int _msgType )
{
	GsmFSMEventId_e id = GSM_FSM_EVENT_ID_INVALID;

	switch ( _msgType )
	{
		case GSM_MSG_TYPE_ACTIVATE_TASK_REQ:
		{
			id = GSM_FSM_EVENT_ID_ACTIVATE_TASK;
			break;
		}
		case GSM_MSG_TYPE_DEACTIVATE_TASK_REQ:
		{
			id = GSM_FSM_EVENT_ID_DEACTIVATE_TASK;
			break;
		}
		case GSM_MSG_TYPE_TRACK_SATELLITE_RSP:
		{
			id = GSM_FSM_EVENT_ID_TRACKING_RSP;
			break;
		}
		case GSM_MSG_TYPE_TRACK_SATELLITE_TIMEOUT:
		{
			id = GSM_FSM_EVENT_ID_TRACKING_TIMEOUT;
			break;
		}
		case GSM_MSG_TYPE_GET_SATELLITE_POS_RSP:
		{
			id = GSM_FSM_EVENT_ID_GET_POS_RSP;
			break;
		}
		case GSM_MSG_TYPE_GET_SATELLITE_POS_TIMEOUT:
		{
			id = GMS_FSM_EVENT_ID_GET_POS_TIMEOUT;
			break;
		}
		case GSM_MSG_TYPE_RELOAD_PREDICT_DB_RSP:
		{
			id = GSM_FSM_EVENT_ID_RELOAD_DB_RSP;
			break;
		}
		case GSM_MSG_TYPE_RELOAD_PREDICT_DB_TIMEOUT:
		{
			id = GSM_FSM_EVENT_ID_RELOAD_DB_TIMEOUT;
			break;
		}
		case GSM_MSG_TYPE_MOVE_ROTATOR_RSP:
		{
			id = GSM_FSM_EVENT_ID_MOVE_ROTATOR_RSP;
			break;
		}
		case GSM_MSG_TYPE_MOVE_ROTATOR_TIMEOUT:
		{
			id = GSM_FSM_EVENT_ID_MOVE_ROTATOR_TIMEOUT;
			break;
		}
		case GSM_MSG_TYPE_GET_ROTATOR_POS_RSP:
		{
			id = GSM_FSM_EVENT_ID_GET_ROTATOR_POS_RSP;
			break;
		}
		case GSM_MSG_TYPE_GET_ROTATOR_POS_TIMEOUT:
		{
			id = GSM_FSM_EVENT_ID_GET_ROTATOR_POS_TIMEOUT;
			break;
		}
		case GSM_MSG_TYPE_CHECK_ROTATOR_DELAY_TIMEOUT:
		{
			id = GSM_FSM_EVENT_ID_CHECK_ROTATOR_DELAY_TIMEOUT;
			break;
		}
		case GSM_MSG_TYPE_RECORDING_DELAY_TIMEOUT:
		{
			id = GSM_FSM_EVENT_ID_RECORDING_DELAY_TIMEOUT;
			break;
		}
		default:
		{
			spdlog::error("GsmEvent::convertMsgTypeToEventId: unknown msg-type={0}",
					_msgType);

			break;
		}
	}

	return id;
}


const char*
GsmEvent::convertEventIdToStr( GsmFSMEventId_e _id )
{

	if ( _id > GSM_FSM_EVENT_ID_MAX )
		return "INVALID";

	return eventIdStr[_id].idStr;
}

