#pragma once
/**
* Sorting_Algorithm_v1.h
* @author: Kevin German
**/
#include <future>
#include <random>
#include <vector>
#include <algorithm>
extern bool sortingDisabled;	//used for stopping sorting algorithms immediately (in case of user event)

namespace sorting
{
	/**
	* @brief available sorting algorithms
	**/
	enum class SortingAlgorithm
	{
		none,
		cyclesort,
		bubblesort,
		bubblesortrc,
		stdsort,
		shellsort,
		combsort,
		gnomesort,
		gnomesort2,
		radixsort,
		radixsortslow,
		radixsortipis,
		bogosort,
		bozosort,
		selectionsort,
		insertionsort,
		insertionsortbinsearch,
		oddevensort,
		shakersort,
		quicksort,
		mergesort,
		heapsort,
		introsort,
		stdstablesort
	};


	/**
	* @brief check container is sorted
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return true if container is sorted, else false
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	bool verifiy_sort_order(I begin, I end, U cmp = U())
	{
		if (begin == end)
			return true;

		for (I last = begin++; begin != end; ++begin, ++last)
			if (cmp(*begin, *last))
				return false;
		return true;
	}


	/**
	* @brief check container is sorted recursively
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return true if container is sorted, else false
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	bool verifiy_sort_order_rc(I begin, I end, U cmp = U())
	{
		I last = begin;
		if (begin == end || ++begin == end) //tail call
			return true;
		if (cmp(*last, *begin))
			return verifiy_sort_order_rc(begin, end, cmp);
		return false;
	}


	/**
	* @brief inverse order of container
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @return void
	**/
	template <typename I>
	void inverse_order(I begin, I end)
	{
		using std::swap;

		for (; begin != end && begin != --end; ++begin)
			swap(*begin, *end);
	}

	/**
	* @brief binary search template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param value: searched value
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return true if value was found, else false
	**/
	template <typename I, typename V = typename std::iterator_traits<I>::value_type , typename U = std::less<typename std::iterator_traits<I>::value_type> >
	bool binary_search(I begin, I end, V &value, U cmp = U())
	{
		begin = std::lower_bound(begin, end, value, cmp);
		return (begin != end && *begin == value);
	}


