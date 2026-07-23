
#ifndef ML__SORTING__STL_SORT_STRATEGY_HPP
#define ML__SORTING__STL_SORT_STRATEGY_HPP

#include <algorithm>

namespace ml::sorting {


/// Encapsulation of 'std::sort()' algorithm into a functional object.
struct stl_sort_strategy
{
	template< typename It, typename Comp >
	void operator()( It begin, It end, Comp comp ) const
		{ std::sort( begin, end, comp ); }

	template< typename It >
	void operator()( It begin, It end ) const
		{ std::sort( begin, end ); }
};


}

#endif // ML__SORTING__STL_SORT_STRATEGY_HPP
