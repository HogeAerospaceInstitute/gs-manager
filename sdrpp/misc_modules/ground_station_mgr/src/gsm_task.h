/*
 * gsm_task.h
 *
 *  Created on: Jun 27, 2022
 *      Author: jrenkema
 */

#ifndef _GSM_TASK_H_
#define _GSM_TASK_H_

#include <string>
#include <json.hpp>


using namespace std;
using nlohmann::json;


class GsmTask
{

	public:
		GsmTask();
		virtual ~GsmTask();

		void init(json _task) {
			struct tm tm;

			mUUID = _task["UUID"];
			mTLE = _task["tle"];

			// convert start time
			string startTime = _task["start-time"];
			if (strptime(startTime.c_str(), "%d %b %Y %H:%M:%S", &tm) == NULL) {
				// failed to parse start time
			}
			else {
				mStartTime = mktime(&tm);
			}

			// convert end time
			string endTime = _task["end-time"];
			if (strptime(endTime.c_str(), "%d %b %Y %H:%M:%S", &tm) == NULL) {
				// failed to parse start time
			}
			else {
				mEndTime = mktime(&tm);
			}

			// convert string to double
			string frequency = _task["frequency"];
			mFrequency = std::stod(frequency);
		}


	private:

		string mUUID;
		string mTLE;

		time_t mStartTime;
		time_t mEndTime;

		double mFrequency;


};


#endif
