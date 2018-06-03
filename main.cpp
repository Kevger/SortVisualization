/**
* main.cpp
* @author: Kevin German
**/
#include <SDL.h>
#include <thread>
#include <iostream>
#include <fstream>
#include "settings.h"
#include "SortingData.h"
#include "Sorting_Algorithm_v1.h"

//-------Prototypes-------
void init_settings(const int argc, char** argv);
void close_system();
void print_controls();
void init_data(std::vector<SortingData>& data);
void keyboard_event(const SDL_KeyboardEvent* type, std::vector<SortingData>& data);
void thread_drawing(std::vector<SortingData>* data);
void thread_sorting(std::vector<SortingData>* data);
int init_system();
//------Variables---------
auto assignmentDelay = defaultAssignmentDelay;
auto compareDelay = defaultCompareDelay;
auto numberOfElements = defaultNumberOfElements;
auto currentSortingAlgorithm = sorting::SortingAlgorithm::none;
auto isRunning = true;
bool sortingDisabled = false;
//------SDL variables-----
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
auto elementWidth = defaultScreenWidth / numberOfElements;
auto screenWidth = defaultScreenWidth;
auto screenHeight = defaultScreenHeight;
auto maxThreads = defaultMaxThreads;

int main(const int argc, char** argv)
{
	init_settings(argc, argv);
	//init
	std::vector<SortingData> data(numberOfElements);
	init_data(data);
	if (init_system())
		return -1;

	print_controls();

	//threads
	std::thread drawingThread(thread_drawing, &data);
	std::thread sortingThread(thread_sorting, &data);

	//handle events
	while (isRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				keyboard_event(&event.key, data);
				break;
			case SDL_QUIT:
				sortingDisabled = true;
				isRunning = false;
			default:
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	//wait for threads
	sortingThread.join();
	drawingThread.join();

	//terminate sdl
	close_system();

	return 1;
}


void thread_drawing(std::vector<SortingData>* data)
{
	SDL_Rect rect;
	rect.y = screenHeight;
	rect.w = elementWidth;

	while (isRunning)
	{
		//clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		//draw each element
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		auto elementPos = 0;
		for (auto& element : *data)
		{
			if (!element.verificationEnabled())
			{
				if (element.compared())
					SDL_SetRenderDrawColor(renderer, 194, 24, 7, SDL_ALPHA_OPAQUE);
				else if (element.assigned())
					SDL_SetRenderDrawColor(renderer, 90, 24, 7, SDL_ALPHA_OPAQUE);
				else
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			}
			else
			{
				if (element.compared())
					SDL_SetRenderDrawColor(renderer, 0, 160, 0, SDL_ALPHA_OPAQUE);
				else
					SDL_SetRenderDrawColor(renderer, 194, 24, 7, SDL_ALPHA_OPAQUE);
			}
			rect.h = -element.mKey;
			rect.x = (elementPos++) * elementWidth;
			SDL_RenderFillRect(renderer, &rect);
		}
		SDL_RenderPresent(renderer);
		std::this_thread::yield();
	}
}

void thread_sorting(std::vector<SortingData>* data)
{
	while (isRunning)
	{
		if (currentSortingAlgorithm != sorting::SortingAlgorithm::none)
		{
			sortingDisabled = false;
			//start timer here for rough measurement 
			const auto start = std::chrono::high_resolution_clock::now();
			switch (currentSortingAlgorithm)
			{
			case sorting::SortingAlgorithm::cyclesort:
				std::cout << "Cyclesort started...\n";
				sorting::cyclesort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::bubblesort:
				std::cout << "Bubblesort started...\n";
				sorting::bubblesort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::bubblesortrc:
				std::cout << "Bubblesort recursivly started...\n";
				sorting::bubblesort_rc(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::stdsort:
				std::cout << "std::sort started...\n";
				std::sort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::shellsort:
				std::cout << "Shellsort started...\n";
				sorting::shellsort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::combsort:
				std::cout << "Combsort started...\n";
				sorting::combsort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::gnomesort:
				std::cout << "Gnomesort started...\n";
				sorting::gnomesort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::gnomesort2:
				std::cout << "Gnomesort2 started...\n";
				sorting::gnomesort2(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::radixsort:
				std::cout << "Radixsort started...\n";
				sorting::radixsort(data->begin(), data->end(), 13);
				break;
			case sorting::SortingAlgorithm::radixsortslow:
				std::cout << "Slow radixsort started...\n";
				sorting::radixsort_slow(data->begin(), data->end(), 13);
				break;
			case sorting::SortingAlgorithm::radixsortipis:
				std::cout << "Inplace Radixsort with insertionsort started...\n";
				sorting::radixsort_ip_is(data->begin(), data->end(), 13, std::less<>(), maxThreads);
				break;
			case sorting::SortingAlgorithm::insertionsort:
				std::cout << "Insertionsort started...\n";
				sorting::insertionsort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::insertionsortbinsearch:
				std::cout << "Insertionsort with binary search started...\n";
				sorting::insertionsort_binsearch(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::selectionsort:
				std::cout << "Selectionsort started...\n";
				sorting::selectionsort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::bogosort:
				std::cout << "Bogosort started...\n";
				sorting::bogosort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::bozosort:
				std::cout << "Bozosort started...\n";
				sorting::bozosort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::oddevensort:
				std::cout << "Odd-even-sort started...\n";
				sorting::odd_even_sort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::shakersort:
				std::cout << "Shakersort started...\n";
				sorting::shakersort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::quicksort:
				std::cout << "Quicksort started...\n";
				sorting::quicksort(data->begin(), data->end(), std::less<>(), maxThreads);
				break;
			case sorting::SortingAlgorithm::mergesort:
				std::cout << "Mergesort started...\n";
				sorting::mergesort(data->begin(), data->end(), std::less<>(), maxThreads);
				break;
			case sorting::SortingAlgorithm::heapsort:
				std::cout << "Heapsort started...\n";
				sorting::heapsort(data->begin(), data->end());
				break;
			case sorting::SortingAlgorithm::introsort:
				std::cout << "Introsort started...\n";
				sorting::introsort(data->begin(), data->end(), std::less<>(), maxThreads);
				break;
			case sorting::SortingAlgorithm::stdstablesort:
				std::cout << "std::stablesort started...\n";
				std::stable_sort(data->begin(), data->end());
				break;
			default:
				break;
			}
			const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
			currentSortingAlgorithm = sorting::SortingAlgorithm::none;
			sortingDisabled = false;
			std::cout << "Sort finished! Elapsed Time:\t" << elapsed.count() << " s\n";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void init_data(std::vector<SortingData>& data)
{
	//init seed & rng
	std::random_device seed;
	std::mt19937 gen(seed());
	std::uniform_int_distribution<int> dist(0, screenHeight);

	//fill with random values
	for (auto& i : data)
		i.mKey = dist(gen);

	for (auto& i : data)
	{
		i.enableVerification(false); //disable slow comparison 
		i.setDelay(compareDelay, assignmentDelay);
	}
}

void keyboard_event(const SDL_KeyboardEvent* type, std::vector<SortingData>& data)
{
	sortingDisabled = true;
	switch ((*type).keysym.sym)
	{
	case SDLK_0:
		init_data(data);
		break;
	case SDLK_1:
		currentSortingAlgorithm = sorting::SortingAlgorithm::stdsort;
		break;
	case SDLK_2:
		currentSortingAlgorithm = sorting::SortingAlgorithm::bubblesort;
		break;
	case SDLK_3:
		currentSortingAlgorithm = sorting::SortingAlgorithm::bubblesortrc;
		break;
	case SDLK_4:
		currentSortingAlgorithm = sorting::SortingAlgorithm::insertionsort;
		break;
	case SDLK_5:
		currentSortingAlgorithm = sorting::SortingAlgorithm::insertionsortbinsearch;
		break;
	case SDLK_6:
		currentSortingAlgorithm = sorting::SortingAlgorithm::selectionsort;
		break;
	case SDLK_7:
		currentSortingAlgorithm = sorting::SortingAlgorithm::gnomesort;
		break;
	case SDLK_8:
		currentSortingAlgorithm = sorting::SortingAlgorithm::gnomesort2;
		break;
	case SDLK_9:
		currentSortingAlgorithm = sorting::SortingAlgorithm::cyclesort;
		break;
	case SDLK_q:
		currentSortingAlgorithm = sorting::SortingAlgorithm::shellsort;
		break;
	case SDLK_w:
		currentSortingAlgorithm = sorting::SortingAlgorithm::combsort;
		break;
	case SDLK_e:
		currentSortingAlgorithm = sorting::SortingAlgorithm::oddevensort;
		break;
	case SDLK_r:
		currentSortingAlgorithm = sorting::SortingAlgorithm::shakersort;
		break;
	case SDLK_t:
		currentSortingAlgorithm = sorting::SortingAlgorithm::radixsort;
		break;
	case SDLK_z:
		currentSortingAlgorithm = sorting::SortingAlgorithm::radixsortslow;
		break;
	case SDLK_u:
		currentSortingAlgorithm = sorting::SortingAlgorithm::radixsortipis;
		break;
	case SDLK_i:
		currentSortingAlgorithm = sorting::SortingAlgorithm::bogosort;
		break;
	case SDLK_o:
		currentSortingAlgorithm = sorting::SortingAlgorithm::bozosort;
		break;
	case SDLK_p:
		currentSortingAlgorithm = sorting::SortingAlgorithm::quicksort;
		break;
	case SDLK_a:
		currentSortingAlgorithm = sorting::SortingAlgorithm::mergesort;
		break;
	case SDLK_s:
		currentSortingAlgorithm = sorting::SortingAlgorithm::heapsort;
		break;
	case SDLK_d:
		currentSortingAlgorithm = sorting::SortingAlgorithm::introsort;
		break;
	case SDLK_f:
		currentSortingAlgorithm = sorting::SortingAlgorithm::stdstablesort;
		break;
	case SDLK_x: //inverse order
		sorting::inverse_order(data.begin(), data.end());
		std::cout << "Order inversed!\n";
		break;
	case SDLK_v:
		//verify order
		//enable verification mode, this is used for drawing correctly sorted values in green
		for (auto& i : data)
			i.enableVerification(true);

		if (sorting::verifiy_sort_order(data.begin(), data.end()))
			std::cout << "Sorted!\n";
		else
			std::cout << "Not sorted\n";
		break;
	case SDLK_b:
		std::cout << "Max. threads increased: " << ++maxThreads << "\n";
		break;
	case SDLK_n:
		if (maxThreads != 0)
		{
			--maxThreads;
			std::cout << "Max. threads decreased: " << maxThreads << "\n";
		}
		else
		{
			std::cout << "Max. threads is already 0\n";
		}
		break;
	default:
		break;
	}
}

int init_system()
{
	//SDL System initialisieren
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		std::cout << "Error SDL_Init: " << SDL_GetError();
		return ERROR_SDL_INIT;
	}

	//fenster und renderer erstellen
	if (SDL_CreateWindowAndRenderer(screenWidth, screenHeight, 0, &window, &renderer) != 0)
	{
		std::cout << "Error SDL_CreateWindowAndRenderer: " << SDL_GetError();
		return ERROR_SDL_INIT;
	}
	return 0;
}

void close_system()
{
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_Quit();
}


void init_settings(const int argc, char** argv)
{
	std::cout << "Kevin German C++ with STL\n\n";

	auto loadFromFile = false;
	std::ifstream file;
	//try to load from specified file
	if (argc != 1)
	{
		file.open(argv[1]);
		if (!file)
		{
			std::cout << "Error: " << argv[1] << " not found!\n";
			loadFromFile = false;
		}
		else
		{
			std::cout << "Loading config from: " << argv[1] << "\n";
			loadFromFile = true;
		}
	}
	//try to load from default file
	if (!loadFromFile)
	{
		file.open(configFileName);
		//if no file was found, use user input
		if (!file)
		{
			std::cout << configFileName << " not found!\n";
			loadFromFile = false;
		}
		else
		{
			std::cout << "Loading config from default file: " << configFileName << "\n";
			loadFromFile = true;
		}
	}

	//----screenwidth----
	std::cout << "Screenwidth (default " << defaultScreenWidth << " ): ";
	if (loadFromFile)
	{
		file >> screenWidth;
		std::cout << screenWidth << "\n";
	}
	else
	{
		std::cin >> screenWidth;
	}
	if (screenWidth <= 0 || screenWidth >= 4096)
	{
		std::cout << "Invalid size. Default: " << defaultScreenWidth << " used\n";
		screenWidth = defaultScreenWidth;
	}

	//----screenheight----
	std::cout << "Screenheight (default " << defaultScreenHeight << " ): ";
	if (loadFromFile)
	{
		file >> screenHeight;
		std::cout << screenHeight << "\n";
	}
	else
	{
		std::cin >> screenHeight;
	}
	if (screenWidth <= 0 || screenWidth >= 4096)
	{
		std::cout << "Invalid size. Default: " << defaultScreenHeight << " used\n";
		screenHeight = defaultScreenHeight;
	}

	//----number of elements----
	std::cout << "Number of elements (default " << defaultScreenWidth / 2 << " ): ";
	if (loadFromFile)
	{
		file >> numberOfElements;
		std::cout << numberOfElements << "\n";
	}
	else
	{
		std::cin >> numberOfElements;
	}
	if (numberOfElements == 0 || numberOfElements > screenWidth)
	{
		std::cout << "Invalid size. Default: " << defaultScreenWidth / 2 << " used\n";
		numberOfElements = defaultScreenWidth / 2;
	}
	elementWidth = screenWidth / numberOfElements;

	//----assignmentdelay----
	std::cout << "Assignment delay (default " << defaultAssignmentDelay.count() << "ns ): ";
	auto delay = 0;
	if (loadFromFile)
	{
		file >> delay;
		std::cout << delay << "\n";
	}
	else
	{
		std::cin >> delay;
	}
	if (delay >= 0)
		assignmentDelay = std::chrono::nanoseconds(delay);
	else
		std::cout << "Invalid delay. Default: " << defaultAssignmentDelay.count() << " used\n";

	//-----comparedelay-----
	std::cout << "Comparison delay (default " << defaultCompareDelay.count() << "ns ): ";
	delay = 0;
	if (loadFromFile)
	{
		file >> delay;
		std::cout << delay << "\n";
	}
	else
	{
		std::cin >> delay;
	}
	if (delay >= 0)
		compareDelay = std::chrono::nanoseconds(delay);
	else
		std::cout << "Invalid delay. Default: " << defaultCompareDelay.count() << " used\n";
}

void print_controls()
{
	std::cout
		<< "\n---------CONTROLS---------\n"
		<< "0|random init data\n"
		<< "1|std::sort (unstoppable)\n"
		<< "2|bubblesort\n"
		<< "3|bubblesort recursivly\n"
		<< "4|insertionsort\n"
		<< "5|insertionsort with binary search\n"
		<< "6|selectionsort\n"
		<< "7|gnomesort\n"
		<< "8|gnomesort with jump(insertionsort variation)\n"
		<< "9|cyclesort\n"
		<< "Q|shellsort\n"
		<< "W|combsort\n"
		<< "E|odd-even-sort (2 threads)\n"
		<< "R|shakersort\n"
		<< "T|radixsort\n"
		<< "Z|radixsort slow (copy instead of move operations used)\n"
		<< "U|radixsort in-place & insertionsort (optional parallel)\n"
		<< "I|bogosort\n"
		<< "O|bozosort\n"
		<< "P|quicksort (optional parallel)\n"
		<< "A|mergesort (optional parallel) (unstoppable)\n"
		<< "S|heapsort (unstoppable)\n"
		<< "D|introsort (optional parallel)\n"
		<< "F|std::stablesort (unstoppable)\n"
		<< "X|reverse order\n"
		<< "V|verify order\n"
		<< "----Threads(default " << defaultMaxThreads << ")----\n"
		<< "B|increase max. threads\n"
		<< "N|decrease max. threads\n"
		<< "--------------------------\n"
		<< "  Keypress stopps sort\n"
		<< "--------------------------\n"
		<< std::endl;
}
