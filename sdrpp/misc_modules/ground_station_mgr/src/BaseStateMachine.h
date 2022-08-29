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
 * BaseStateMachine.h
 *
 *  Created on: Aug 27, 2022
 *
 */


#ifndef _BASE_STATE_MACHINE_H_
#define _BASE_STATE_MACHINE_H_

#include <string>



class BaseEvent
{

	public:

		BaseEvent() { mId = 0; }
		BaseEvent( unsigned int _id, const char* _name )
							{ mId = _id; mName = _name; }
		virtual ~BaseEvent() {}

		unsigned int getId() const { return mId; }
		const std::string& getName() const { return mName; }
		const char* getNameStr() const { return mName.c_str(); }


	protected:

		unsigned int mId;
		std::string mName;

	private:

};



template<class O> class BaseFSM;

template <class O>
class BaseState
{
	public:

		BaseState( unsigned int id, const char* name) : mId(id) , mName(name) {}
		virtual ~BaseState() {}

		virtual int onEvent( O&, BaseFSM< O >&, const BaseEvent* ) = 0;
		virtual void enter(O& , BaseFSM< O >& ) {}
		virtual void exit(O& , BaseFSM<O>& ) {}

		unsigned int getId() const { return mId; }
		const std::string& getName() const { return mName; }
		const char* getNameStr() const { return mName.c_str(); }


	protected:
	private:

		unsigned int mId;
		std::string mName;

};


template<class O>
class BaseFSM
{
	protected:

		O& mOwner;
		BaseState< O >* mCurrent;
		BaseState< O >* mPrevious;

	public:
		BaseFSM( O& aOwner, BaseState< O >* initState )
				: mOwner(aOwner), mCurrent(initState), mPrevious(initState)
		{
			mCurrent->enter(aOwner, *this);
		}

		BaseFSM(O& aOwner) : mOwner(aOwner)
		{
			mCurrent = 0;
			mPrevious = 0;
		}

		void setInitState(BaseState< O >* aNew)
		{
			mCurrent = aNew;
			mPrevious = aNew;
			mCurrent->enter(mOwner, *this);
		}

		virtual ~BaseFSM()
		{
			if ( mCurrent )
			{
				mCurrent->exit(mOwner, *this);
			}
		}

		virtual int onEvent( const BaseEvent* pEvent )
		{
			if ( mCurrent == 0 ) return 0;
			return mCurrent->onEvent(mOwner, *this, pEvent );
		}

		virtual void setState( const BaseState< O >* aNew)
		{
			if (mCurrent==aNew) return;
			mPrevious = mCurrent;
			mCurrent = (BaseState< O >*)aNew;
			if ( mPrevious != 0 )
			{
				mPrevious->exit(mOwner, *this);
			}
			mCurrent->enter(mOwner, *this);
		}

		O& getOwner() const { return mOwner; }
		const BaseState< O >* getPreviousState() const { return mPrevious; }
		const BaseState< O >* getCurrentState() const { return mCurrent; }
		bool isCurrentState(BaseState< O >* state) const { return (mCurrent==state); }
		BaseFSM& operator=(const BaseFSM & fsm)
		{
			mOwner = fsm.mOwner;
			mCurrent = fsm.mCurrent;
			mPrevious = fsm.mPrevious;
		};

};


#endif
