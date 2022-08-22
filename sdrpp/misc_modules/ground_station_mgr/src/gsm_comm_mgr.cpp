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
 * gsm_comm_mgr.cpp
 *
 *  Created on: Jul 30, 2022
 */

#include <spdlog/spdlog.h>

#include "gsm_comm_mgr.h"


GsmCommMgr* GsmCommMgr::mInstance = NULL;


GsmCommMgr* GsmCommMgr::getInstance()
{
	if ( mInstance == NULL )
	{
		mInstance = new GsmCommMgr();
	}

	return mInstance;
}


GsmResult_e GsmCommMgr::init()
{
	spdlog::info("GsmCommMgr::init: entered");


	return GSM_SUCCESS;
}


GsmResult_e GsmCommMgr::registerConsumer(const std::string& _name,
										 GsmCommConsumer* _consumer)
{
	mConsumers.insert(std::pair<const char*, GsmCommConsumer*>(_name.c_str(),
															   _consumer));

	spdlog::info("GsmCommMgr::registerConsumer: {0}, num-consumers={1}",
				 _name.c_str(), mConsumers.size());
	return GSM_SUCCESS;
}


GsmResult_e GsmCommMgr::sendMsg(GsmMsg* _msg)
{
	GsmResult_e result = GSM_SUCCESS;
	GsmCommConsumer* pConsumer = NULL;
	std::string destination = _msg->getDestination();

	// TODO: optimize
	std::map<const char*,GsmCommConsumer*>::iterator it;
	for (it = mConsumers.begin(); it != mConsumers.end(); ++it)
	{
		pConsumer = it->second;

		std::string consumer = pConsumer->getName();
		spdlog::info("GsmCommMgr::sendMsg: destination={0}, consumer={1}",
					 destination.c_str(), consumer.c_str());
		int compare = destination.compare(consumer);
		if (compare == 0)
		{
			result = pConsumer->postMsg(_msg);
			break;
		}
	}

    spdlog::info("GsmCommMgr::sendMsg: result={0}", result);
	return result;
}

