
#ifndef ML__SORTING__STL_MERGE_STRATEGY_HPP
#define ML__SORTING__STL_MERGE_STRATEGY_HPP

#include <algorithm>

namespace ml::sorting {


/// Encapsulation of 'std::merge()' algorithm into a functional object.
struct stl_merge_strategy
{
	template< typename InIt1, typename InIt2, typename OutIt, 
			typename Comp >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, 
			OutIt out, Comp comp ) const
		{ return std::merge( it_1, end_1, it_2, end_2, 
				out, comp ); }

	template< typename InIt1, typename InIt2, typename OutIt >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, 
			OutIt out ) const
		{ return std::merge( it_1, end_1, it_2, end_2, out ); }
};


} // namespace ml::sorting

#endif // ML__SORTING__STL_MERGE_STRATEGY_HPP