	/**
	* @brief bubblesort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void bubblesort(I begin, I end, U cmp = U())
	{
		using std::swap;
		for (auto swapped = true; swapped && (begin != end) && !sortingDisabled; --end)
		{
			swapped = false;
			for (I i = begin, next = std::next(begin); next != end && !sortingDisabled; ++i, ++next)
			{
				if (cmp(*next, *i))
				{
					swap(*i, *next);
					swapped = true;
				}
			}
		}
	}


	template <typename I, typename U>
	bool _bubblesort_rc(const I firstBegin, const I begin, I end, U cmp)
	{
		using std::swap;
		if (begin == end || sortingDisabled)
			return true;

		const I next = std::next(begin);

		if (next == end) //tail call
			return true;

		auto sorted = true;
		if (cmp(*next, *begin)) //sort 2 elements
		{
			swap(*begin, *next);
			sorted = false;
		}
		sorted &= _bubblesort_rc(firstBegin, std::next(begin), end, cmp);
		//1 rc bubblesort run to the last element, will return false if 1 element is not sorted
		if (begin == firstBegin && !sorted)
			//if first call and array is not sorted, repeat process, but ignore last element
			_bubblesort_rc(firstBegin, firstBegin, --end, cmp);
		return sorted;
	}

	/**
	* @brief recursive bubblesort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void bubblesort_rc(I begin, I end, U cmp = U())
	{
		_bubblesort_rc(begin, begin, end, cmp);
	}


	/**
	* @brief combsort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void combsort(I begin, I end, U cmp = U())
	{
		using std::swap;

		if (begin == end)
			return;
		auto sorted = false;
		const int length = std::distance(begin, end);
		auto gap = length;

		while (!sorted && !sortingDisabled)
		{
			if (gap > 1)
			{
				sorted = false;
				gap = (gap * 10) / 13;
			}
			else
			{
				gap = 1;
				sorted = true;
			}

			auto u = 0;
			for (I i = begin; (u < length - gap) && !sortingDisabled; ++i, ++u)
			{
				I next = std::next(i, gap);
				if (cmp(*next, *i))
				{
					swap(*i, *next);
					sorted = false;
				}
			}
		}
	}


	/**
	* @brief shakersort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void shakersort(I begin, I end, U cmp = U())
	{
		using std::swap;
	
		if (begin == end)
			return;
		--end;
		while (std::distance(begin, end) > 0 && !sortingDisabled)
		{
			for (I i = begin, next = std::next(begin); i != end && !sortingDisabled; ++i, ++next) //from left to right
				if (cmp(*next, *i))
					swap(*i, *next);
			--end;

			for (I i = end, prev = end; i != begin && !sortingDisabled; --i) //from right to left
				if (cmp(*i, *(--prev)))
					swap(*prev, *i);
			++begin;
		}
	}


	/**
	* @brief insertionsort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void insertionsort(I begin, I end, U cmp = U())
	{
		if (begin == end)
			return;

		for (I next = std::next(begin), prev; next != end && !sortingDisabled; ++next)
		{
			auto value = *next;
			auto itToBegin = false;

			for (prev = std::prev(next); cmp(value, *prev) && !sortingDisabled; --prev)
			{
				*std::next(prev) = *prev;
				if (prev == begin)
				{
					itToBegin = true;
					break;
				}
			}
			if (!itToBegin)
				++prev;
			*prev = value;
		}
	}


	/**
	* @brief insertionsort with binary search template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::remove_reference<decltype(*std::declval<I>())>::type> >	//this works for stl but not std::less<decltype(*std::declval<I>())> >  -_-
	void insertionsort_binsearch(I begin, I end, U cmp = U())
	{
		//shorter and much nicer version of insertionsort but this is not my work (cppreference) :(
		//uses binary search
		for (I next = std::next(begin); next != end && !sortingDisabled; ++next)
			std::rotate(std::upper_bound(begin, next, *next,cmp), next, std::next(next));
	}



	/**
	* @brief shellsort
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void shellsort(I begin, I end, U cmp = U())
	{
		const int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1}; //gapsequence -> A102549 Ciuras sequence
		const int num_of_gaps = sizeof(gaps) / sizeof(gaps[0]);
		const auto size = std::distance(begin, end);

		for (auto g = 0; g < num_of_gaps && !sortingDisabled; ++g) //go through all gaps
		{
			//insertionsort with gap as distance instead of 1
			for (auto gap = gaps[g], j = gap; j < size && !sortingDisabled; j += gap)
			{
				auto next_Element = std::move(*std::next(begin, j));
				auto i = j - gap;

				while (i >= 0 && cmp(next_Element, *std::next(begin, i)))
				{
					*std::next(begin, i + gap) = std::move(*std::next(begin, i));
					i -= gap;
				}
				*std::next(begin, i + gap) = next_Element;
			}
		}
	}


	/**
	* @brief selectionsort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void selectionsort(I begin, I end, U cmp = U())
	{
		using std::swap;

		if (begin == end)
			return;

		for (; begin != end && !sortingDisabled; ++begin)
		{
			I min = begin;
			for (I next = std::next(begin); next != end && !sortingDisabled; ++next)
				if (cmp(*next, *min))
					min = next;
			swap(*begin, *min);
		}
	}


	/**
	* @brief odd-even-sort template (parallel)
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void odd_even_sort(I begin, I end, U cmp = U())
	{
		using std::swap;

		const int dist = std::distance(begin, end);
		if (dist <= 1)
			return;

		bool isOdd = dist % 2;
		auto evenSortedCtr = 4, oddSortedCtr = 4;
		auto sorted = false;

		auto partSort = [&](bool isEvenSort, int *sortedCtr)
		{
			auto fixedEnd = std::prev(end, isEvenSort ? isOdd : !isOdd);
			auto fixedBegin = std::next(begin, isEvenSort ? 0 : 1);

			for (auto partSorted = true; !sorted; partSorted = true)
			{
				for (I i = fixedBegin; i != fixedEnd; i += 2) 
				{
					if (cmp(*std::next(i), *i))
					{
						swap(*i, *std::next(i));
						*sortedCtr = 4;
						partSorted = false;
					}
				}
				if (partSorted)
					--*sortedCtr;
			}
		};

		std::future<void> feven = std::async(partSort, true, &evenSortedCtr );
		std::future<void> fodd = std::async(partSort, false, &oddSortedCtr);

		while((oddSortedCtr > 0 || evenSortedCtr > 0) && !sortingDisabled)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(2000));
		}

		sorted = true;
		feven.wait();
		fodd.wait();
	}


	/**
	* @brief cyclesort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void cyclesort(I begin, I end, U cmp = U())
	{
		using std::swap;

		if (begin == end)
			return;

		I new_pos = std::next(begin);
		for (I start = begin; start != end && !sortingDisabled; ++start)
		{
			I old_pos = start;
			auto current_element = *start;
			while (start != new_pos && !sortingDisabled) //complete a full cycle
			{
				new_pos = start;

				for (I j = std::next(start); j != end && !sortingDisabled; ++j) //calculate correct position 
					if (cmp(*j, current_element))
						++new_pos;

				if (new_pos == old_pos) //check if element is already in correct position
					break;

				while (!cmp(*new_pos, current_element) && !cmp(current_element, *new_pos)) //position right after duplicates
					++new_pos;

				old_pos = new_pos;
				swap(*new_pos, current_element); //swap to correct position, use now old element in array for comparison
			}
		}
	}


	/**
	* @brief gnomesort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void gnomesort(I begin, I end, U cmp = U())
	{
		using std::swap;

		if (begin == end)
			return;
		const I start = begin;
		while (begin != std::prev(end) && !sortingDisabled)
		{
			if (cmp(*std::next(begin), *begin))
			{
				swap(*begin, *std::next(begin));
				start != begin ? --begin : ++begin;
			}
			else
			{
				++begin;
			}
		}
	}


	/**
	* @brief gnomesort template with jump to last corrrect position (Insertionsort variation)
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void gnomesort2(I begin, I end, U cmp = U())
	{
		using std::swap;

		if (begin == end)
			return;
		const I start = begin;
		I last = start; //this gnomesort will return to the last position
		while (begin != std::prev(end) && !sortingDisabled)
		{
			if (cmp(*std::next(begin), *begin))
			{
				swap(*begin, *std::next(begin));
				start != begin ? --begin : ++begin;
			}
			else
			{
				last = begin = std::next(last);
			}
		}
	}


	/**
	* @brief bogosort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void bogosort(I begin, I end, U cmp = U())
	{
		using std::swap;

		std::random_device seed;
		std::mt19937 gen(seed());
		std::uniform_int_distribution<int> dist(0, std::distance(begin, end) - 1);
		while (!verifiy_sort_order(begin, end, cmp) && !sortingDisabled)
			for (I i = begin; i != end; ++i)
				swap(*i, *std::next(begin, dist(gen)));
	}


	/**
	* @brief bozosort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void bozosort(I begin, I end, U cmp = U())
	{
		using std::swap;

		std::random_device seed;
		std::mt19937 gen(seed());
		std::uniform_int_distribution<int> dist(0, std::distance(begin, end) - 1);
		while (!verifiy_sort_order(begin, end, cmp) && !sortingDisabled)
			swap(*std::next(begin, dist(gen)), *std::next(begin, dist(gen)));
	}


	/**
	* @brief radixsort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param bits: (optional) number of bits used for sorting. default: 32
	* @return void
	* @note bitwise '&' operator needs to be defined
	**/
	template <typename I>
	void radixsort(I begin, I end,const int bits = 32)
	{
		std::vector<typename std::iterator_traits<I>::value_type> buckets[2];
		const size_t size = std::distance(begin, end);
		buckets[0].resize(size);
		buckets[1].resize(size);
		int top[2] = {0, 0}; //index of next top element in bucket0 and bucket1

		for (auto b = 0; b < bits && !sortingDisabled; ++b)
		{
			for (auto i = begin; i != end; ++i) //sort into buckets
			{
				if (*i & (1 << b))
					buckets[1][top[1]++] = std::move(*i); //next MSB is 1
				else
					buckets[0][top[0]++] = std::move(*i); //next MSB is 0
			}

			auto j = begin;
			for (auto x = 0; x < top[0]; ++x, ++j)
				*j = std::move(buckets[0][x]);
			for (auto x = 0; x < top[1]; ++x, ++j)
				*j = std::move(buckets[1][x]);
			top[0] = top[1] = 0;
		}
	}

