
#ifndef ML__SORTING__MERGE_HPP
#define ML__SORTING__MERGE_HPP

#include <functional>
#include <algorithm>
#include <utility>

namespace ml::sorting {


/// Compares the head values of given 2 sequence [a_it, a_end) and 
/// [b_it, b_end) and writes the smaller value into 'out_it'.
#define MERGE_COMPARE_2( a_it, a_end, b_it, b_end, out_it, comp ) \
		if ( comp( *(a_it), *(b_it) ) ) { \
			*((out_it)++) = std::move( *((a_it)++) ); \
			if ( (a_it) == (a_end) ) \
				break; \
		} \
		else { \
			*((out_it)++) = std::move( *((b_it)++) ); \
			if ( (b_it) == (b_end) ) \
				break; \
		}


/// Compares the head values of given 3 sequences [a_it, a_end), 
/// [b_it, b_end) and [c_it, c_end), and writes the smaller value 
/// into 'out_it'.
#define MERGE_COMPARE_3( a_it, a_end, b_it, b_end, c_it, c_end, out_it, comp ) \
		if ( comp( *(a_it), *(b_it) ) ) { \
			MERGE_COMPARE_2( a_it, a_end, c_it, c_end, out_it, comp ); \
		} \
		else { \
			MERGE_COMPARE_2( b_it, b_end, c_it, c_end, out_it, comp ); \
		}


/// Merges two sorted sequences [it_1, end_1) and [it_2, end_2) 
/// into one, and writes it starting from 'out'.
/// Returns the advanced output iterator.
/// 
/// Note: this implementation turned out identical to the one 
/// of standard library.
template< typename InIt1, typename InIt2, 
		typename OutIt, 
		typename Comp >
inline OutIt _2_merge(
		InIt1 it_1, InIt1 end_1, 
		InIt2 it_2, InIt2 end_2, 
		OutIt out, 
		Comp comp )
{
	using std::move;
	if ( it_1 != end_1 && it_2 != end_2 ) {
		// There are two sequences to merge
		while ( true ) {
			if ( comp( *it_1, *it_2 ) ) {  // "*it_1 < *it_2"
				*(out++) = move( *(it_1++) );
				if ( it_1 == end_1 )
					break;  // First sequence is exhausted
			}
			else {
				*(out++) = move( *(it_2++) );
				if ( it_2 == end_2 )
					break;  // Second sequence is exhausted
			}
			// OR
			// MERGE_COMPARE_2( it_1, end_1, it_2, end_2, out, comp );
		}
	}
	// Copy the remaining sequence to output
	if ( it_1 != end_1 )
		out = std::copy( it_1, end_1, out );
	else
		out = std::copy( it_2, end_2, out );
	return out;
}


/// The overload, which invokes "a < b" for comparison.
template< typename InIt1, typename InIt2, 
		typename OutIt >
inline OutIt _2_merge(
		InIt1 it_1, InIt1 end_1, 
		InIt2 it_2, InIt2 end_2, 
		OutIt out )
{
	return _2_merge( it_1, end_1, it_2, end_2, out, std::less<>() );
}


/// Encapsulation of '_2_merge' algorithm into a functional object.
struct _2_merge_strategy
{
	template< typename InIt1, typename InIt2, typename OutIt, 
			typename Comp >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, 
			OutIt out, Comp comp ) const
		{ return _2_merge( it_1, end_1, it_2, end_2, 
				out, comp ); }

	template< typename InIt1, typename InIt2, typename OutIt >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, 
			OutIt out ) const
		{ return _2_merge( it_1, end_1, it_2, end_2, out ); }
};


/// Merges three sorted sequences [a_it, a_end), [b_it, b_end), and 
/// [c_it, c_end) into one, and writes it starting from 'out'.
/// Returns the advanced output iterator.
template< typename InIt1, typename InIt2, typename InIt3, 
		typename OutIt, 
		typename Comp >
inline OutIt _3_merge(
		InIt1 a_it, InIt1 a_end, 
		InIt2 b_it, InIt2 b_end, 
		InIt3 c_it, InIt3 c_end, 
		OutIt out, 
		Comp comp )
{
	using std::move;
	if ( a_it != a_end && b_it != b_end && c_it != c_end ) {
		// There are three sequences to merge
		while ( true ) {
			if ( comp( *a_it, *b_it ) ) {  // "*a_it < *b_it"
				if ( comp( *a_it, *c_it ) ) {  // "*a_it < *c_it"
					*(out++) = move( *(a_it++) );
					if ( a_it == a_end )
						break;  // First sequence is exhausted
				}
				else {  // "*c_it < *a_it"
					*(out++) = move( *(c_it++) );
					if ( c_it == c_end )
						break;  // Third sequence is exhausted
				}
			}
			else {  // "*b_it < *a_it"
				if ( comp( *b_it, *c_it ) ) {  // "*b_it < *c_it"
					*(out++) = move( *(b_it++) );
					if ( b_it == b_end )
						break;  // Second sequence is exhausted
				}
				else {  // "*c_it < *b_it"
					*(out++) = move( *(c_it++) );
					if ( c_it == c_end )
						break;  // Third sequence is exhausted
				}
			}
			// or
			// MERGE_COMPARE_3( a_it, a_end, b_it, b_end, c_it, c_end, out, comp );
		}
	}
	// Merge the remaining two sequences to the output
	if ( a_it == a_end )
		out = _2_merge( b_it, b_end, c_it, c_end, out, comp );
	else if ( b_it == b_end )
		out = _2_merge( a_it, a_end, c_it, c_end, out, comp );
	else
		out = _2_merge( a_it, a_end, b_it, b_end, out, comp );
	return out;
}


