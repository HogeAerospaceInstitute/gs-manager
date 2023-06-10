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
 * gsm_rotator_controller.cpp
 *
 *  Created on: Jul 30, 2022
 *
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#include <spdlog/spdlog.h>

#include "gsm_rotator_controller.h"
#include "gsm_comm_mgr.h"


GsmRotatorController* GsmRotatorController::mInstance = NULL;


GsmRotatorController* GsmRotatorController::getInstance()
{
	if ( mInstance == NULL )
	{
		mInstance = new GsmRotatorController();
	}

	return mInstance;
}


GsmResult_e GsmRotatorController::onMessage(GsmMsg* _msg)
{
	GsmResult_e result = GSM_SUCCESS;

	spdlog::info("GsmRotatorController::onMessage: entered...");

	switch (_msg->getType())
	{
		case GSM_MSG_TYPE_TRACK_SATELLITE_REQ:
		{
			result = handleTrackSatelliteReq(_msg);
			break;
		}
		case GSM_MSG_TYPE_GET_SATELLITE_POS_REQ:
		{
			result = handleGetSatellitePosReq(dynamic_cast<GsmMsgGetSatellitePosReq*>(_msg));
			break;
		}
		case GSM_MSG_TYPE_RELOAD_PREDICT_DB_REQ:
		{
			result = handleReloadPredictDbReq(dynamic_cast<GsmMsgReloadPredictDbReq*>(_msg));
			break;
		}
		case GSM_MSG_TYPE_MOVE_ROTATOR_REQ:
		{
			result = handleMoveRotatorReq(dynamic_cast<GsmMsgMoveRotatorReq*>(_msg));
			break;
		}
		case GSM_MSG_TYPE_GET_ROTATOR_POS_REQ:
		{
			result = handleGetRotatorPosReq(dynamic_cast<GsmMsgGetRotatorPosReq*>(_msg));
			break;
		}
		default:
		{
			// error
			break;
		}
	}

	return result;
}


GsmResult_e GsmRotatorController::onStart()
{
	spdlog::info("GsmRotatorController::onStart: entered...");

	// TODO establish connection to predict
	return GSM_SUCCESS;
}


GsmResult_e GsmRotatorController::onShutdown()
{
	spdlog::info("GsmRotatorController::onShutdown: entered...");
	return GSM_SUCCESS;
}


GsmResult_e GsmRotatorController::handleTrackSatelliteReq(GsmMsg* _msg)
{
	std::string rsp;
    std::string az = "0";
    std::string el = "0";

	spdlog::info("GsmRotatorController::handleTrackSatelliteReq: entered...");

	// TODO fill in data from message

    // Send command to rotctld
    std::string rotcommand = "P " + az + " " + el;
    sendCommandToRotctld(rotcommand, rsp);

	return GSM_SUCCESS;
}


GsmResult_e GsmRotatorController::handleGetSatellitePosReq(GsmMsgGetSatellitePosReq* _msg)
{
	std::string satellite;
	std::string command;
	std::string rsp;
	std::string taskId;

	_msg->getSatellite(satellite);
	_msg->getTaskId(taskId);

	spdlog::info("GsmRotatorController::handleGetSatellitePosReq: satellite={0}",
			satellite.c_str());

	command = "GET_SAT_POS " + satellite;

	// send message to predict
	if (sendCommand(command.c_str(), rsp) != GSM_SUCCESS)
	{
		spdlog::error("GsmRotatorController::handleGetSatellitePosReq: failed!!");
		// TODO: send failure rsp message
		return GSM_FAILURE;
	}

	// verify response
	if (rsp.empty())
	{
		spdlog::error("GsmRotatorController::handleGetSatellitePosReq: empty response!!");
		// TODO: send failure rsp message
		return GSM_FAILURE;
	}

    // parse the response
    std::stringstream responseSS(rsp);
    std::istream_iterator<std::string> begin(responseSS);
    std::istream_iterator<std::string> end;
    std::vector<std::string> tokens(begin, end);

    // verify string contents
    if (tokens.size() < 5)
    {
		spdlog::error("GsmRotatorController::handleGetSatellitePosReq: invalid string!!");
		// TODO: send failure rsp message
		return GSM_FAILURE;
    }

    std::string az = tokens[5];
    std::string el = tokens[4];

    spdlog::info("GsmRotatorController::sendCommand: az={0}, el={1}",
    			  az.c_str(), el.c_str());

    // send response
    GsmMsgGetSatellitePosRsp* pMsg = new GsmMsgGetSatellitePosRsp();

	pMsg->setDestination("GSMGR");
	pMsg->setType(GSM_MSG_TYPE_GET_SATELLITE_POS_RSP);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);

	pMsg->setData((char*)rsp.c_str(), (int)rsp.size());
	pMsg->setTaskId(taskId);
	pMsg->setSatellite(satellite);
	pMsg->setElevation(el);
	pMsg->setAzimuth(az);

	GsmCommMgr::getInstance()->sendMsg(pMsg);

	return GSM_SUCCESS;
}


