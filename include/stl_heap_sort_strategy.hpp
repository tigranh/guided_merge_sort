
#ifndef ML__SORTING__STL_HEAP_SORT_STRATEGY_HPP
#define ML__SORTING__STL_HEAP_SORT_STRATEGY_HPP

#include <algorithm>

namespace ml::sorting {


/// Encapsulation of sorting, which is done using STL heap-manipulation 
/// routines.
struct stl_heap_sort_strategy
{
	template< typename It, typename Comp >
	void operator()( It begin, It end, Comp comp ) const {
		std::make_heap( begin, end, comp );
		std::sort_heap( begin, end, comp );
	}

	template< typename It >
	void operator()( It begin, It end ) const {
		std::make_heap( begin, end );
		std::sort_heap( begin, end );
	}
};


}

#endif // ML__SORTING__STL_HEAP_SORT_STRATEGY_HPP
