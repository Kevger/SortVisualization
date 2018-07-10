/**
* SortingData.cpp
* @author: Kevin German
**/
#include "SortingData.h"
#include "sort.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <exception>

extern bool sortingDisabled;	//used for stopping sorting algorithms immediately (in case of a user event)

void SortingData::setDelay(const std::chrono::nanoseconds compareDelay,const std::chrono::nanoseconds assignmentDelay)
{
	mCompareDelay = compareDelay;
	mAssignmentDelay = assignmentDelay;
}

SortingData::SortingData(SortingData&& other) noexcept
	: mAssignmentDelay(other.mAssignmentDelay),mCompareDelay(other.mCompareDelay), mRecentlyAssigned(true), mKey(other.mKey)
{
	other.mRecentlyAssigned = false;
	other.mKey = 0;
	other.mRecentlyCompared = false;
}

SortingData::SortingData(const SortingData& other)
	: mAssignmentDelay(other.mAssignmentDelay), mCompareDelay(other.mCompareDelay), mRecentlyAssigned(true), mKey(other.mKey)
{
	std::lock(mMutex, other.mMutex);
	std::lock_guard<std::mutex> lock(mMutex,std::adopt_lock);
	std::lock_guard<std::mutex> lockOther(other.mMutex,std::adopt_lock);	
	std::this_thread::sleep_for(mAssignmentDelay); //delay to simulate heavy copy work
}


bool SortingData::operator<(const SortingData& other) const
{
	{
		std::lock(mMutex, other.mMutex);
		std::lock_guard<std::mutex> lock(mMutex, std::adopt_lock);
		std::lock_guard<std::mutex> lockOther(other.mMutex, std::adopt_lock);
		if (mVerificationEnabled)
		{
			if (mKey < other.mKey)
			{
				mRecentlyCompared = false;
				other.mRecentlyCompared = false;
			}
			else
			{
				mRecentlyCompared = true;
				other.mRecentlyCompared = true;
			}
		}
		else
		{
			mRecentlyCompared = true;
			other.mRecentlyCompared = true;
		}
	}
	std::this_thread::sleep_for(mCompareDelay);//delay to simulate heavy comparison work
	return mKey < other.mKey;
}



int SortingData::operator&(const int other) const
{
	std::lock_guard<std::mutex> lock(mMutex);
	return mKey & other;
}

SortingData& SortingData::operator=(const SortingData &other)
{
	if (sortingDisabled)
		throw std::exception("Sort interrupted by user input. ");

	if (this != &other)
	{
		{
			std::lock(mMutex, other.mMutex);
			std::lock_guard<std::mutex> lock(mMutex, std::adopt_lock);
			std::lock_guard<std::mutex> lockOther(other.mMutex, std::adopt_lock);
			mRecentlyAssigned = true;
			mKey = other.mKey;
		}
		std::this_thread::sleep_for(mAssignmentDelay);	//delay to simulate heavy copy work
	}
	return *this;
}

SortingData& SortingData::operator=(SortingData&& other)
{
	if (sortingDisabled)
		throw std::exception("Sort interrupted by user input. ");

	{	
		std::lock_guard<std::mutex> lockThis(mMutex);
		mKey = other.mKey;
		mRecentlyAssigned = true;
	}
	return *this;
}


SortingData& SortingData::operator++()
{
	std::lock_guard<std::mutex> lock(mMutex);
	++mKey;
	return *this;
}


bool SortingData::compared()
{
	std::lock_guard<std::mutex> lock(mMutex);
	const auto tmpCompared = mRecentlyCompared;
	if (!mVerificationEnabled)
		mRecentlyCompared = false;
	return tmpCompared;
}


bool SortingData::assigned()
{
	std::lock_guard<std::mutex> lock(mMutex);
	const auto tmpAssigned = mRecentlyAssigned;
	mRecentlyAssigned = false;
	return tmpAssigned;
}


void SortingData::enableVerification(const bool enable)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mRecentlyAssigned = false;
	mRecentlyCompared = false;
	if (enable)
	{
		mCompareDelay = mCompareDelay*timeForVerification;
	}
	mVerificationEnabled = enable;
}

bool SortingData::verificationEnabled() const
{
	return mVerificationEnabled;
}
