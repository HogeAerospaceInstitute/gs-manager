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

#ifndef _GSM_GLOBALS_H_
#define _GSM_GLOBALS_H_


typedef enum GsmResult
{
	GSM_SUCCESS = 0,
	GSM_FAILURE,
	GSM_MSG_CACHED
} GsmResult_e;


typedef enum GsmFSMResult
{
	GSM_FSM_SUCCESS = 0,
	GSM_FSM_FAILURE
} GsmFSMResult_e;



/* Message Categories */
#define GSM_MSG_CAT_RESERVED				0x0000
#define GSM_MSG_CAT_COMMON					0x1000
#define GSM_MSG_CAT_GSMGR					0x2000
#define GSM_MSG_CAT_HTTPCLIENT				0x3000
#define GSM_MSG_CAT_ROTCTRL					0x4000


/* Message Types */

/* Common Messages */
#define GSM_MSG_TYPE_START 							GSM_MSG_CAT_COMMON+0x0001
#define GSM_MSG_TYPE_SHUTDOWN						GSM_MSG_CAT_COMMON+0x0002

/* Ground Station Manager Messages */
#define GSM_MSG_TYPE_GET_SATELLITE_POS_REQ  		GSM_MSG_CAT_GSMGR+0x0001
#define GSM_MSG_TYPE_GET_SATELLITE_POS_RSP  		GSM_MSG_CAT_GSMGR+0x0002
#define GSM_MSG_TYPE_GET_SATELLITE_POS_TIMEOUT		GSM_MSG_CAT_GSMGR+0x0003
#define GSM_MSG_TYPE_TRACK_SATELLITE_REQ			GSM_MSG_CAT_GSMGR+0x0004
#define GSM_MSG_TYPE_TRACK_SATELLITE_RSP			GSM_MSG_CAT_GSMGR+0x0005
#define GSM_MSG_TYPE_TRACK_SATELLITE_TIMEOUT		GSM_MSG_CAT_GSMGR+0x0006
#define GSM_MSG_TYPE_DEACTIVATE_TASK_REQ    		GSM_MSG_CAT_GSMGR+0x0007
#define GSM_MSG_TYPE_REFRESH_TASKS_REQ				GSM_MSG_CAT_GSMGR+0x0008
#define GSM_MSG_TYPE_REFRESH_TASKS_RSP				GSM_MSG_CAT_GSMGR+0x0009
#define GSM_MSG_TYPE_CLEAR_TASKS_REQ				GSM_MSG_CAT_GSMGR+0x000A
#define GSM_MSG_TYPE_ACTIVATE_TASK_REQ				GSM_MSG_CAT_GSMGR+0x000B
#define GSM_MSG_TYPE_CHECK_ROTATOR_DELAY_TIMEOUT	GSM_MSG_CAT_GSMGR+0x000C
#define GSM_MSG_TYPE_RECORDING_DELAY_TIMEOUT		GSM_MSG_CAT_GSMGR+0x000D
#define GSM_MSG_TYPE_PERIODIC_TIMEOUT				GSM_MSG_CAT_GSMGR+0x000E


/* HTTP Client Messages */


/* Rotator Controller Messages */
#define GSM_MSG_TYPE_RELOAD_PREDICT_DB_REQ		GSM_MSG_CAT_ROTCTRL+0x0001
#define GSM_MSG_TYPE_RELOAD_PREDICT_DB_RSP		GSM_MSG_CAT_ROTCTRL+0x0002
#define GSM_MSG_TYPE_RELOAD_PREDICT_DB_TIMEOUT	GSM_MSG_CAT_ROTCTRL+0x0003
#define GSM_MSG_TYPE_MOVE_ROTATOR_REQ			GSM_MSG_CAT_ROTCTRL+0x0004
#define GSM_MSG_TYPE_MOVE_ROTATOR_RSP			GSM_MSG_CAT_ROTCTRL+0x0005
#define GSM_MSG_TYPE_MOVE_ROTATOR_TIMEOUT		GSM_MSG_CAT_ROTCTRL+0x0006
#define GSM_MSG_TYPE_GET_ROTATOR_POS_REQ		GSM_MSG_CAT_ROTCTRL+0x0007
#define GSM_MSG_TYPE_GET_ROTATOR_POS_RSP		GSM_MSG_CAT_ROTCTRL+0x0008
#define GSM_MSG_TYPE_GET_ROTATOR_POS_TIMEOUT	GSM_MSG_CAT_ROTCTRL+0x0009



#endif
