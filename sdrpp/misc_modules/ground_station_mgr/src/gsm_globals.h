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


// Message types
#define GSM_MSG_TYPE_START 					0x10
#define GSM_MSG_TYPE_SHUTDOWN				0x11
#define GSM_MSG_TYPE_GET_SATELLITE_POS_REQ  0x12
#define GSM_MSG_TYPE_TRACK_SATELLITE_REQ	0x13
#define GSM_MSG_TYPE_REFRESH_TASKS_REQ		0x14
#define GSM_MSG_TYPE_REFRESH_TASKS_RSP		0x15
#define GSM_MSG_TYPE_CLEAR_TASKS_REQ		0x16
#define GSM_MSG_TYPE_ACTIVATE_TASK_REQ		0x17
#define GSM_MSG_TYPE_RELOAD_PREDICT_DB_REQ	0x18
#define GSM_MSG_TYPE_GET_SATELLITE_POS_RSP  0x19
#define GSM_MSG_TYPE_DEACTIVATE_TASK_REQ    0x1A

#endif