/// The overload, where comparison of values is performed as "a < b".
template< typename InIt1, typename InIt2, typename InIt3, 
		typename OutIt >
inline OutIt _3_merge(
		InIt1 a_it, InIt1 a_end, 
		InIt2 b_it, InIt2 b_end, 
		InIt3 c_it, InIt3 c_end, 
		OutIt out )
{
	return _3_merge( a_it, a_end, b_it, b_end, c_it, c_end, 
			out, std::less<>() );
}


/// Encapsulation of '_3_merge' algorithm into a functional object.
struct _3_merge_strategy
{
	template< typename InIt1, typename InIt2, typename InIt3, 
			typename OutIt, typename Comp >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, InIt3 it_3, InIt3 end_3, 
			OutIt out, Comp comp ) const
		{ return _3_merge( it_1, end_1, it_2, end_2, it_3, end_3, 
				out, comp ); }

	template< typename InIt1, typename InIt2, typename InIt3, 
			typename OutIt >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, InIt3 it_3, InIt3 end_3, 
			OutIt out ) const
		{ return _3_merge( it_1, end_1, it_2, end_2, it_3, end_3, out ); }
};


/// Merges four sorted sequences [a_it, a_end), [b_it, b_end), 
/// [c_it, c_end) and [d_it, d_end) into one, and writes it 
/// starting from 'out'.
/// Returns the advanced output iterator.
template< typename InIt1, typename InIt2, typename InIt3, 
		typename InIt4, typename OutIt, 
		typename Comp >
inline OutIt _4_merge(
		InIt1 a_it, InIt1 a_end, 
		InIt2 b_it, InIt2 b_end, 
		InIt3 c_it, InIt3 c_end, 
		InIt4 d_it, InIt4 d_end, 
		OutIt out, 
		Comp comp )
{
	if ( a_it != a_end && b_it != b_end 
			&& c_it != c_end && d_it != d_end ) {
		// There are four sequences to merge
		while ( true ) {
			if ( comp( *a_it, *b_it ) ) {  // "a < b"
				MERGE_COMPARE_3( a_it, a_end, c_it, c_end, d_it, d_end, 
						out, comp );
			}
			else {  // "b < a"
				MERGE_COMPARE_3( b_it, b_end, c_it, c_end, d_it, d_end, 
						out, comp );
			}
		}
	}
	// Merge the remaining two sequences to the output
	if ( a_it == a_end )
		out = _3_merge( b_it, b_end, c_it, c_end, d_it, d_end, out, comp );
	else if ( b_it == b_end )
		out = _3_merge( a_it, a_end, c_it, c_end, d_it, d_end, out, comp );
	else if ( c_it == c_end )
		out = _3_merge( a_it, a_end, b_it, b_end, d_it, d_end, out, comp );
	else
		out = _3_merge( a_it, a_end, b_it, b_end, c_it, c_end, out, comp );
	return out;
}

/// The overload, where comparison of values is performed as "a < b".
template< typename InIt1, typename InIt2, typename InIt3, 
		typename InIt4, typename OutIt >
inline OutIt _4_merge(
		InIt1 a_it, InIt1 a_end, 
		InIt2 b_it, InIt2 b_end, 
		InIt3 c_it, InIt3 c_end, 
		InIt3 d_it, InIt3 d_end, 
		OutIt out )
{
	return _4_merge( a_it, a_end, b_it, b_end, c_it, c_end, 
			d_it, d_end, out, std::less<>() );
}


/// Encapsulation of '_4_merge' algorithm into a functional object.
struct _4_merge_strategy
{
	template< typename InIt1, typename InIt2, typename InIt3, 
			typename InIt4, typename OutIt, typename Comp >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, InIt3 it_3, InIt3 end_3, 
			InIt4 it_4, InIt4 end_4, OutIt out, Comp comp ) const
		{ return _4_merge( it_1, end_1, it_2, end_2, it_3, end_3, 
				it_4, end_4, out, comp ); }

	template< typename InIt1, typename InIt2, typename InIt3, 
			typename InIt4, typename OutIt >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, InIt3 it_3, InIt3 end_3, 
			InIt4 it_4, InIt4 end_4, OutIt out ) const
		{ return _4_merge( it_1, end_1, it_2, end_2, it_3, end_3, 
				it_4, end_4, out ); }
};


} // namespace ml::sorting

#endif // ML__SORTING__MERGE_HPP
