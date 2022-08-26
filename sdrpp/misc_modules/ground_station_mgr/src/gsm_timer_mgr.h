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
 * gsm_timer_mgr.h
 *
 *  Created on: Aug 14, 2022
 *
 */

#ifndef _GSM_TIMER_MGR_H_
#define _GSM_TIMER_MGR_H_

#include <thread>
#include <vector>

#include "gsm_globals.h"
#include "gsm_timer.h"


typedef struct HeapEntry
{
	unsigned long long time;
	size_t index;

	std::string owner;
	int msgType;

} HeapEntry_t;


class GsmTimerMgr
{
	public:

		static GsmTimerMgr* getInstance();

		virtual ~GsmTimerMgr() {};


		GsmResult_e start();

		static unsigned long long GetCurrentMillisecs();


	private:

		friend class GsmTimer;

		void DetectTimers();

		size_t AddTimer(GsmTimer* timer);
		void RemoveTimer(int timerId);

		void UpHeap(size_t index);
		void DownHeap(size_t index);
		void SwapHeap(size_t, size_t index2);

		void threadEntry();

		void fireTimer(HeapEntry_t& _entry);


	private:

		GsmTimerMgr();

		static GsmTimerMgr* mInstance;

		std::vector<HeapEntry> heap_;

		std::thread* mThread;
};




#endif