GsmResult_e GsmRotatorController::handleReloadPredictDbReq(GsmMsgReloadPredictDbReq* _msg)
{
	std::string rsp;
	std::string taskId;

	spdlog::info("GsmRotatorController::handleReloadPredictDbReq: entered...");

	_msg->getTaskId(taskId);

	std::string command = "RELOAD_TLE";

	// send message to predict
	if (sendCommand(command.c_str(), rsp) != GSM_SUCCESS)
	{
		spdlog::error("GsmRotatorController::handleReloadPredictDbReq: failed!!");

		// TODO: send failure response message
		return GSM_FAILURE;
	}

    // send response
	GsmMsgReloadPredictDbRsp* pMsg = new GsmMsgReloadPredictDbRsp();

	pMsg->setDestination("GSMGR");
	pMsg->setType(GSM_MSG_TYPE_RELOAD_PREDICT_DB_RSP);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);
	pMsg->setTaskId(taskId);

	GsmCommMgr::getInstance()->sendMsg((GsmMsg*)pMsg);

	return GSM_SUCCESS;
}


GsmResult_e GsmRotatorController::handleMoveRotatorReq(GsmMsgMoveRotatorReq* _msg)
{
	std::string taskId;
	std::string rsp;
    std::string az;
    std::string el;

    _msg->getTaskId(taskId);
	_msg->getElevation(el);
	_msg->getAzimuth(az);

	spdlog::info("GsmRotatorController::handleMoveRotatorReq: az={0}, el={1}",
			az.c_str(), el.c_str());

    // Send move/position command to rotctld
    std::string rotcommand = "P " + az + " " + el;
    sendCommandToRotctld(rotcommand, rsp);

    // TODO: fill in success/failure in response

    spdlog::info("GsmRotatorController::handleMoveRotatorReq: sending response");

    // send response
	GsmMsgMoveRotatorRsp* pMsg = new GsmMsgMoveRotatorRsp();

	pMsg->setDestination("GSMGR");
	pMsg->setType(GSM_MSG_TYPE_MOVE_ROTATOR_RSP);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);
	pMsg->setTaskId(taskId);

	GsmCommMgr::getInstance()->sendMsg((GsmMsg*)pMsg);

	return GSM_SUCCESS;
}


GsmResult_e GsmRotatorController::handleGetRotatorPosReq(GsmMsgGetRotatorPosReq* _msg)
{
	GsmResult_e result = GSM_SUCCESS;
	std::string taskId;
	std::string rsp;

    _msg->getTaskId(taskId);

	spdlog::info("GsmRotatorController::handleGetRotatorPosReq: taskId={0}",
			taskId.c_str());

    // Send get position command to rotctld
    std::string rotcommand = "p";
    result = sendCommandToRotctld(rotcommand, rsp);

	// verify response
	if (rsp.empty())
	{
		spdlog::error("GsmRotatorController::handleGetRotatorPosReq: empty response!!");
		// TODO: send failure rsp message
		return GSM_FAILURE;
	}

    // send response
	GsmMsgGetRotatorPosRsp* pMsg = new GsmMsgGetRotatorPosRsp();

	pMsg->setDestination("GSMGR");
	pMsg->setType(GSM_MSG_TYPE_GET_ROTATOR_POS_RSP);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);
	pMsg->setTaskId(taskId);

    // parse the response
	if (result == GSM_SUCCESS)
	{
		std::stringstream responseSS(rsp);
		std::istream_iterator<std::string> begin(responseSS);
		std::istream_iterator<std::string> end;
		std::vector<std::string> tokens(begin, end);

	    // verify string contents
	    if (tokens.size() < 2)
	    {
			spdlog::error("GsmRotatorController::handleGetSatellitePosReq: invalid string!!");
			// TODO: send failure rsp message
			return GSM_FAILURE;
	    }

	    if ((tokens[0] == "RPRT"))
	    {
			spdlog::error("GsmRotatorController::handleGetSatellitePosReq: error rsp!!");
			// TODO: send failure rsp message
			return GSM_FAILURE;
	    }

		std::string az = tokens[0];
		std::string el = tokens[1];

		pMsg->setAzimuth(az);
		pMsg->setElevation(el);
	}

	GsmCommMgr::getInstance()->sendMsg((GsmMsg*)pMsg);

	return GSM_SUCCESS;
}


