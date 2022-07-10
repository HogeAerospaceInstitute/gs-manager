/*
 * http_client.h
 *
 *  Created on: Jun 24, 2022
 *      Author: jrenkema
 */

#ifndef _GSM_HTTP_CLIENT_H_
#define _GSM_HTTP_CLIENT_H_

#include <string>
#include <curl.h>

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
#define GSM_MAX_HTTP_DATA_SIZE		1024
    char data[GSM_MAX_HTTP_DATA_SIZE];
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


class GsmHttpClient {

	public:
		GsmHttpClient();
		virtual ~GsmHttpClient();

		int init(GsmHttpTransaction_t& _txn);

		int send();

		static size_t writeRspData(void *buffer, size_t size, size_t nmemb, void *stream);
		static const char* strerror(int errcode);

		void setTimeout(int sec) {
			mTimeout = sec;
		}


	protected:


	private:

		CURL* mCurl;
    	int mTimeout;

};


#endif
