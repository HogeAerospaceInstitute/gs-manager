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
 * gsm_comm_consumer.h
 *
 *  Created on: Jul 30, 2022
 *
 */

#ifndef _GSM_COMM_CONSUMER_H_
#define _GSM_COMM_CONSUMER_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "gsm_globals.h"
#include "gsm_msg.h"


class GsmCommConsumer {

	public:

		GsmCommConsumer();
		virtual ~GsmCommConsumer() {}

		GsmResult_e init();
		GsmResult_e start();
		GsmResult_e shutdown();

		GsmResult_e postMsg(GsmMsg* _msg);
		void msgHandler();

		// Virtual functions
		virtual GsmResult_e onMessage(GsmMsg* _msg);
		virtual GsmResult_e onStart();
		virtual GsmResult_e onShutdown();


		const char* getName() { return mName.c_str(); }


	protected:

		std::string mName;


	private:


		std::thread* mThread;

		std::queue<GsmMsg*> mQueue;
		std::mutex mMutex;
		std::condition_variable mCondVar;

};


#endif