/**
 * @brief   	sendCommand()
 *
 * @details 	This function sends a command to predict and returns the
 * 				response.  The function opens a socket, connects to server
 * 				and sends a UDP message to predict server.  The function
 * 				will block waiting for the response from the server.
 *
 * @param[in]   _cmd	String which contains the command to be sent.
 * @param[out]  _rsp	Response returned by predict server
 *
 * @return      The return code whether the operation was successful or failed.
 *
 * @retval      GSM_SUCCESS		The function is successfully executed
 * @retval      GSM_FAILURE		An error occurred
 */
GsmResult_e GsmRotatorController::sendCommand(const std::string& _cmd,
											  std::string& _rsp)
{
	int valread = 0;
	int socketFd = 0;
	int bytessent = 0;
	struct sockaddr_in serv_addr;
	char rspBuf[1024];

	spdlog::info("GsmRotatorController::sendCommand: cmd={0}", _cmd);

	memset((char*)&serv_addr, 0, sizeof(struct sockaddr_in));

	// TODO: read port and IP address from configuration

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(10099);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if ((socketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		spdlog::error("GsmRotatorController::sendCommand: can't create socket!!");
		return GSM_FAILURE;
	}

	if (connect(socketFd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0)
	{
		spdlog::error("GsmRotatorController::sendCommand: connect failed, errno={0}",
					   strerror(errno));
	    close(socketFd);
		return GSM_FAILURE;
	}

   //  int numsent = send(socketFd, _command, strlen(_command), 0);
	if ((bytessent = write(socketFd, _cmd.c_str(), strlen(_cmd.c_str()))) < 0)
	{
		spdlog::error("GsmRotatorController::sendCommand: send failed, errno={0}!!",
					   strerror(errno));
	    close(socketFd);
		return GSM_FAILURE;
	}

    memset(rspBuf, 0, 1024);
    valread = read(socketFd, rspBuf, 1024);

    if (valread == -1)
    {
    	spdlog::error("GsmRotatorController::sendCommand: read failed, errno={0}!!",
    				  strerror(errno));
	    close(socketFd);
		return GSM_FAILURE;
    }

    spdlog::info("GsmRotatorController::sendCommand: rsp={0}, result={1}",
    			  rspBuf, strerror(errno));

    _rsp = rspBuf;

    // closing the connected socket
    close(socketFd);

    return GSM_SUCCESS;
}


/**
 * @brief   	sendCommandToRotctld()
 *
 * @details 	This function sends a command to rotctld and returns the
 * 				response.  The function opens a socket, connects to server
 * 				and sends a TCP message to the server.  The function
 * 				will block waiting for the response from the server.
 *
 * @param[in]   _cmd	String which contains the command to be sent.
 * @param[out]  _rsp	String which contains the response from the rotator
 *
 * @return      The return code whether the operation was successful or failed.
 *
 * @retval      GSM_SUCCESS		The function is successfully executed
 * @retval      GSM_FAILURE		An error occurred
 */
GsmResult_e GsmRotatorController::sendCommandToRotctld(const std::string& _cmd,
													   std::string& _rsp)
{
	int valread;
	int socketFd;
	struct sockaddr_in serv_addr;
	char buffer[1024];

	spdlog::info("GsmRotatorController::sendCommandToRotctld: command={0}",
			_cmd.c_str());

	memset((char *)&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(4533);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if ( socketFd < 0 )
	{
		spdlog::error("GsmRotatorController::sendCommandToRotctld: can't create socket!!");
		return GSM_FAILURE;
	}

	// Set read timeout
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	if (connect(socketFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
	{
		spdlog::error("GsmRotatorController::sendCommand: can't connect to server");
		return GSM_FAILURE;
	}

    //  int numsent = send(sock, _command, strlen(_command), 0);
	int numsent = write(socketFd, _cmd.c_str(), strlen(_cmd.c_str()));
    spdlog::info("GsmRotatorController::sendCommandToRotctld: bytes-sent={0}, code={1}",
    		numsent, strerror(errno));

    memset(buffer, 0, 1024);
    valread = read(socketFd, buffer, 1024);
    if (valread == -1)
    {
    	spdlog::error("GsmRotatorController::sendCommandToRotctld: read failed, errno={0}!!",
    				  strerror(errno));
	    close(socketFd);
		return GSM_FAILURE;
    }

    spdlog::info("GsmRotatorController::sendCommandToRotctld: response={0}, code={1}",
    		buffer, strerror(errno));

    _rsp = buffer;

    // closing the connected socket
    close(socketFd);

    return GSM_SUCCESS;
}

