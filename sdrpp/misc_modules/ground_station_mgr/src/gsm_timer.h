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
 * gsm_timer.h
 *
 *  Created on: Aug 25, 2022
 *
 */

#ifndef _GSM_TIMER_H_
#define _GSM_TIMER_H_

#include <string>

#include "gsm_globals.h"


#define GSM_INVALID_TIMER_ID		-1


class GsmTimer
{
	public:

    	enum TimerType { ONCE, CIRCLE };

    	GsmTimer();
    	GsmTimer(const std::string& _owner, int _msgType);
    	virtual ~GsmTimer();

    	void init(const std::string& _owner,
    			  const std::string& _appId,
				  int _msgType);

    	void start();
    	void start(unsigned interval, TimerType timeType = ONCE);
    	void stop();

    	void onTimeout();

    	unsigned getInterval() { return mInterval; }
    	void getOwner(std::string& _owner) {
    		_owner = mOwner;
    	}

    	void getAppId(std::string& _appId) {
    		_appId = mAppId;
    	}

    	int getMsgType() { return mMsgType; }



	private:

    	TimerType mTimerType;
    	int mTimerId;

    	std::string mOwner;
    	std::string mAppId;
    	int mMsgType;

    	unsigned mInterval;

};



#endif
