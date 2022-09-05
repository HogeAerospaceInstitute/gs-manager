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
 *      Author: jrenkema
 */

#include <spdlog/spdlog.h>

#include "gsm_timer.h"
#include "gsm_timer_mgr.h"


GsmTimer::GsmTimer()
{
	mTimerId = -1;
}


GsmTimer::GsmTimer(const std::string& _owner, int _msgType)
{
	mTimerId = -1;
	mOwner = _owner;
	mMsgType = _msgType;
	mInterval = 0;
}


GsmTimer::~GsmTimer()
{
    stop();
}

//template<typename Fun>
void GsmTimer::start(unsigned _interval, TimerType _type)
{
	spdlog::info("GsmTimer::start: entered...");

	if (mTimerId != -1)
	{
		spdlog::info("GsmTimer::start: timer already started...");
		return;
	}

 //   stop();
    mInterval = _interval;
//    timerFun_ = fun;
    mTimerType = _type;
 //   expires_ = interval_ + GsmTimerMgr::GetCurrentMillisecs();
    mTimerId = GsmTimerMgr::getInstance()->AddTimer(this);
}


void GsmTimer::stop()
{
	spdlog::info("GsmTimer::stop: entered...");

    if (mTimerId != -1)
    {
        GsmTimerMgr::getInstance()->RemoveTimer(mTimerId);
        mTimerId = -1;
    }
}


void GsmTimer::onTimeout()
{
	spdlog::info("GsmTimer::onTimeout: entered...");

    if (mTimerType == GsmTimer::CIRCLE)
    {
        //expires_ = mInterval + now;
        mTimerId = GsmTimerMgr::getInstance()->AddTimer(this);
    }
    else
    {
    	mTimerId = -1;
    }
}

