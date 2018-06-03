/**
* SortingData.cpp
* @author: Kevin German
**/
#include "SortingData.h"
#include <chrono>
#include <thread>
#include <mutex>

void SortingData::setDelay(const std::chrono::nanoseconds compareDelay,const std::chrono::nanoseconds assignmentDelay)
{
	mCompareDelay = compareDelay;
	mAssignmentDelay = assignmentDelay;
}

SortingData::SortingData(SortingData&& other) noexcept
	: mAssignmentDelay(other.mAssignmentDelay),mCompareDelay(other.mCompareDelay), mAssigned(true), mKey(other.mKey)
{
	other.mAssigned = false;
	other.mKey = 0;
	other.mCompared = false;
}

SortingData::SortingData(const SortingData& other)
	: mAssignmentDelay(other.mAssignmentDelay), mCompareDelay(other.mCompareDelay), mAssigned(true), mKey(other.mKey)
{
	std::lock_guard<std::mutex> lock(mMutex);
	std::lock_guard<std::mutex> lockOther(other.mMutex);	
	std::this_thread::sleep_for(mAssignmentDelay); //delay to simulate heavy copy work
}


bool SortingData::operator<(const SortingData& other) const
{
	{
		std::lock_guard<std::mutex> lockThis(mMutex);
		std::lock_guard<std::mutex> lockOther(other.mMutex);
		if (mVerificationEnabled)
		{
			if (mKey < other.mKey)
			{
				const_cast<SortingData*>(this)->mCompared = false;
				const_cast<SortingData*> (&other)->mCompared = false;
			}
			else
			{
				const_cast<SortingData*>(this)->mCompared = true;
				const_cast<SortingData*> (&other)->mCompared = true;
			}
		}
		else
		{
			const_cast<SortingData*>(this)->mCompared = true;
			const_cast<SortingData*> (&other)->mCompared = true;
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
	if (this != &other)
	{
		{
			std::unique_lock<std::mutex> lockThis(mMutex);
			std::unique_lock<std::mutex> lockOther(other.mMutex);
			mAssigned = true;
			mKey = other.mKey;
		}
		std::this_thread::sleep_for(mAssignmentDelay);	//delay to simulate heavy copy work
	}
	return *this;
}

SortingData& SortingData::operator=(SortingData&& other)
{
	{	
		std::unique_lock<std::mutex> lockThis(mMutex);
		mKey = other.mKey;
		mAssigned = true;
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
	const auto tmpCompared = mCompared;
	if (!mVerificationEnabled)
		mCompared = false;
	return tmpCompared;
}


bool SortingData::assigned()
{
	std::lock_guard<std::mutex> lock(mMutex);
	const auto tmpAssigned = mAssigned;
	mAssigned = false;
	return tmpAssigned;
}


void SortingData::enableVerification(const bool enable)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mAssigned = false;
	mCompared = false;
	if (enable)
	{
		mCompareDelay = mCompareDelay*5;
	}
	mVerificationEnabled = enable;
}

bool SortingData::verificationEnabled() const
{
	return mVerificationEnabled;
}
