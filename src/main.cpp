
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>
#include <iomanip>
#include <cassert>

#include "../include/insertion_sort.hpp"
#include "../include/stl_sort_strategy.hpp"
#include "../include/stl_heap_sort_strategy.hpp"
#include "../include/merge.hpp"
#include "../include/guided_merge.hpp"
#include "../include/merge_sort.hpp"


/// Performs manual tests on the sorting algorithm, represented
/// as provided 'SortStrategy'.
template< typename SortStrategy >
void test_sorting_manual( SortStrategy s = SortStrategy() )
{
	std::vector< int > v;

	{
		v = { 5, 12, 19, 8, 13, 31, 24, 28 };  // random
		s( v.begin(), v.end() );
		assert( std::is_sorted( v.begin(), v.end() ) );
	}

	{
		v = { 7, 5, 3, 1 };  // reverse-sorted
		s( v.begin(), v.end() );
		assert( std::is_sorted( v.begin(), v.end() ) );
	}

	{
		v = { 4, 5, 6, 7, 8 };  // sorted
		s( v.begin(), v.end() );
		assert( std::is_sorted( v.begin(), v.end() ) );
	}

	{
		v = { 8, 6, 7, 5, 6, 4, 5, 3 };  // step-ladder downwards
		s( v.begin(), v.end() );
		assert( std::is_sorted( v.begin(), v.end() ) );
	}

	{
		v = { 1, 3, 2, 4, 3, 5, 4 };  // step-ladder upwards
		s( v.begin(), v.end() );
		assert( std::is_sorted( v.begin(), v.end() ) );
	}

	{
		v = { 8, 4 };  // short
		s( v.begin(), v.end() );
		assert( std::is_sorted( v.begin(), v.end() ) );
	}

	{
		v = { 5 };  // very short
		s( v.begin(), v.end() );
		assert( std::is_sorted( v.begin(), v.end() ) );
	}

	{
		v = { 8, 6, 6, 8, 8, 8, 6, 6, 8 };  // 2 values
		s( v.begin(), v.end() );
		assert( std::is_sorted( v.begin(), v.end() ) );
	}

	{
		v = { 19, 32, 11, 34, 19, 35, 16, 17, 3, 4, 24 };  // random
		s( v.begin(), v.end() );
		assert( std::is_sorted( v.begin(), v.end() ) );
	}

}


