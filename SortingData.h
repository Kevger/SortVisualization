#pragma once
/**
* SortingData.h
* @author: Kevin German
**/
#include <mutex>
#include <chrono>
#include "settings.h"

class SortingData
{
private:
	mutable std::mutex mMutex;	//mutex for protecting the internal data from race conditions
	std::chrono::nanoseconds mAssignmentDelay{ defaultAssignmentDelay }; //artificial delay for assignments
	std::chrono::nanoseconds mCompareDelay{ defaultCompareDelay };	//artificial delay for comparisons
	bool mAssigned{ false }; //true if recently assigned
	bool mCompared{false};	//true if recently compared
	bool mVerificationEnabled{ false }; //used for visualizing the verification process
public:
	int mKey{0};//key used for comparisons

	/**
	* @brief constructor
	* @param key: int key for comparisons
	**/
	explicit SortingData(const int key) : mKey(key) {}

	/**
	* @brief move constructor
	* @param other: SortingData rvalue
	**/
	SortingData(SortingData&& other) noexcept;

	/**
	* @brief copy constructor
	* @param other: SortingData reference
	**/
	SortingData(const SortingData& other);


	/**
	* @brief default constructor
	**/
	SortingData() = default;

	/**
	* @brief default destructor
	**/
	~SortingData() = default;

	/**
	* @brief classic comparison operator '<'. modifies internal state of booth object (despite constness) and has a artificial delay build in (mCompareDelay)
	* @param other: reference to the object on the right side of '<'
	* @return true if this->mKey < other.mKey
	* @note modifies internal state of both objects. The fake constness is necessary for some algorithms in the stl (like std::inplace_merge)
	**/
	bool operator<(const SortingData& other) const;

	/**
	* @brief classic bitwise operator
	* @param other: int used for bitwise operation
	* @return int mKey & other
	**/
	int operator&(const int other) const;

	/**
	* @brief assignment operator '=' with artificial delay(mAssignmentDelay)
	* @param other: SortingData
	* @return reference to this object
	**/
	SortingData& operator=(const SortingData & other);

	/**
	* @brief move assignment operator '=' with artificial delay(mAssignmentDelay)
	* @param other SortingData rvalue
	* @return reference to this object
	**/
	SortingData& operator=(SortingData &&other);

	/**
	* @brief increment operator (prefix). neded for std::iota
	* @return reference to this object
	**/
	SortingData& operator++();

	/**
	* @brief check if this object was recently compared (used for visualizing comparison operations)
	* @return true if object was compared and modify state back to not compared
	**/
	bool compared();

	/**
	* @brief check if this object was recently assigned (used for visualizing assignment operations)
	* @return true if object was assigned recently and modify state back to not assigned
	**/
	bool assigned();

	/**
	* @brief enable verification mode. if enabled all calls to compared() and assigned() will not modify the state back to not compared.
	* Resets assigned and compare flag as well and increases the time needed for comparisons (looks better in visualization process)
	* @return void
	**/
	void enableVerification(const bool enable);

	/**
	* @brief check if verification mode is enabled
	* @return true if verification mode is enabled
	**/
	bool verificationEnabled() const;

	/**
	* @brief classic comparison operator '<'. modifies internal state of booth object (despite constness) and has a artificial delay build in (mCompareDelay)
	* @param compareDelay: std::chrono::nanoseconds. Sets minimum time needed for comparisions to this value
	* @param assignmentDelay: std::chrono::nanoseconds. Sets minimum time needed for assignments to this value
	**/
	void setDelay(const std::chrono::nanoseconds compareDelay, const  std::chrono::nanoseconds assignmentDelay);
};
