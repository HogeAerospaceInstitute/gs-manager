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


#include "gsm_event.h"


typedef struct GsmEventIdStr
{
	char idStr[32];
} GsmEventIdStr_t;


GsmEventIdStr_t eventIdStr[GSM_FSM_EVENT_ID_MAX] =
{
	{ "INVALID" },
	{ "ACTIVATE_TASK" },
	{ "TRACKING_RSP" },
	{ "GET_POSITION_RSP" }
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
GsmEvent::init( const GsmMsg& _msg )
{
	mId = convertMsgTypeToEventId( _msg.getType() );
	mName = convertEventIdToStr((GsmFSMEventId_e)mId);
	mData = (void*)&_msg;

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
		case GSM_MSG_TYPE_TRACK_SATELLITE_RSP:
		{
			id = GSM_FSM_EVENT_ID_TRACKING_RSP;
			break;
		}
		case GSM_MSG_TYPE_GET_SATELLITE_POS_RSP:
		{
			id = GSM_FSM_EVENT_ID_GET_POS_RSP;
			break;
		}
		default:
		{
			//meshLogErr( "::convertMsgTypeToEventId: unknown msgType=0x%x",
			//			_msgType );
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