	/**
	* @brief radixsort template without move operations (expensive copies are used) this 
	* visualizes the sorting process better and illustrates the performance difference between copy & move
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param bits: (optional) number of bits used for sorting. default: 32
	* @return void
	* @note bitwise '&' operator needs to be defined
	**/
	template <typename I>
	void radixsort_slow(I begin, I end,const int bits = 32)
	{
		std::vector<typename std::iterator_traits<I>::value_type> buckets[2];
		const size_t size = std::distance(begin, end);
		buckets[0].resize(size);
		buckets[1].resize(size);
		int top[2] = { 0, 0 }; //index of next top element in bucket0 and bucket1

		for (auto b = 0; b < bits && !sortingDisabled; ++b)
		{
			for (auto i = begin; i != end && !sortingDisabled; ++i) //sort into buckets
			{
				if (*i & (1 << b))
					buckets[1][top[1]++] = *i; //next MSB is 1
				else
					buckets[0][top[0]++] = *i; //next MSB is 0
			}

			auto j = begin;
			for (auto x = 0; x < top[0]; ++x, ++j)
				*j = buckets[0][x];
			for (auto x = 0; x < top[1]; ++x, ++j)
				*j = buckets[1][x];
			top[0] = top[1] = 0;
		}
	}


	/**
	* @brief radixsort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param bits: (optional) number of bits used for sorting. default: 32
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @param maxThreads: (optional) max. number of threads which can run this algorithm parallel. default: 0
	* @return void
	* @note bitwise '&' operator needs to be defined
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void radixsort_ip_is(I begin, I end,int bits = 32, U cmp = U(), int maxThreads = 0)
	{
		using std::swap;

		if (std::distance(begin, end) <= 1)
			return;
		--bits;
		auto lb = begin; //left index
		auto rb = std::prev(end); //right index

		while (lb != rb && !sortingDisabled)
		{
			if (*lb & (1 << bits)) //move all 1's to the right and 0's to the left
			{
				swap(*lb, *rb);
				--rb; //rb moves to next element, because digit of current element is definitly '1' 
			}
			else
			{
				++lb; //digit of current element is 0, move to next element with lb
			}
		}

		if (*lb & (1 << bits) && lb != begin) //lb needs to be placed on the last 0, for splitting the array
			--lb;

		if (bits != 0)
		{
			if (std::distance(begin, end) < 20) //switch to insertionsort if size gets smaller than 20
			{
				insertionsort(begin, std::next(lb),cmp);
				insertionsort(std::next(lb), end,cmp);
			}
			else
			{
				std::future<void> f1, f2;
				//rec. call on array with leading 0's of current call
				if (lb != begin)
				{
					if (maxThreads > 0)
						f1 = std::async([&]() { radixsort_ip_is(begin, std::next(lb), bits, cmp,--maxThreads); });
					else
						radixsort_ip_is(begin, std::next(lb), bits, cmp, 0);
				}

				//rec. call array with leading 1's
				if (rb != std::prev(end))
				{
					if (maxThreads > 0)
						f2 = std::async([&]() { radixsort_ip_is(std::next(lb), end, bits, cmp, --maxThreads); });
					else
						radixsort_ip_is(std::next(lb), end, bits, cmp, 0);
				}

				//wait for finish if started
				if (f1.valid())
					f1.wait();
				if (f2.valid())
					f2.wait();
			}
		}
	}


	/**
	* @brief partition template. Moves all elements smaller than the pivot to the left and everything else to the right
	* @param begin: iterator to the begin of the container
	* @param pivot: iterator to the end of the container which contains the pivot element
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return Iterator to the correct position of the pivot element
	**/
	template <typename I, typename U>
	I partition(I begin, I pivot, U cmp)
	{
		using std::swap;

		I pivPos = begin;
		while (begin != pivot && !sortingDisabled)
		{
			if (cmp(*begin, *pivot))
			{
				swap(*begin, *pivPos);
				++pivPos;
			}
			++begin;
		}
		//last position of element which is not < as the pivot
		return pivPos;
	}


