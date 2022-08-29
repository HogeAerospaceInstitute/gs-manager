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
 * gsm_msg.h
 *
 *  Created on: Jul 30, 2022
 *
 */

#ifndef _GSM_MSG_H_
#define _GSM_MSG_H_

#include <stdio.h>
#include <cstring>
#include <string>


#define GSM_MAX_MSG_DATA_SIZE		4096


// Defines inter-thread message

class GsmMsg {

public:

	typedef enum {
		GSM_MSG_CAT_UNDEFINED = 0,
		GSM_MSG_CAT_APP = 1,
		GSM_MSG_CAT_SYS = 2,
		GMS_MSG_CAT_COMM = 3
	} GsmMsgCategory_e;

	GsmMsg()
	{
		mType = 0;
		mCategory = 0;
		memset(mData, 0, GSM_MAX_MSG_DATA_SIZE);
	}

	virtual ~GsmMsg() {}


	int getType() const { return mType; }
	int getCategory() { return mCategory; }
	const char* getDestination() { return mDestination.c_str(); }

	void setDestination(const char* _dest) {
		mDestination = _dest;
	}

	void setType(int _type) {
		mType = _type;
	}

	void setCategory(int _category) {
		mCategory = _category;
	}

	void setData(char* _data, int _len) {
		if (_len > GSM_MAX_MSG_DATA_SIZE) {
			_len = GSM_MAX_MSG_DATA_SIZE;
		}

		strncpy(mData, _data, GSM_MAX_MSG_DATA_SIZE);
		mData[GSM_MAX_MSG_DATA_SIZE] = '\0';
	}

	char* getData() {
		return mData;
	}


private:

	int mType;
	int mCategory;

	std::string mDestination;

	char mData[GSM_MAX_MSG_DATA_SIZE+1];

};



class GsmMsgActivateTaskReq : public GsmMsg
{
	public:

		GsmMsgActivateTaskReq() {}
		virtual ~GsmMsgActivateTaskReq() {}


		void setTaskId(const std::string& _taskId)
		{
			mTaskId = _taskId;
		}

		void getTaskId(std::string& _taskId)
		{
			_taskId = mTaskId;
		}


	private:

		std::string mTaskId;

};


class GsmMsgDeactivateTaskReq : public GsmMsg
{
	public:

		GsmMsgDeactivateTaskReq() {}
		virtual ~GsmMsgDeactivateTaskReq() {}


		void setTaskId(const std::string& _taskId)
		{
			mTaskId = _taskId;
		}

		void getTaskId(std::string& _taskId)
		{
			_taskId = mTaskId;
		}


	private:

		std::string mTaskId;

};


class GsmMsgTrackSatelliteReq : public GsmMsg
{
	public:

		GsmMsgTrackSatelliteReq() {}
		virtual ~GsmMsgTrackSatelliteReq() {}


		void setSatelliteName(const std::string& _name)
		{
			mSatelliteName = _name;
		}

		void getSatelliteName(std::string& _name)
		{
			_name = mSatelliteName;
		}


	private:

		std::string mSatelliteName;

};


class GsmMsgTrackSatelliteRsp : public GsmMsg
{
	public:

		GsmMsgTrackSatelliteRsp() {}
		virtual ~GsmMsgTrackSatelliteRsp() {}


		void setSatelliteName(const std::string& _name)
		{
			mSatelliteName = _name;
		}

		void getSatelliteName(std::string& _name)
		{
			_name = mSatelliteName;
		}

		void setResult(const std::string& _result)
		{
			mResult = _result;
		}

		void getResult(std::string& _result)
		{
			_result = mResult;
		}


	private:

		std::string mSatelliteName;
		std::string mResult;

};


class GsmMsgGetSatellitePosReq : public GsmMsg
{
	public:

		GsmMsgGetSatellitePosReq() {}
		virtual ~GsmMsgGetSatellitePosReq() {}


		void setSatelliteName(const std::string& _name)
		{
			mSatelliteName = _name;
		}

		void getSatelliteName(std::string& _name)
		{
			_name = mSatelliteName;
		}


	private:

		std::string mSatelliteName;

};


class GsmMsgGetSatellitePosRsp : public GsmMsg
{
	public:

		GsmMsgGetSatellitePosRsp() {}
		virtual ~GsmMsgGetSatellitePosRsp() {}


		void setSatelliteName(const std::string& _name)
		{
			mSatelliteName = _name;
		}

		void getSatelliteName(std::string& _name)
		{
			_name = mSatelliteName;
		}


	private:

		std::string mSatelliteName;

};


#endif
