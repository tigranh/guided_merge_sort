
#ifndef ML__SORTING__INSERTION_SORT_HPP
#define ML__SORTING__INSERTION_SORT_HPP

#include <iterator>
#include <functional>

namespace ml::sorting {


/// Performs Insertion sort on the range [it_begin, it_end), using 
/// provided comparator 'comp'.
template< typename It, typename Comp >
inline void insertion_sort( It it_begin, It it_end, Comp comp )
{
	// Type of the values being sorted
	typedef typename std::iterator_traits< It >::value_type 
			value_type;
	if ( it_begin == it_end )
		return;  // Nothing to do for empty array
	for ( It it_mid = std::next( it_begin ); 
			it_mid != it_end; 
			++it_mid ) {
		// Remember '*id_mid'
		const value_type hold = std::move( *it_mid );
		// Shift rightwards necessary elements
		It it_curr = it_mid;
		It it_prev = std::prev( it_curr );
		while ( ! comp( *it_prev, hold ) ) {
			*it_curr = std::move( *it_prev );
			it_curr = it_prev;
			if ( it_curr == it_begin )
				break;
			--it_prev;
		}
		// Place back the rememberd value
		*it_curr = std::move( hold );
	}
}


/// The same algorithm, where comparison is performed as "a < b".
template< typename It >
inline void insertion_sort( It it_begin, It it_end )
{
	insertion_sort( it_begin, it_end, std::less<>() );
}


/// Encapsulation of 'insertion_sort()' algorithm into a functional object.
struct insertion_sort_strategy
{
	template< typename It, typename Comp >
	void operator()( It begin, It end, Comp comp ) const
		{ insertion_sort( begin, end, comp ); }

	template< typename It >
	void operator()( It begin, It end ) const
		{ insertion_sort( begin, end ); }
};


} // namespace ml::sorting

#endif // ML__SORTING__INSERTION_SORT_HPP