	/**
	* @brief median of three template. returns the b for a < b < c
	* @param first: iterator to the first element
	* @param second: iterator to the second element
	* @param third: iterator to the third element
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return iterator reference tob for a < b < c
	**/
	template <typename I, typename U = std::less <typename std::iterator_traits<I>::value_type> >
	I median_of_three(const I &first,const I &second,const I &third, U cmp = U())
	{
		return std::max(std::min(first, second,cmp), std::min(std::max(first, second, cmp), third, cmp), cmp);
	}


	/**
	* @brief quicksort template (with optional multithreading)
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @param maxThreads: number of max concurrent threads. default = 0 (no multithreading)
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void quicksort(I begin, I end, U cmp = U(), const int maxThreads = 0)
	{
		using std::swap;

		const auto dist = std::distance(begin, end);
		if (dist < 2 || sortingDisabled) //tail call
			return;

		//median of three
		I pivot = std::prev(end);
		swap(*median_of_three(begin, std::next(begin, dist / 2), std::prev(end), cmp), *pivot);

		//partition
		I pivPos = sorting::partition(begin, pivot, cmp);
		//move pivot to correct position
		swap(*pivPos, *pivot);
		if (maxThreads > 1)
		{
			std::future<void> f1 = std::async([&]() {quicksort(begin, pivPos, cmp,maxThreads - 2); });
			std::future<void> f2 = std::async([&]() {quicksort(std::next(pivPos), end, cmp,maxThreads - 2); });
			f1.wait();
			f2.wait();
		}
		else
		{
			quicksort(begin, pivPos, cmp,0);
			quicksort(std::next(pivPos), end, cmp,0);
		}
	}

	/**
	* @brief heapsort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void heapsort(I begin, I end, U cmp = U())
	{
		std::make_heap(begin, end, cmp);
		std::sort_heap(begin, end, cmp);
	}



	/**
	* @brief in place merge template which degrades the runtime of mergesort because of my poor implementation 
	* (std::inplace_merge(begin, mid,end) is much better) 
	* @param begin: iterator to the begin of the container
	* @param mid: iterator to the mid of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type>>
	void inplace_merge(I begin, I mid, I end, U cmp = U())
	{
		I &leftList = begin;
		I &rightList = mid;

		while (leftList != mid && rightList != end && !sortingDisabled)
		{
			if (cmp(*rightList,*leftList))
			{
				auto tmp = std::move(*leftList);
				*leftList = std::move(*rightList);
				//move all elements to the left, until correct position is found
				auto i = std::next(rightList);
				for(;i != end && cmp(*i,tmp);++i)
				{
					*std::prev(i) = std::move(*i);
				}
				//place the old value from left list in correct position of rightlist
				*--i = std::move(tmp);
			}
			else
			{
				++leftList;
			}
		}
	}

	/**
	* @brief mergesort template (optional multithreading)
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @param maxThreads: number of max concurrent threads. default = 0 (no multithreading)
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >	//this works but not std::less<decltype(*std::declval<I>())> >  -_-
	void mergesort(I begin, I end, U cmp = U(), int maxThreads = 0)
	{
		const auto dist = std::distance(begin, end);
		//tail call
		if (dist < 2 || sortingDisabled)
			return;
		//split
		I mid = std::next(begin, dist / 2);
		if (maxThreads > 1)
		{
			const std::future<void> f1 = std::async([&]() {mergesort(begin, mid,cmp, maxThreads - 2); });
			const std::future<void> f2 = std::async([&]() {mergesort(mid, end, cmp, maxThreads - 2); });
			f1.wait();
			f2.wait();
		}
		else
		{
			mergesort(begin, mid, cmp,0);
			mergesort(mid, end, cmp,0);
		}
		//merge
		std::inplace_merge(begin, mid,end,cmp);
		//inplace_merge(begin,mid,end,cmp);	//slow, because of the shifts required
	}



	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type>>
	void _introsort(I begin, I end, U cmp = U(),const int maxDepth = 8,const int maxThreads = 0)
	{
		using std::swap;

		const auto dist = std::distance(begin, end);
		if (dist < 2 || sortingDisabled) //tail call
			return;

		if (maxDepth == 0)
		{
			heapsort(begin, end, cmp);
			return;
		}

		//median of three
		I pivot = std::prev(end);
		swap(*median_of_three(begin, std::next(begin, dist / 2), std::prev(end), cmp), *pivot);

		//partition
		I pivPos = sorting::partition(begin, pivot, cmp);
		//move pivot to correct position
		swap(*pivPos, *pivot);
		
		if (maxThreads > 1)
		{
			std::future<void> f1 = std::async([&]() {_introsort(begin, pivPos, cmp, maxDepth - 1,maxThreads - 2); });
			std::future<void> f2 = std::async([&]() {_introsort(std::next(pivPos), end, cmp, maxDepth - 1,maxThreads - 2); });
			f1.wait();
			f2.wait();
		}
		else
		{
			_introsort(begin, pivPos, cmp, maxDepth - 1, 0);
			_introsort(std::next(pivPos), end, cmp, maxDepth - 1, 0);
		}

	
	}

	/**
	* @brief introsort template
	* @param begin: iterator to the begin of the container
	* @param end: iterator to the end of the container
	* @param cmp: (optional) compare function (bool cmp(const X &a,const X &b);) which returns true if a < b.
	* @param maxThreads: (optional) max. number of threads which can run this algorithm parallel. default: 0
	* @return void
	**/
	template <typename I, typename U = std::less<typename std::iterator_traits<I>::value_type> >
	void introsort(I begin, I end, U cmp = U(),int maxThreads = 0)
	{
		const auto dist = std::distance(begin, end);
		const auto maxDepth = static_cast<int>(std::log2(static_cast<double>(dist)));
		_introsort(begin, end, cmp, maxDepth,maxThreads);
	}
}
