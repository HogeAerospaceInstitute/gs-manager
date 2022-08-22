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

#ifndef _GSM_COMM_MGR_H_
#define _GSM_COMM_MGR_H_

#include <map>
#include <mutex>
#include <string>

#include "gsm_globals.h"
#include "gsm_comm_consumer.h"
#include "gsm_msg.h"


/*
 * 	Maintains list of consumers
 */
class GsmCommMgr {

	public:

		static GsmCommMgr* getInstance();

		virtual ~GsmCommMgr() {}


		GsmResult_e init();

		GsmResult_e registerConsumer(const std::string& _name,
									 GsmCommConsumer* _consumer);
		GsmResult_e sendMsg(GsmMsg* _msg);


	private:

		GsmCommMgr() {}

		static GsmCommMgr* mInstance;

		std::mutex mMutex;
		std::map<const char*, GsmCommConsumer*> mConsumers;

};


#endif
