
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
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


int main( int argc, char* argv[] )
{
	using namespace ml::sorting;

	std::default_random_engine gen;

	std::clog << "Demo of 'Guided Merge Sort' algorithm ..." << std::endl;

	/////////////////////////////////////////////////////////////////
	// Testing
	/////////////////////////////////////////////////////////////////

	std::clog << "Testing ..." << std::endl;

	const int N = 500;         // Length of randomly generated arrays
	const int TESTS_NUM = 15;  // Number of random tests, applied
	                           // to every algorithm.

	//     std::sort
	std::clog << "\t std::sort" << std::endl;
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
	std::clog << "\t ordinary merge sort" << std::endl;
	merge_sort_strategy< _2_merge_strategy > ordinary_merge_sort_s(
			4 );  // Threshold of switching to Insertion sort
	test_sorting_manual(
			ordinary_merge_sort_s );
	test_sorting_automated( N, TESTS_NUM, gen,
			ordinary_merge_sort_s );

	//     ordinary merge sort, using guided 2-merge
	std::clog << "\t ordinary merge sort (using guided 2-merge)" << std::endl;
	merge_sort_strategy< guided_2_merge_strategy > guided_2_merge_sort_s(
			4 );  // Threshold of switching to Insertion sort
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

	// Benchmarking





	return 0;
}
