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
 * gsm_event.h
 *
 *  Created on: Aug 28, 2022
 *
 */

#ifndef _GSM_EVENT_H_
#define _GSM_EVENT_H_


#include "BaseStateMachine.h"

#include "gsm_globals.h"
#include "gsm_msg.h"



//---------------------- Types/Macros ---------------------------------
typedef enum GsmFSMEventId
{
	GSM_FSM_EVENT_ID_INVALID = 0,
	GSM_FSM_EVENT_ID_ACTIVATE_TASK,
	GSM_FSM_EVENT_ID_TRACKING_RSP,
	GSM_FSM_EVENT_ID_GET_POS_RSP,
	GSM_FSM_EVENT_ID_MAX
} GsmFSMEventId_e;



class GsmEvent : public BaseEvent
{

	public:

		GsmEvent();
		GsmEvent( const GsmMsg& _msg );

		virtual ~GsmEvent() {}

		GsmResult_e init( const GsmMsg& _msg );


		void* getData() const { return mData; }
		void setData( void* _data ) { mData = _data; }

		GsmMsg* getMsg() const { return (GsmMsg*)mData; }


	private:

		const char* convertEventIdToStr(GsmFSMEventId_e _id);
		GsmFSMEventId_e convertMsgTypeToEventId(int _msgType);


	private:

		void* mData;


};


#endif
