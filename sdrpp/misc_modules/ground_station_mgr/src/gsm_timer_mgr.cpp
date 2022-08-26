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
 * gsm_timer_mgr.cpp
 *
 *  Created on: Aug 14, 2022
 *
 */

#define _CRT_SECURE_NO_WARNINGS
#include "config.h"
#ifdef _MSC_VER
# include <sys/timeb.h>
#else
# include <sys/time.h>
#endif

#include <spdlog/spdlog.h>

#include "gsm_timer_mgr.h"
#include "gsm_msg.h"
#include "gsm_comm_mgr.h"



GsmTimerMgr* GsmTimerMgr::mInstance = NULL;


GsmTimerMgr* GsmTimerMgr::getInstance()
{
	if ( mInstance == NULL )
	{
		mInstance = new GsmTimerMgr();
	}

	return mInstance;
}


GsmTimerMgr::GsmTimerMgr()
{
}


void GsmTimerMgr::threadEntry()
{
	spdlog::info("GsmTimerMgr::threadEntry: entered...");

    while (1)
    {
        DetectTimers();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


GsmResult_e GsmTimerMgr::start()
{
	spdlog::info("GsmTimerMgr::start: entered...");

    if ( !mThread )
    {
        mThread = new std::thread(&GsmTimerMgr::threadEntry, this);
    }

	return GSM_SUCCESS;
}



size_t GsmTimerMgr::AddTimer(GsmTimer* timer)
{
	HeapEntry entry;
	size_t timerId = heap_.size();

	// timer->heapIndex_ = heap_.size();
    // HeapEntry entry = { timer->expires_, timer };
    // expires_ = interval_ + GsmTimerMgr::GetCurrentMillisecs();

    entry.time = timer->getInterval() + GetCurrentMillisecs();
    entry.msgType = timer->getMsgType();
    timer->getOwner(entry.owner);
    heap_.push_back(entry);
    UpHeap(heap_.size() - 1);

    return timerId;
}


void GsmTimerMgr::RemoveTimer(int _timerId)
{
    size_t index = _timerId;
    if (!heap_.empty() && index < heap_.size())
    {
        if (index == heap_.size() - 1)
        {
            heap_.pop_back();
        }
        else
        {
            SwapHeap(index, heap_.size() - 1);
            heap_.pop_back();
            size_t parent = (index - 1) / 2;
            if (index > 0 && heap_[index].time < heap_[parent].time)
                UpHeap(index);
            else
                DownHeap(index);
        }
    }
}


void GsmTimerMgr::DetectTimers()
{
    unsigned long long now = GetCurrentMillisecs();

    while (!heap_.empty() && heap_[0].time <= now)
    {
    	int index = heap_[0].index;
    	fireTimer(heap_[0]);
        RemoveTimer(index);

        //timer->OnTimer(now);
    }
}

void GsmTimerMgr::UpHeap(size_t index)
{
    size_t parent = (index - 1) / 2;
    while (index > 0 && heap_[index].time < heap_[parent].time)
    {
        SwapHeap(index, parent);
        index = parent;
        parent = (index - 1) / 2;
    }
}

void GsmTimerMgr::DownHeap(size_t index)
{
    size_t child = index * 2 + 1;
    while (child < heap_.size())
    {
        size_t minChild = (child + 1 == heap_.size() || heap_[child].time < heap_[child + 1].time)
            ? child : child + 1;
        if (heap_[index].time < heap_[minChild].time)
            break;
        SwapHeap(index, minChild);
        index = minChild;
        child = index * 2 + 1;
    }
}

void GsmTimerMgr::SwapHeap(size_t index1, size_t index2)
{
    HeapEntry tmp = heap_[index1];
    heap_[index1] = heap_[index2];
    heap_[index2] = tmp;
    heap_[index1].index = index1;
    heap_[index2].index = index2;
}


unsigned long long GsmTimerMgr::GetCurrentMillisecs()
{
#ifdef _MSC_VER
    _timeb timebuffer;
    _ftime(&timebuffer);
    unsigned long long ret = timebuffer.time;
    ret = ret * 1000 + timebuffer.millitm;
    return ret;
#else
    timeval tv;
    ::gettimeofday(&tv, 0);
    unsigned long long ret = tv.tv_sec;
    return ret * 1000 + tv.tv_usec / 1000;
#endif
}


void GsmTimerMgr::fireTimer(HeapEntry_t& _entry)
{
	GsmMsg* pMsg = new GsmMsg();
	pMsg->setDestination(_entry.owner.c_str());
	pMsg->setType(_entry.msgType);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

	GsmCommMgr::getInstance()->sendMsg(pMsg);
}


