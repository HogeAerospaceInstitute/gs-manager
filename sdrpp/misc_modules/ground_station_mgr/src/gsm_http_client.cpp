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
 * gsm_http_client.cpp
 *
 *  Created on: Jun 24, 2022
 *
 */


#include <iostream>
#include <string>
#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include <config.h>

#include "gsm_http_client.h"
#include "gsm_comm_mgr.h"



GsmHttpClient* GsmHttpClient::mInstance = NULL;

//extern ConfigManager gConfig;


GsmHttpClient* GsmHttpClient::getInstance()
{
	if ( mInstance == NULL )
	{
		mInstance = new GsmHttpClient();
	}

	return mInstance;
}


GsmHttpClient::GsmHttpClient() {
	mTimeout = 10;
	mCurl = NULL;
	mName = "HTTPCLIENT";
	mHttpTxn.status = GSM_HTTP_TRANSACTION_STATE_NULL;
	memset(&mHttpTxn.rsp.data, 0, GSM_MAX_HTTP_DATA_SIZE);
}


GsmHttpClient::~GsmHttpClient() {

}


GsmResult_e GsmHttpClient::onMessage(GsmMsg* _msg)
{
	GsmResult_e result = GSM_SUCCESS;

	spdlog::info("GsmHttpClient::onMessage: entered...");

	switch (_msg->getType())
	{
		case GSM_MSG_TYPE_REFRESH_TASKS_REQ:
		{
			result = handleRefreshTasksReq(_msg);
			break;
		}
		case GSM_MSG_TYPE_UPLOAD_FILE_REQ:
		{
			result = handleUploadFileReq(dynamic_cast<GsmMsgHttpReq*>(_msg));
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


GsmResult_e GsmHttpClient::onStart()
{
	spdlog::info("GsmHttpClient::onStart: entered...");

	// TODO establish connection to predict
	return GSM_SUCCESS;
}


GsmResult_e GsmHttpClient::onShutdown()
{
	spdlog::info("GsmHttpClient::onShutdown: entered...");
	return GSM_SUCCESS;
}


GsmResult_e GsmHttpClient::handleRefreshTasksReq(GsmMsg* _msg)
{

	spdlog::info("GsmHttpClient::handleRefreshTasksReq: entering...");

	// "https://bowshock.onrender.com/api/ground_stations/ae78f216-d97a-4612-8f06-ab14d2dbc22a/tasks");

 	// Fill in HTTP transaction message
	mHttpTxn.req.method = "GET";
 	std::string webServer = gConfig.conf["web-server"];
 	std::string groundStationId = gConfig.conf["ground-station-id"];
 	mHttpTxn.req.url = "https://" + webServer + "/api/v1/ground_stations/" + groundStationId + "/tasks";

 	init(mHttpTxn);
 	send();

 	mHttpTxn.status = GSM_HTTP_TRANSACTION_STATE_COMPLETE;

 	// respond back to GsmMgr thread
	GsmMsg* pMsg = new GsmMsg();
	pMsg->setDestination("GSMGR");
	pMsg->setType(GSM_MSG_TYPE_REFRESH_TASKS_RSP);
	pMsg->setCategory(GsmMsg::GSM_MSG_CAT_APP);
	pMsg->setData(mHttpTxn.rsp.data, strlen(mHttpTxn.rsp.data));

	GsmCommMgr::getInstance()->sendMsg(pMsg);

	// clear data
	memset(&mHttpTxn.rsp.data, 0, GSM_MAX_HTTP_DATA_SIZE);

 	spdlog::info("GsmHttpClient::handleRefreshTasksReq: exiting...");
 	return GSM_SUCCESS;
 }


int GsmHttpClient::init(GsmHttpTransaction_t& _txn)
{
	// CURLcode res;

    spdlog::info("GsmHttpClient::init: entered");

    if ((mCurl = curl_easy_init()) == NULL) {
        spdlog::warn("GsmHttpClient::init: curl init failed!");
        return -1;
    }

    // TODO: read web-server address from config
    curl_easy_setopt(mCurl, CURLOPT_URL, _txn.req.url.c_str());
    curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, writeRspData);
    curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, _txn.rsp.data);
    curl_easy_setopt(mCurl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(mCurl, CURLOPT_TIMEOUT, 30L);

	return 0;
}


int GsmHttpClient::send() {
	CURLcode res;

	spdlog::info("GsmHttpClient::send: entered");

    res = curl_easy_perform(mCurl);

    spdlog::info("GsmHttpClient::send: curl-code={0}", curl_easy_strerror(res));

    curl_easy_cleanup(mCurl);

    return 0;
}



size_t GsmHttpClient::writeRspData(void* contents, size_t size, size_t nmemb, void* userbuf) {

	spdlog::info("GsmHttpClient::writeRspData: entered");

	size_t realsize = size * nmemb;

	if (realsize > GSM_MAX_HTTP_DATA_SIZE) {
		spdlog::warn("GsmHttpClient::writeRspData: received data size={0} too large for buffer", realsize);
		return (size_t)0;
	}

	if (userbuf == NULL) {
		spdlog::warn("GsmHttpClient::writeRspData: target buffer is NULL!");
		return (size_t)0;
	}

	char* tmpbuf = (char*)userbuf;
	int length = strlen(tmpbuf);

	spdlog::info("GsmHttpClient::writeRspData: current-length={0}", length);

	strncpy(tmpbuf+length, (char*)contents, GSM_MAX_HTTP_DATA_SIZE-length);
	tmpbuf[GSM_MAX_HTTP_DATA_SIZE] = '\0';

	// print the buffer
	string s = "";
	s = tmpbuf;

	spdlog::info("GsmHttpClient::writeRspData: size={0}, data={1}", realsize, s.c_str());
	return realsize;
}


GsmResult_e GsmHttpClient::handleUploadFileReq(GsmMsgHttpReq* _msg)
{
	CURLcode res;
	std::string file;
	std::string url;
	struct stat file_info;
//	curl_off_t speed_upload, total_time;
	FILE *fd;

	_msg->getFile(file);

	spdlog::info("GsmHttpClient::handleUploadFileReq: file={0}", file.c_str());

	fd = fopen(file.c_str(), "rb");
	if (!fd)
	{
        spdlog::warn("GsmHttpClient::handleUploadFileReq: failed to open file!");
	    return GSM_FAILURE;
	}

	/* to get the file size */
	if (fstat(fileno(fd), &file_info) != 0)
	{
        spdlog::warn("GsmHttpClient::handleUploadFileReq: fstat failed!");
		return GSM_FAILURE;
	}

	mCurl = curl_easy_init();
	if (mCurl == NULL)
	{
        spdlog::warn("GsmHttpClient::handleUploadFileReq: curl init failed!");
		return GSM_FAILURE;
	}

 	std::string webServer = gConfig.conf["web-server"];
 	std::string groundStationId = gConfig.conf["ground-station-id"];
 	url = "https://" + webServer + "/api/v1/ground_stations/" + groundStationId + "/tasks";

	/* upload to this place */
	curl_easy_setopt(mCurl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(mCurl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(mCurl, CURLOPT_READDATA, fd);
    curl_easy_setopt(mCurl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
	curl_easy_setopt(mCurl, CURLOPT_VERBOSE, 1L);

	res = curl_easy_perform(mCurl);
	if (res != CURLE_OK)
	{
	     fprintf(stderr, "curl_easy_perform() failed: %s\n",
	              curl_easy_strerror(res));
	}
	else
	{
#if 0
	      /* now extract transfer info */
	      curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD_T, &speed_upload);
	      curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &total_time);

	      fprintf(stderr, "Speed: %lu bytes/sec during %lu.%06lu seconds\n",
	              (unsigned long)speed_upload,
	              (unsigned long)(total_time / 1000000),
	              (unsigned long)(total_time % 1000000));
#endif
	}

	/* always cleanup */
	curl_easy_cleanup(mCurl);
	fclose(fd);

	// TODO send response back

	return GSM_SUCCESS;
}

