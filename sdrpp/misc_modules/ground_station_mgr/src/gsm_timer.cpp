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
 * gsm_timer.cpp
 *
 *  Created on: Aug 25, 2022
 *
 */

#include <spdlog/spdlog.h>

#include "gsm_timer.h"
#include "gsm_timer_mgr.h"


GsmTimer::GsmTimer()
{
	mTimerId = GSM_INVALID_TIMER_ID;
	mMsgType = 0;
	mInterval = 0;
	mTimerType = ONCE;
}


GsmTimer::GsmTimer(const std::string& _owner, int _msgType)
{
	mTimerId = GSM_INVALID_TIMER_ID;
	mOwner = _owner;
	mMsgType = _msgType;
	mInterval = 0;
	mTimerType = ONCE;
}


GsmTimer::~GsmTimer()
{
    stop();
}


void GsmTimer::init(const std::string& _owner,  const std::string& _appId, int _msgType)
{
	mOwner = _owner;
	mAppId = _appId;
	mMsgType = _msgType;
}


void GsmTimer::start(unsigned _interval, TimerType _type)
{
	spdlog::info("GsmTimer::start: interval={0}, type={1}",
			_interval, _type);

	if (mTimerId != GSM_INVALID_TIMER_ID)
	{
		spdlog::info("GsmTimer::start: timer already started...");
		return;
	}

    mInterval = _interval;
    mTimerType = _type;
    mTimerId = GsmTimerMgr::getInstance()->AddTimer(this);
}


void GsmTimer::stop()
{
	spdlog::info("GsmTimer::stop: timer-id={0}", mTimerId);

    if (mTimerId != GSM_INVALID_TIMER_ID)
    {
        GsmTimerMgr::getInstance()->RemoveTimer(mTimerId);
        mTimerId = GSM_INVALID_TIMER_ID;
    }
}


void GsmTimer::onTimeout()
{
	spdlog::info("GsmTimer::onTimeout: entered...");

    if (mTimerType == GsmTimer::CIRCLE)
    {
        mTimerId = GsmTimerMgr::getInstance()->AddTimer(this);
    }
    else
    {
    	mTimerId = GSM_INVALID_TIMER_ID;
    }
}

