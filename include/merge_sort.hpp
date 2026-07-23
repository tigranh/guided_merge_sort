
#ifndef ML__SORTING__MERGE_SORT_HPP
#define ML__SORTING__MERGE_SORT_HPP

#include <iterator>
#include <vector>
#include <functional>
#include <cassert>

#include "insertion_sort.hpp"
#include "merge.hpp"
#include "stl_merge_strategy.hpp"

namespace ml::sorting {


///
/// 2-merge sort
/// 


/// Implies Merge sort, and writes n-long sorted sequence starting 
/// from 'data_it', using already allocated auxiliary array, which 
/// starts from 'aux_it'.
/// @param data_in_buffer Indicates if the actual data to be sorted 
/// 	resides currently in the buffer 'data_it' (true), or in the 
/// 	auxiliary array 'aux_it' (false).
/// @param comp The comparator object, used to compare two values.
/// @param merge_s The Strategy, used to merge two sorted sequences.
/// @param switch_threshold The maximal length of sub-array, 
/// 	sorting which switches from Merge sort to Insertion sort 
/// 	algorithm.
template< typename DataIt, 
		typename AuxIt, 
		typename Comp, 
		typename MergeStrategy >
inline void merge_sort( DataIt data_it, unsigned int n, 
		AuxIt aux_it, bool data_in_buffer, 
		const Comp& comp, const MergeStrategy& merge_s, 
		unsigned int switch_threshold )
{
	if ( n <= switch_threshold && data_in_buffer ) {
		insertion_sort( data_it, std::next(data_it, n), comp );
		return;
	}
	// Divide the range
	assert( n >= 2 );
	const unsigned int half = n / 2;
	const DataIt data_mid = std::next(data_it, half);
	const AuxIt aux_mid = std::next(aux_it, half);
	// Prepare sorted halves in the auxiliary array
	merge_sort( aux_it, half, 
			data_it, ! data_in_buffer, 
			comp, merge_s, switch_threshold );
	merge_sort( aux_mid, n - half, 
			data_mid, ! data_in_buffer, 
			comp, merge_s, switch_threshold );
	// Merge them back into data array
	merge_s( aux_it, aux_mid, 
			aux_mid, std::next(aux_it, n), 
			data_it, comp );
}


/// The default comparator object for merge sort algorithm.
typedef std::less<> default_merge_sort_comp;

/// The default merging routine for merge sort.
typedef _2_merge_strategy default_merge_strategy;

/// Default threshold for switching to Insertion sort, during 
/// merge sort algorithm.
static constexpr unsigned int default_merge_sort_threshold = 16;


/// Performs Merge sort on the sequence [it_begin, it_end).
/// When it comes to merging two sorted sequences, 'MergeStrategy'
///     is being used then.
/// Two elements a' and 'b' are compared using 'comp', to 
/// 	understand if which one should be place to the left of the 
/// 	other one.
/// If the range being sorted becomes less than 'switch_threshold', 
/// 	the algorithm switches to Insertion sort.
template< typename MergeStrategy, 
		typename It,
		typename Comp >
inline void merge_sort( It it_begin, It it_end, 
		const Comp& comp,
		const MergeStrategy& merge_s,
		unsigned int switch_threshold )
{
	typedef typename std::iterator_traits< It >::value_type 
			value_type;
			// Type of the values being sorted
	const unsigned int n = (unsigned int)std::distance( it_begin, it_end );
			// Length of the array
	std::vector< value_type > aux( n );
			// Prepare the auxiliary array
	merge_sort( 
			it_begin, n, 
			aux.data(), true,
			comp, merge_s, switch_threshold );
}

/// Overload for 'merge_sort()', with some arguments brought to 
/// their default values.
template< typename It, typename Comp >
inline void merge_sort( It it_begin, It it_end, Comp comp )
{
	merge_sort( it_begin, it_end, 
			comp,
			default_merge_strategy(),
			default_merge_sort_threshold );
}

/// Overload for 'merge_sort()', with more arguments brought to 
/// their default values.
template< typename It >
inline void merge_sort( It it_begin, It it_end )
{
	merge_sort( it_begin, it_end, 
			default_merge_sort_comp() );
}


/// Encapsulation of 'merge_sort()' algorithm into a functional object.
template< typename MergeStrategy = stl_merge_strategy >
struct merge_sort_strategy
{
	/// The length of array, at which we switch to Insertion sort
	/// algorithm.
	unsigned int _switch_threshold;

