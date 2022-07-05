/*
 * http_client.cpp
 *
 *  Created on: Jun 24, 2022
 *      Author: jrenkema
 */


#include <iostream>
#include <string>
#include <curl/curl.h>
#include <spdlog/spdlog.h>

#include "http_client.h"




GsmHttpClient::GsmHttpClient() {
	mTimeout = 10;
	mCurl = NULL;
}


GsmHttpClient::~GsmHttpClient() {

}



int GsmHttpClient::init(GsmHttpTransaction_t& _txn) {
	CURLcode res;

    spdlog::info("GsManagerModule::init: entered");

    if ((mCurl = curl_easy_init()) == NULL) {
        spdlog::warn("GsManagerModule::init: curl init failed!");
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

