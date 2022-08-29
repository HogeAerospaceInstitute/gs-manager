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
 * gsm_http_client.h
 *
 *  Created on: Jun 24, 2022
 *
 */

#ifndef _GSM_HTTP_CLIENT_H_
#define _GSM_HTTP_CLIENT_H_

#include <string>
#include <curl.h>

#include "gsm_globals.h"
#include "gsm_msg.h"
#include "gsm_comm_consumer.h"


using namespace std;

typedef struct GsmHttpRequest {

	//request line
	string		method;
	string		url;
	string		version;

	//headers
//	KeyValue	headers;

	//body
//	ContentType     contenType;
	string          text;
//	KeyValue        kvs;//QUERY_STRING,X_WWW_FORM_URLENCODED
} GsmHttpRequest_t;


typedef struct GsmHttpResponse {
   //status line
    string version;
    int    status_code;
    string status_message;

   //headers
//    KeyValue headers;

   //body
#define GSM_MAX_HTTP_DATA_SIZE		4096
    char data[GSM_MAX_HTTP_DATA_SIZE+1];
} GsmHttpResponse_t;


typedef enum {
	GSM_HTTP_TRANSACTION_STATE_NULL = 0,
	GSM_HTTP_TRANSACTION_STATE_INIT = 1,
	GSM_HTTP_TRANSACTION_STATE_WF_RESPONSE = 2,
	GSM_HTTP_TRANSACTION_STATE_COMPLETE = 3
} GsmTransactionState_e;


typedef struct GsmHttpTransaction {
	GsmHttpRequest req;
	GsmHttpResponse rsp;
	GsmTransactionState_e status;
} GsmHttpTransaction_t;


extern ConfigManager gConfig;


class GsmHttpClient : public GsmCommConsumer
{

	public:

		static GsmHttpClient* getInstance();

		virtual ~GsmHttpClient();


		GsmResult_e onMessage(GsmMsg* _msg);
		GsmResult_e onStart();
		GsmResult_e onShutdown();

		GsmResult_e handleRefreshTasksReq(GsmMsg* _msg);


	public:

		int init(GsmHttpTransaction_t& _txn);

		int send();

		static size_t writeRspData(void *buffer, size_t size, size_t nmemb, void *stream);
		static const char* strerror(int errcode);

		void setTimeout(int sec) {
			mTimeout = sec;
		}


	protected:


	private:

		static GsmHttpClient* mInstance;

		GsmHttpClient();


		CURL* mCurl;
    	int mTimeout;

    	GsmHttpTransaction_t mHttpTxn;


};


#endif
