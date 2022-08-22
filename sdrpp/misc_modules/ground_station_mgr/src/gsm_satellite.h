/* -----------------------------------------------------------------------------
 * Copyright (C) 2022, Hoge Aerospace Institute
 * This software is the confidential and proprietary information
 * of Hoge Aerospace Institute.
 *
 * ALL RIGHTS RESERVED
 *
 * Permission is hereby granted to licensees of Hoge Aerospace
 * Institute products to use or abstract this computer program for
 * the sole purpose of implementing a product based on Hoge
 * Aerospace Institute products.  No other rights to reproduce, use,
 * or disseminate this computer program, whether in part or in
 * whole, are granted.
 *
 * Hoge Aerospace Institute makes no representation or warranties
 * with respect to the performance of this computer program, and
 * specifically disclaims any responsibility for any damages,
 * special or consequential, connected with the use of this program.
 * -----------------------------------------------------------------------------
 */


/*
 * gsm_satellite.h
 *
 *  Created on: Jul 30, 2022
 *
 */

#ifndef _GSM_SATELLITE_H_
#define _GSM_SATELLITE_H_


class GsmSatellite {

	public:

		GsmSatellite() {
			mAzimuth = "0";
			mElevation = "0";
		}

		virtual ~GsmSatellite() {}


		void setName(const std::string& _name) {
			mName = _name;
		}

		void getName(std::string& _name) {
			_name = mName;
		}

		void setTLE(const std::string& _tle) {
			mTLE = _tle;
		}

		void getTLE(std::string& _tle) {
			_tle = mTLE;
		}

		void setAzimuth(const std::string& _azimuth) {
			mAzimuth = _azimuth;
		}

		void getAzimuth(std::string& _azimuth) {
			_azimuth = mAzimuth;
		}

		void setElevation(const std::string& _elevation) {
			mElevation = _elevation;
		}

		void getElevation(std::string& _elevation) {
			_elevation = mElevation;
		}

		void print(string& _out) {
			std::stringstream buffer;
			buffer << "Name=" << mName << "\n" << "    tle=" << mTLE;
			_out = buffer.str();
		}


	private:

		std::string mName;
		std::string mTLE;

		std::string mAzimuth;
		std::string mElevation;

};


#endif