/// Performs automated tests on the sorting algorithm, represented
/// as provided 'SortStrategy'.
/// @param n Length of the randomly generated array, that will go
///    through sorting.
/// @param num_of_tests Number of invocations of the sorting strategy,
///    on different, randomly generated arrays.
template< typename SortStrategy, typename Gen >
void test_sorting_automated(
		int n,
		int tests_num,
		Gen& gen,
		SortStrategy s = SortStrategy() )
{
	std::vector< int > v( n );
	std::uniform_int_distribution< int > dist( 0, 1'000'000'000 );
	for ( int t = 0; t < tests_num; ++t ) {
		std::generate( v.begin(), v.end(),
				std::bind( dist, std::ref( gen ) ) );  // Generate random unsorted array
		s( v.begin(), v.end() );  // Sort it
		assert( std::is_sorted( v.begin(), v.end() ) );  // Check
	}
}


/// Runs provided sorting algorithm 's' on n-long randomly generated
/// array, for 'num_of_tests' times, and measures average running time.
/// The array being sorted is composed from large static objects.
template< typename SortStrategy, typename Gen >
void benchmark_sorting(
		int n,
		int num_of_tests,
		Gen& gen,
		const std::string& name,
		SortStrategy s = SortStrategy() )
{
	typedef std::array<
			int,
			320 > element_t;
			// Type of the elements being sorted
	typedef std::chrono::high_resolution_clock clock_t;
			// Type of the clock, used for measuring
	clock_t::duration overall_duration( 0 );
			// Overall time, spent on sorting
	std::vector< element_t > v( n );
	std::uniform_int_distribution< int > dist( 0, 1'000'000'000 );
	for ( int t = 0; t < num_of_tests; ++t ) {
		// Generate array
		for ( element_t& elem : v ) {
			elem.fill( 0 );
			elem.back() = dist( gen ); // Only last value differs
		}
		// Sort it
		clock_t::time_point start_t = clock_t::now();
		s( v.begin(), v.end() );
		clock_t::time_point finish_t = clock_t::now();
		overall_duration += (finish_t - start_t);
	}
	clock_t::duration average_duration = overall_duration / num_of_tests;
			// Calculate the average
	// Print the results
	std::clog << std::setw( 40 ) << name << ": "
			<< std::setw( 10 ) <<
			std::chrono::duration_cast< std::chrono::microseconds >(
					average_duration ).count() << " mcs" << std::endl;
}


/// Tests correctness of various sorting algorithms.
template< typename Gen >
void test_algorithms( Gen& gen )
{
	using namespace ml::sorting;

	std::clog << "Testing ..." << std::endl;

	const int N = 500;         // Length of randomly generated arrays
	const int TESTS_NUM = 15;  // Number of random tests, applied
	                           // to every algorithm.

	//     std::sort
	std::clog << "\t std::sort()" << std::endl;
	stl_sort_strategy stl_sort_s;
	test_sorting_manual(
			stl_sort_s );
	test_sorting_automated( N, TESTS_NUM, gen,
			stl_sort_s );

	//     STL heap sort implementation
	std::clog << "\t stl heap sort" << std::endl;
	stl_heap_sort_strategy stl_heap_sort_s;
	test_sorting_manual(
			stl_heap_sort_s );
	test_sorting_automated( N, TESTS_NUM, gen,
			stl_heap_sort_s );

	//     ordinary merge sort
	std::clog << "\t merge sort" << std::endl;
	merge_sort_strategy< _2_merge_strategy > ordinary_merge_sort_s(
			4 );  // Threshold of switching to Insertion sort
	test_sorting_manual(
			ordinary_merge_sort_s );
	test_sorting_automated( N, TESTS_NUM, gen,
			ordinary_merge_sort_s );

	//     ordinary merge sort, using guided 2-merge
	std::clog << "\t merge sort (with guided 2-merge)" << std::endl;
	merge_sort_strategy< guided_2_merge_strategy > guided_2_merge_sort_s(
			4 );
	test_sorting_manual(
			guided_2_merge_sort_s );
	test_sorting_automated( N, TESTS_NUM, gen,
			guided_2_merge_sort_s );

	//     3-merge sort
	std::clog << "\t 3-merge sort" << std::endl;
	_3_merge_sort_strategy< _3_merge_strategy > _3_merge_sort_s(
			9 );  // Threshold of switching to Insertion sort
	test_sorting_manual(
			_3_merge_sort_s );
	test_sorting_automated( N, TESTS_NUM, gen,
			_3_merge_sort_s );

	//     guided 3-merge sort
	std::clog << "\t guided 3-merge sort" << std::endl;
	_3_merge_sort_strategy< guided_3_merge_strategy > guided_3_merge_sort_s(
			9 );
	test_sorting_manual(
			guided_3_merge_sort_s );
	test_sorting_automated( N, TESTS_NUM, gen,
			guided_3_merge_sort_s );

	//     4-merge sort
	std::clog << "\t 4-merge sort" << std::endl;
	_4_merge_sort_strategy< _4_merge_strategy > _4_merge_sort_s(
			16 );  // Threshold of switching to Insertion sort
	test_sorting_manual(
			_4_merge_sort_s );
	test_sorting_automated( N, TESTS_NUM, gen,
			_4_merge_sort_s );

	//     guided 4-merge sort
	std::clog << "\t guided 4-merge sort" << std::endl;
	_4_merge_sort_strategy< guided_4_merge_strategy > guided_4_merge_sort_s(
			16 );
	test_sorting_manual(
			guided_4_merge_sort_s );
	test_sorting_automated( N, TESTS_NUM, gen,
			guided_4_merge_sort_s );

	std::clog << "Tests completed." << std::endl;
}


/// Tests correctness of various sorting algorithms.
template< typename Gen >
void benchmark_algorithms( Gen& gen )
{
	using namespace ml::sorting;

	std::clog << "Benchmarking ..." << std::endl;

	const int N = 100'000;            // Length of randomly generated
	                                  // arrays.
	const int RUNS_NUM = 5;           // Number of runs, performed for
	                                  // each algorithm.
	const int SWITCH_THRESHOLD = 24;  // When to switch to Insertion
	                                  // sort.

	stl_sort_strategy stl_sort_s;
	benchmark_sorting( N, RUNS_NUM, gen,
			"std::sort()", stl_sort_s );

	stl_heap_sort_strategy stl_heap_sort_s;
	benchmark_sorting( N, RUNS_NUM, gen,
			"stl heap sort", stl_heap_sort_s );

	merge_sort_strategy< _2_merge_strategy > ordinary_merge_sort_s(
			SWITCH_THRESHOLD );
	benchmark_sorting( N, RUNS_NUM, gen,
			"merge sort", ordinary_merge_sort_s );

	merge_sort_strategy< guided_2_merge_strategy > guided_2_merge_sort_s(
			SWITCH_THRESHOLD );
	benchmark_sorting( N, RUNS_NUM, gen,
			"merge sort (with guided 2-merge)", guided_2_merge_sort_s );

	_3_merge_sort_strategy< _3_merge_strategy > _3_merge_sort_s(
			SWITCH_THRESHOLD );
	benchmark_sorting( N, RUNS_NUM, gen,
			"3-merge sort", _3_merge_sort_s );

	_3_merge_sort_strategy< guided_3_merge_strategy > guided_3_merge_sort_s(
			SWITCH_THRESHOLD );
	benchmark_sorting( N, RUNS_NUM, gen,
			"guided 3-merge sort", guided_3_merge_sort_s );

	_4_merge_sort_strategy< _4_merge_strategy > _4_merge_sort_s(
			SWITCH_THRESHOLD );
	benchmark_sorting( N, RUNS_NUM, gen,
			"4-merge sort", _4_merge_sort_s );

	_4_merge_sort_strategy< guided_4_merge_strategy > guided_4_merge_sort_s(
			SWITCH_THRESHOLD );
	benchmark_sorting( N, RUNS_NUM, gen,
			"guided 4-merge sort", guided_4_merge_sort_s );

	std::clog << "Benchmarking completed." << std::endl;
}


int main( int argc, char* argv[] )
{
	std::default_random_engine gen;

	std::clog << "Demo of 'Guided Merge Sort' algorithm" << std::endl;

	test_algorithms( gen );
	benchmark_algorithms( gen );

	return 0;
}
