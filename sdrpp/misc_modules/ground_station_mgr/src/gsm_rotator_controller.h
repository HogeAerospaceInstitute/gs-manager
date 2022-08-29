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
 * gsm_rotator_controller.h
 *
 *  Created on: Jul 30, 2022
 *
 */

#ifndef _GSM_ROTATOR_CONTROLLER_H_
#define _GSM_ROTATOR_CONTROLLER_H_

#include "gsm_globals.h"
#include "gsm_comm_consumer.h"
#include "gsm_msg.h"


/*
 * GsmRotatorController
 *
 * The GsmRotatorController is a singleton class that implements the
 * interface towards 'predict' to control which satellite to track and adjust
 * the rotator to point to its position.  The class inherits from the
 * GsmCommConsumer class for handling messages from other threads.
 *
 */
class GsmRotatorController : public GsmCommConsumer
{

	public:

		static GsmRotatorController* getInstance();

		virtual ~GsmRotatorController() {}


		GsmResult_e onMessage(GsmMsg* _msg);
		GsmResult_e onStart();
		GsmResult_e onShutdown();


	private:

		GsmRotatorController() {
			mName = "ROTCTRL";
		}

		static GsmRotatorController* mInstance;


		GsmResult_e handleTrackSatelliteReq(GsmMsg* _msg);
		GsmResult_e handleGetSatellitePosReq(GsmMsgGetSatellitePosReq* _msg);
		GsmResult_e handleReloadPredictDbReq(GsmMsg* _msg);
		GsmResult_e handleMoveRotatorReq(GsmMsgMoveRotatorReq* _msg);

		GsmResult_e sendCommand(const std::string& _cmd,
								std::string& _rsp);
		GsmResult_e sendCommandToRotctld(const std::string& _cmd);


};


#endif
