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

		void setTaskId(const std::string& _taskId)
		{
			mTaskId = _taskId;
		}

		void getTaskId(std::string& _taskId)
		{
			_taskId = mTaskId;
		}

		void setSatellite(const std::string& _name)
		{
			mSatellite = _name;
		}

		void getSatellite(std::string& _name)
		{
			_name = mSatellite;
		}


	private:

		std::string mTaskId;
		std::string mSatellite;

};


class GsmMsgGetSatellitePosRsp : public GsmMsg
{
	public:

		GsmMsgGetSatellitePosRsp()
		{
			mResult = 0;
		}

		virtual ~GsmMsgGetSatellitePosRsp() {}

		void setTaskId(const std::string& _taskId)
		{
			mTaskId = _taskId;
		}

		void getTaskId(std::string& _taskId)
		{
			_taskId = mTaskId;
		}

		void setSatellite(const std::string& _satellite)
		{
			mSatellite = _satellite;
		}

		void getSatellite(std::string& _satellite)
		{
			_satellite = mSatellite;
		}

		void setAzimuth(const std::string& _azimuth)
		{
			mAzimuth = _azimuth;
		}

		void getAzimuth(std::string& _azimuth)
		{
			_azimuth = mAzimuth;
		}

		void setElevation(const std::string& _elevation)
		{
			mElevation = _elevation;
		}

		void getElevation(std::string& _elevation)
		{
			_elevation = mElevation;
		}


	private:

		std::string mTaskId;
		std::string mSatellite;
		std::string mAzimuth;
		std::string mElevation;
		int mResult;

};


class GsmMsgMoveRotatorReq : public GsmMsg
{
	public:

		GsmMsgMoveRotatorReq()
		{
			mElevation = "0.0";
			mAzimuth = "0.0";
		}

		virtual ~GsmMsgMoveRotatorReq() {}

		void setTaskId(const std::string& _taskId)
		{
			mTaskId = _taskId;
		}

		void getTaskId(std::string& _taskId)
		{
			_taskId = mTaskId;
		}

		void setSatellite(const std::string& _satellite)
		{
			mSatellite = _satellite;
		}

		void getSatellite(std::string& _satellite)
		{
			_satellite = mSatellite;
		}

		void setAzimuth(const std::string& _azimuth)
		{
			mAzimuth = _azimuth;
		}

		void getAzimuth(std::string& _azimuth)
		{
			_azimuth = mAzimuth;
		}

		void setElevation(const std::string& _elevation)
		{
			mElevation = _elevation;
		}

		void getElevation(std::string& _elevation)
		{
			_elevation = mElevation;
		}


	private:

		std::string mTaskId;
		std::string mSatellite;
		std::string mAzimuth;
		std::string mElevation;

};


class GsmMsgMoveRotatorRsp : public GsmMsg
{
	public:

		GsmMsgMoveRotatorRsp() {
			mResult = 0;
		}

		virtual ~GsmMsgMoveRotatorRsp() {}

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
		int mResult;

};


class GsmMsgReloadPredictDbReq : public GsmMsg
{
	public:

		GsmMsgReloadPredictDbReq() {}

		virtual ~GsmMsgReloadPredictDbReq() {}

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


class GsmMsgReloadPredictDbRsp : public GsmMsg
{
	public:

		GsmMsgReloadPredictDbRsp() {
			mResult = 0;
		}

		virtual ~GsmMsgReloadPredictDbRsp() {}

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
		int mResult;

};


class GsmMsgGetRotatorPosReq : public GsmMsg
{
	public:

		GsmMsgGetRotatorPosReq() {}

		virtual ~GsmMsgGetRotatorPosReq() {}

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


class GsmMsgGetRotatorPosRsp : public GsmMsg
{
	public:

		GsmMsgGetRotatorPosRsp()
		{
			mElevation = "0.0";
			mAzimuth = "0.0";
		}

		virtual ~GsmMsgGetRotatorPosRsp() {}

		void setTaskId(const std::string& _taskId)
		{
			mTaskId = _taskId;
		}

		void getTaskId(std::string& _taskId)
		{
			_taskId = mTaskId;
		}

		void setAzimuth(const std::string& _azimuth)
		{
			mAzimuth = _azimuth;
		}

		void getAzimuth(std::string& _azimuth)
		{
			_azimuth = mAzimuth;
		}

		void setElevation(const std::string& _elevation)
		{
			mElevation = _elevation;
		}

		void getElevation(std::string& _elevation)
		{
			_elevation = mElevation;
		}


	private:

		std::string mTaskId;
		std::string mAzimuth;
		std::string mElevation;

};


class GsmMsgTimeout : public GsmMsg
{
public:

	GsmMsgTimeout()
	{
		mTimerId = -1;
	}

	virtual ~GsmMsgTimeout() {}

	void setAppId(const std::string& _appId)
	{
		mAppId = _appId;
	}

	void getAppId(std::string& _appId) const
	{
		_appId = mAppId;
	}

	void setTimerId(int _timerId)
	{
		mTimerId = _timerId;
	}

	int getTimerId(std::string& _taskId) const
	{
		return mTimerId;
	}



private:

	int mTimerId;
	std::string mAppId;

};


class GsmMsgHttpReq : public GsmMsg
{
public:

	GsmMsgHttpReq()  {}
	virtual ~GsmMsgHttpReq() {}

	void setURL(const std::string& _url)
	{
		mURL = _url;
	}

	void getURL(std::string& _url) const
	{
		_url = mURL;
	}

	void setFile(const std::string& _file)
	{
		mFile = _file;
	}

	void getFile(std::string& _file) const
	{
		_file = mFile;
	}


private:

	std::string mURL;
	std::string mFile;

};


#endif
