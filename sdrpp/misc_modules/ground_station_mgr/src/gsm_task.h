/*
 * gsm_task.h
 *
 *  Created on: Jun 27, 2022
 *      Author: jrenkema
 */

#ifndef _GSM_TASK_H_
#define _GSM_TASK_H_

#include <string.h>

using namespace std;


class GsmTask
{

	public:
		GsmTask();
		virtual ~GsmTask();



	private:

		string mUUID;
		string mStart;
		string mEnd;
		string mFrequency;
		string mTLE;

};


#endif