	/// [Default] constructor
	explicit merge_sort_strategy( 
			unsigned int switch_threshold_ = default_merge_sort_threshold )
		: _switch_threshold( switch_threshold_ )
		{}

	template< typename It, typename Comp >
	void operator()( It begin, It end, Comp comp, 
			MergeStrategy merge_s, unsigned int switch_threshold ) const
		{ merge_sort( begin, end, comp, 
				merge_s, switch_threshold ); }

	template< typename It, typename Comp >
	void operator()( It begin, It end, Comp comp ) const
		{ merge_sort( begin, end, comp, 
				MergeStrategy(), 
				_switch_threshold ); }

	template< typename It >
	void operator()( It begin, It end ) const
		{ merge_sort( begin, end, 
				default_merge_sort_comp(),
				MergeStrategy(), 
				_switch_threshold ); }
};


///
/// 3-merge sort
/// 


/// Implies 3-merge sort, and writes n-long sorted sequence starting 
/// from 'data_it', using already allocated auxiliary array, which 
/// starts from 'aux_it'.
/// @param data_in_buffer Indicates if the actual data to be sorted 
/// 	resides currently in the buffer 'data_it' (true), or in the 
/// 	auxiliary array 'aux_it' (false).
/// @param comp The comparator object, used to compare two values.
/// @param merge_s The Strategy, used to merge three sorted sequences.
/// @param switch_threshold The maximal length of sub-array, 
/// 	sorting which switches from Merge sort to Insertion sort 
/// 	algorithm.
template< typename DataIt, 
		typename AuxIt, 
		typename Comp, 
		typename _3MergeStrategy >
inline void _3_merge_sort( DataIt data_it, unsigned int n, 
		AuxIt aux_it, bool data_in_buffer, 
		const Comp& comp, const _3MergeStrategy& _3_merge_s, 
		unsigned int switch_threshold )
{
	if ( n <= switch_threshold && data_in_buffer ) {
		insertion_sort( data_it, std::next(data_it, n), comp );
		return;
	}
	// Divide the range
	assert( n >= 3 );
	const unsigned int third = n / 3;
	const DataIt data_mid_1 = std::next(data_it, third);
	const DataIt data_mid_2 = std::next(data_mid_1, third);
	const AuxIt aux_mid_1 = std::next(aux_it, third);
	const AuxIt aux_mid_2 = std::next(aux_mid_1, third);
	// Prepare sorted thirds in the auxiliary array
	_3_merge_sort( aux_it, third, 
			data_it, ! data_in_buffer, 
			comp, _3_merge_s, switch_threshold );
	_3_merge_sort( aux_mid_1, third, 
			data_mid_1, ! data_in_buffer, 
			comp, _3_merge_s, switch_threshold );
	_3_merge_sort( aux_mid_2, n - 2*third, 
			data_mid_2, ! data_in_buffer, 
			comp, _3_merge_s, switch_threshold );
	// Merge them back into data array
	_3_merge_s( aux_it, aux_mid_1, 
			aux_mid_1, aux_mid_2,
			aux_mid_2, std::next(aux_mid_2, n - 2*third), 
			data_it, comp );
}


/// Performs 3-merge sort on the sequence [it_begin, it_end).
/// When it comes to merging three sorted sequences, '_3MergeStrategy'
///     is being used then.
/// Two elements a' and 'b' are compared using 'comp', to 
/// 	understand if which one should be place to the left of the 
/// 	other one.
/// If the range being sorted becomes less than 'switch_threshold', 
/// 	the algorithm switches to Insertion sort.
template< typename _3MergeStrategy, 
		typename It,
		typename Comp >
inline void _3_merge_sort( It it_begin, It it_end, 
		const Comp& comp,
		const _3MergeStrategy& _3_merge_s,
		unsigned int switch_threshold )
{
	typedef typename std::iterator_traits< It >::value_type 
			value_type;
			// Type of the values being sorted
	const unsigned int n = (unsigned int)std::distance( it_begin, it_end );
			// Length of the array
	std::vector< value_type > aux( n );
			// Prepare the auxiliary array
	_3_merge_sort( 
			it_begin, n, 
			aux.data(), true,
			comp, _3_merge_s, switch_threshold );
}


/// The default comparator object for 3-merge sort algorithm.
typedef std::less<> default_3_merge_sort_comp;

/// The default merging routine for 3-merge sort.
typedef _3_merge_strategy default_3_merge_strategy;

/// Default threshold for switching ro Insertion sort, during 
/// 3-merge sort algorithm.
static constexpr unsigned int default_3_merge_sort_threshold = 16;


/// Overload for '_3_merge_sort()', with some arguments brought to 
/// their default values.
template< typename It, typename Comp >
inline void _3_merge_sort( It it_begin, It it_end, Comp comp )
{
	_3_merge_sort( it_begin, it_end, 
			comp,
			default_3_merge_strategy(),
			default_3_merge_sort_threshold );
}

/// Overload for '_3_merge_sort()', with more arguments brought to 
/// their default values.
template< typename It >
inline void _3_merge_sort( It it_begin, It it_end )
{
	_3_merge_sort( it_begin, it_end, 
			default_3_merge_sort_comp() );
}


/// Encapsulation of '_3_merge_sort()' algorithm into a functional object.
template< typename _3MergeStrategy = default_3_merge_strategy >
struct _3_merge_sort_strategy
{
	/// The length of sub-array, at which the algorithm switches 
	/// to Insertion sort.
	unsigned int _switch_threshold;

