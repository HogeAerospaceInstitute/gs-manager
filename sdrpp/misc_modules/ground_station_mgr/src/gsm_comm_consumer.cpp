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
 * gsm_comm_consumer.cpp
 *
 *  Created on: Jul 30, 2022
 *
 */

#include <thread>

#include <spdlog/spdlog.h>

#include "gsm_globals.h"
#include "gsm_comm_consumer.h"
#include "gsm_msg.h"



GsmCommConsumer::GsmCommConsumer()
{
	mThread = NULL;
}


GsmResult_e GsmCommConsumer::init()
{
	spdlog::info("GsmCommConsumer::init: entered...");
	return GSM_SUCCESS;
}


GsmResult_e GsmCommConsumer::start()
{
	spdlog::info("GsmCommConsumer::start: entered...");

    if ( !mThread )
    {
        mThread = new std::thread(&GsmCommConsumer::msgHandler, this);
    }

	return GSM_SUCCESS;
}


GsmResult_e GsmCommConsumer::shutdown()
{
	spdlog::info("GsmCommConsumer::shutdown: entered...");

	// TODO: create shutdown event and post to queue

	return GSM_SUCCESS;
}


GsmResult_e GsmCommConsumer::postMsg(GsmMsg* _msg)
{
	spdlog::info("GsmCommConsumer::postMsg: entered...");

    // Add user data msg to queue and notify worker thread
    std::unique_lock<std::mutex> lk(mMutex);
    mQueue.push(_msg);
    mCondVar.notify_one();

    return GSM_SUCCESS;
}


void GsmCommConsumer::msgHandler()
{
	spdlog::info("GsmCommConsumer::msgHandler: entered...");

	while (1)
	{
		GsmMsg* pMsg = NULL;
		GsmResult_e result = GSM_SUCCESS;

		// Wait for a message to be added to the queue
		std::unique_lock<std::mutex> lk(mMutex);
		while (mQueue.empty())
		{
			mCondVar.wait(lk);
		}

		if (mQueue.empty())
			continue;

		pMsg = mQueue.front();
		mQueue.pop();

		switch (pMsg->getCategory())
		{
			case GsmMsg::GSM_MSG_CAT_APP:
			{
				result = onMessage(pMsg);

				if (result != GSM_MSG_CACHED)
				{
					free(pMsg);
				}
				break;
			}
			case GsmMsg::GMS_MSG_CAT_COMM:
			{
				if (pMsg->getType() == GSM_MSG_TYPE_START) {
					onStart();
				}
				else if (pMsg->getType() == GSM_MSG_TYPE_SHUTDOWN) {
					onShutdown();
					shutdown();
				}

				break;
			}
			default:
				// unknown categor
				break;
		}

	}

}


GsmResult_e GsmCommConsumer::onMessage(GsmMsg* _msg)
{
	spdlog::info("GsmCommConsumer::onMessage: entered...");
	return GSM_SUCCESS;
}


GsmResult_e GsmCommConsumer::onStart()
{
	spdlog::info("GsmCommConsumer::onStart: entered...");
	return GSM_SUCCESS;
}


GsmResult_e GsmCommConsumer::onShutdown()
{
	spdlog::info("GsmCommConsumer::onShutdown: entered...");
	return GSM_SUCCESS;
}