	/// [Default] constructor
	explicit _3_merge_sort_strategy( 
			unsigned int switch_threshold_ = default_3_merge_sort_threshold )
		: _switch_threshold( switch_threshold_ )
		{}

	template< typename It, typename Comp >
	void operator()( It begin, It end, Comp comp, 
			_3MergeStrategy _3_merge_s, unsigned int switch_threshold ) const
		{ _3_merge_sort( begin, end, comp, 
				_3_merge_s, switch_threshold ); }

	template< typename It, typename Comp >
	void operator()( It begin, It end, Comp comp ) const
		{ _3_merge_sort( begin, end, comp, 
				_3MergeStrategy(), 
				_switch_threshold ); }

	template< typename It >
	void operator()( It begin, It end ) const
		{ _3_merge_sort( begin, end, 
				default_3_merge_sort_comp(),
				_3MergeStrategy(), 
				_switch_threshold ); }
};


///
/// 4-merge sort
/// 


/// Implies 4-merge sort, and writes n-long sorted sequence starting 
/// from 'data_it', using already allocated auxiliary array, which 
/// starts from 'aux_it'.
/// @param data_in_buffer Indicates if the actual data to be sorted 
/// 	resides currently in the buffer 'data_it' (true), or in the 
/// 	auxiliary array 'aux_it' (false).
/// @param comp The comparator object, used to compare two values.
/// @param merge_s The Strategy, used to merge four sorted sequences.
/// @param switch_threshold The maximal length of sub-array, 
/// 	sorting which switches from Merge sort to Insertion sort 
/// 	algorithm.
template< typename DataIt, 
		typename AuxIt, 
		typename Comp, 
		typename _4MergeStrategy >
inline void _4_merge_sort( DataIt data_it, unsigned int n, 
		AuxIt aux_it, bool data_in_buffer, 
		const Comp& comp, const _4MergeStrategy& _4_merge_s, 
		unsigned int switch_threshold )
{
	if ( n <= switch_threshold && data_in_buffer ) {
		insertion_sort( data_it, std::next(data_it, n), comp );
		return;
	}
	// Divide the range
	assert( n >= 4 );
	const unsigned int quarter = n / 4;
	const DataIt data_mid_1 = std::next(data_it, quarter);
	const DataIt data_mid_2 = std::next(data_mid_1, quarter);
	const DataIt data_mid_3 = std::next(data_mid_2, quarter);
	const AuxIt aux_mid_1 = std::next(aux_it, quarter);
	const AuxIt aux_mid_2 = std::next(aux_mid_1, quarter);
	const AuxIt aux_mid_3 = std::next(aux_mid_2, quarter);
	// Prepare sorted quarters in the auxiliary array
	_4_merge_sort( aux_it, quarter, 
			data_it, ! data_in_buffer, 
			comp, _4_merge_s, switch_threshold );
	_4_merge_sort( aux_mid_1, quarter,
			data_mid_1, ! data_in_buffer, 
			comp, _4_merge_s, switch_threshold );
	_4_merge_sort( aux_mid_2, quarter, 
			data_mid_2, ! data_in_buffer, 
			comp, _4_merge_s, switch_threshold );
	_4_merge_sort( aux_mid_3, n - 3*quarter, 
			data_mid_3, ! data_in_buffer, 
			comp, _4_merge_s, switch_threshold );
	// Merge them back into data array
	_4_merge_s( aux_it, aux_mid_1, 
			aux_mid_1, aux_mid_2,
			aux_mid_2, aux_mid_3, 
			aux_mid_3, std::next(aux_mid_3, n - 3*quarter), 
			data_it, comp );
}


/// Performs 4-merge sort on the sequence [it_begin, it_end).
/// When it comes to merging four sorted sequences, '_4MergeStrategy'
///     is being used then.
/// Two elements a' and 'b' are compared using 'comp', to 
/// 	understand if which one should be place to the left of the 
/// 	other one.
/// If the range being sorted becomes less than 'switch_threshold', 
/// 	the algorithm switches to Insertion sort.
template< typename _4MergeStrategy, 
		typename It,
		typename Comp >
inline void _4_merge_sort( It it_begin, It it_end, 
		const Comp& comp,
		const _4MergeStrategy& _4_merge_s,
		unsigned int switch_threshold )
{
	typedef typename std::iterator_traits< It >::value_type 
			value_type;
			// Type of the values being sorted
	const unsigned int n = (unsigned int)std::distance( it_begin, it_end );
			// Length of the array
	std::vector< value_type > aux( n );
			// Prepare the auxiliary array
	_4_merge_sort( 
			it_begin, n, 
			aux.data(), true,
			comp, _4_merge_s, switch_threshold );
}


/// The default comparator object for 4-merge sort algorithm.
typedef std::less<> default_4_merge_sort_comp;

/// The default merging routine for 4-merge sort.
typedef _4_merge_strategy default_4_merge_strategy;

/// Default threshold for switching to Insertion sort, during 
/// 4-merge sort algorithm.
static constexpr unsigned int default_4_merge_sort_threshold = 32;


/// Overload for '_4_merge_sort()', with some arguments brought to 
/// their default values.
template< typename It, typename Comp >
inline void _4_merge_sort( It it_begin, It it_end, Comp comp )
{
	_4_merge_sort( it_begin, it_end, 
			comp,
			default_4_merge_strategy(),
			default_4_merge_sort_threshold );
}

/// Overload for '_4_merge_sort()', with more arguments brought to 
/// their default values.
template< typename It >
inline void _4_merge_sort( It it_begin, It it_end )
{
	_4_merge_sort( it_begin, it_end, 
			default_4_merge_sort_comp() );
}


/// Encapsulation of '_4_merge_sort()' algorithm into a functional object.
template< typename _4MergeStrategy = default_4_merge_strategy >
struct _4_merge_sort_strategy
{
	/// The length of sub-array, at which the algorithm switches 
	/// to Insertion sort.
	unsigned int _switch_threshold;

	/// [Default] constructor
	explicit _4_merge_sort_strategy( 
			unsigned int switch_threshold_ = default_4_merge_sort_threshold )
		: _switch_threshold( switch_threshold_ )
		{}

	template< typename It, typename Comp >
	void operator()( It begin, It end, Comp comp, 
			_4MergeStrategy _4_merge_s, unsigned int switch_threshold ) const
		{ _4_merge_sort( begin, end, comp, 
				_4_merge_s, switch_threshold ); }

	template< typename It, typename Comp >
	void operator()( It begin, It end, Comp comp ) const
		{ _4_merge_sort( begin, end, comp, 
				_4MergeStrategy(), 
				_switch_threshold ); }

	template< typename It >
	void operator()( It begin, It end ) const
		{ _4_merge_sort( begin, end, 
				default_4_merge_sort_comp(),
				_4MergeStrategy(), 
				_switch_threshold ); }
};


} // namespace ml::sorting

#endif // ML__SORTING__MERGE_SORT_HPP
