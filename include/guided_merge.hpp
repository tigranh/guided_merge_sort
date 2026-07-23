
#ifndef ML__SORTING__GUIDED_MERGE_HPP
#define ML__SORTING__GUIDED_MERGE_HPP

#include <functional>
#include <algorithm>
#include <cassert>

namespace ml::sorting {

//
// guided_2_merge
//

/// Performs merge of two sorted sequences, according to the 
/// "Guided merge" algorithm.
template< typename InIt1, typename InIt2, 
		typename OutIt, 
		typename Comp >
inline OutIt guided_2_merge(
		InIt1 a_it, InIt1 a_end, 
		InIt2 b_it, InIt2 b_end,
		OutIt out, 
		Comp comp )
{
	using std::move;
	if ( a_it != a_end && b_it != b_end ) {
		// Initial decision on which order to start from
		if ( comp( *a_it, *b_it ) )  // "*a_it < *b_it"
			goto ab;
		else
			goto ba;
		// Sequence of the sections
	ab: 
		*(out++) = move( *(a_it++) );
		if ( a_it == a_end )
			goto finish_1;
		if ( comp( *a_it, *b_it ) )
			goto ab;  // The order didn't change
		//else
		//	goto ba;  // We can skip the jump, as the next instruction 
		              // is already what needed.
	ba:
		*(out++) = move( *(b_it++) );
		if ( b_it == b_end )
			goto finish_1;
		if ( comp( *b_it, *a_it ) )
			goto ba;  // The order didn't change
		else
			goto ab;  // The order has changed
	}
finish_1:  // Once only one sequence remains, we continue 
	       // from here.
	if ( a_it != a_end )
		out = std::copy( a_it, a_end, out );
	else
		out = std::copy( b_it, b_end, out );
	return out;
}


/// Overload, where comparison is performed as "a < b".
template< typename InIt1, typename InIt2, 
		typename OutIt >
inline OutIt guided_2_merge(
		InIt1 a_it, InIt1 a_end, 
		InIt2 b_it, InIt2 b_end,
		OutIt out )
{
	return guided_2_merge( a_it, a_end, b_it, b_end, out, std::less<>() );
}


/// Encapsulation of 'guided_2_merge' algorithm into a functional object.
struct guided_2_merge_strategy
{
	template< typename InIt1, typename InIt2, typename OutIt, 
			typename Comp >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, 
			OutIt out, Comp comp ) const
		{ return guided_2_merge( it_1, end_1, it_2, end_2, 
				out, comp ); }

	template< typename InIt1, typename InIt2, typename OutIt >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, 
			OutIt out ) const
		{ return guided_2_merge( it_1, end_1, it_2, end_2, out ); }
};

//
// guided_3_merge
//

/// Knowing that 'p' comes before 'q', decides the sorted order of 
/// all the 3 values 'p', 'q' and 'r', and jumps to one of possible 
/// 3 labels.
#define DECIDE_OVER_PQ_ITEM_R( p, q, r, comp, \
		pqr_label, prq_label, rpq_label ) \
	assert( ! comp( (q), (p) ) );  /* "p <= q" */ \
	if ( comp( (q), (r) ) ) \
		goto pqr_label; \
	else { \
		if ( comp( (p), (r) ) ) \
			goto prq_label; \
		else \
			goto rpq_label; \
	}
	

/// Decides the sorted order of the 3 values 'p', 'q' and 'r', 
/// and jumps to one of the 6 possible labels.
#define DECIDE_OVER_P_ITEMS_QR( p, q, r, comp, \
		pqr_label, prq_label, qpr_label, qrp_label, rpq_label, rqp_label ) \
	if ( comp( (p), (q) ) ) { \
		DECIDE_OVER_PQ_ITEM_R( p, q, r, comp, pqr_label, prq_label, rpq_label ); \
	} \
	else { \
		DECIDE_OVER_PQ_ITEM_R( q, p, r, comp, qpr_label, qrp_label, rqp_label ); \
	}


/// This directive proceses the first (minimal) item 'p_it' of the 
/// sorted sequences, starting from 'p_it', 'q_it' and 'r_it', and  
/// places it in the sequence, starting from 'out_it'.
/// Then it compares the next item from 'p_it', decides the new 
/// order of items 'p_it', 'q_it' and 'r_it', and jumps to corresponding 
/// label.
/// The comparator 'comp' is used for comparison.
/// Once the first sequence [p_it, p_end) is exhausted, it jumps to the 
/// 'finish_label'.
#define PROCESS_3_SORTED_ITEMS_LR( p_it, p_end, q_it, r_it, \
		out_it, comp, \
		pqr_label, qpr_label, qrp_label, finish_label ) \
	*((out_it)++) = std::move( *((p_it)++) ); \
	if ( (p_it) == (p_end) ) \
		goto finish_label; \
	if ( comp( *(p_it), *(q_it) ) ) \
		goto pqr_label; \
	else { \
		if ( comp( *(p_it), *(r_it) ) ) \
			goto qpr_label; \
		else \
			goto qrp_label; \
	}

/// Same as 'PROCESS_3_SORTED_ITEMS_LR', but the comparisons are 
/// made in a bit different order, so the order of jumps is also 
/// different.
#define PROCESS_3_SORTED_ITEMS_RL( p_it, p_end, q_it, r_it, \
		out_it, comp, \
		pqr_label, qrp_label, qpr_label, finish_label ) \
	*((out_it)++) = std::move( *((p_it)++) ); \
	if ( (p_it) == (p_end) ) \
		goto finish_label; \
	if ( comp( *(p_it), *(q_it) ) ) \
		goto pqr_label; \
	else { \
		if ( comp( *(r_it), *(p_it) ) ) \
			goto qrp_label; \
		else \
			goto qpr_label; \
	}


/// Performs merge of three sorted sequences, according to the 
/// "Guided merge" algorithm.
template< typename InIt1, typename InIt2, typename InIt3, 
		typename OutIt, 
		typename Comp >
inline OutIt guided_3_merge(
		InIt1 a_it, InIt1 a_end, 
		InIt2 b_it, InIt2 b_end,
		InIt3 c_it, InIt3 c_end, 
		OutIt out, 
		Comp comp )
{
	if ( a_it != a_end && b_it != b_end && c_it != c_end ) {
		// Initial decision on which order to start from
		DECIDE_OVER_P_ITEMS_QR( *a_it, *b_it, *c_it, comp, 
				abc, acb, bac, bca, cab, cba );
		// Sequence of the sections
	abc:
		PROCESS_3_SORTED_ITEMS_LR( a_it, a_end, b_it, c_it, 
				out, comp, abc, bac, bca, finish_2 );
	bca:
		PROCESS_3_SORTED_ITEMS_LR( b_it, b_end, c_it, a_it, 
				out, comp, bca, cba, cab, finish_2 );
	cab:
		PROCESS_3_SORTED_ITEMS_RL( c_it, c_end, a_it, b_it, 
				out, comp, cab, abc, acb, finish_2 );
	acb:
		PROCESS_3_SORTED_ITEMS_LR( a_it, a_end, c_it, b_it, 
				out, comp, acb, cab, cba, finish_2 );
	cba:
		PROCESS_3_SORTED_ITEMS_LR( c_it, c_end, b_it, a_it, 
				out, comp, cba, bca, bac, finish_2 );
	bac:
		PROCESS_3_SORTED_ITEMS_RL( b_it, b_end, a_it, c_it, 
				out, comp, bac, acb, abc, finish_2 );
	}
finish_2:  // Once only two sequences remain, we continue 
	       // from here.
	if ( a_it == a_end )
		out = guided_2_merge( b_it, b_end, c_it, c_end, out, comp );
	else if ( b_it == b_end )
		out = guided_2_merge( a_it, a_end, c_it, c_end, out, comp );
	else
		out = guided_2_merge( a_it, a_end, b_it, b_end, out, comp );
	return out;
}


/// Overload, for the case when comparison is performed as "a < b".
template< typename InIt1, typename InIt2, typename InIt3,
		typename OutIt >
inline OutIt guided_3_merge(
		InIt1 a_it, InIt1 a_end,
		InIt2 b_it, InIt2 b_end,
		InIt3 c_it, InIt3 c_end,
		OutIt out )
{
	return guided_3_merge( a_it, a_end, b_it, b_end, c_it, c_end, 
			out, std::less<>() );
}


/// Encapsulation of 'guided_3_merge' algorithm into a functional object.
struct guided_3_merge_strategy
{
	template< typename InIt1, typename InIt2, typename InIt3, 
			typename OutIt, typename Comp >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, InIt3 it_3, InIt3 end_3, 
			OutIt out, Comp comp ) const
		{ return guided_3_merge( it_1, end_1, it_2, end_2, 
				it_3, end_3, out, comp ); }

	template< typename InIt1, typename InIt2, typename InIt3, 
			typename OutIt >
	OutIt operator()( InIt1 it_1, InIt1 end_1, 
			InIt2 it_2, InIt2 end_2, InIt3 it_3, InIt3 end_3, 
			OutIt out ) const
		{ return guided_3_merge( it_1, end_1, it_2, end_2, 
				it_3, end_3, out ); }
};

//
// guided_4_merge
//

/// Knowing that 'p' comes before 'q', which comes before 'r', 
/// decides the sorted order of all the 4 values 'p', 'q', 'r', and 's', 
/// and jumps to one of 4 possible labels.
#define DECIDE_OVER_PQR_ITEM_S( p, q, r, s, comp, \
		spqr_label, psqr_label, pqsr_label, pqrs_label ) \
	assert( ! comp( (q), (p) ) );  /* "p <= q" */ \
	assert( ! comp( (r), (q) ) );  /* "q <= r" */ \
	if ( comp( (s), (q) ) ) { \
		if ( comp( (s), (p) ) ) \
			goto spqr_label; \
		else \
			goto psqr_label; \
	} \
	else { \
		if ( comp( (s), (r) ) ) \
			goto pqsr_label; \
		else \
			goto pqrs_label; \
	}

/// Knowing that 'p' comes before 'q', decides the sorted order of 
/// all the 4 values 'p', 'q', 'r', and 's', and jumps to one of 
/// possible 12 labels.
#define DECIDE_OVER_PQ_ITEMS_RS( p, q, r, s, comp, \
		spqr_label, psqr_label, pqsr_label, pqrs_label, \
		sprq_label, psrq_label, prsq_label, prqs_label, \
		srpq_label, rspq_label, rpsq_label, rpqs_label ) \
	assert( ! comp( (q), (p) ) );  /* "p <= q" */ \
	if ( comp( (q), (r) ) ) { \
		DECIDE_OVER_PQR_ITEM_S( p, q, r, s, comp, \
				spqr_label, psqr_label, pqsr_label, pqrs_label ); \
	} else { \
		if ( comp( (p), (r) ) ) { \
			DECIDE_OVER_PQR_ITEM_S( p, r, q, s, comp, \
					sprq_label, psrq_label, prsq_label, prqs_label ); \
		} else { \
			DECIDE_OVER_PQR_ITEM_S( r, p, q, s, comp, \
					srpq_label, rspq_label, rpsq_label, rpqs_label ); \
		} \
	}
	

/// This directive proceses the first (minimal) item 'p_it' of the 
/// sorted sequences, starting from 'p_it', 'q_it', 'r_it', and 's_it', 
/// and places it in the sequence, starting from 'out_it'.
/// Then it compares the next item from 'p_it', decides the new 
/// order of items 'p_it', 'q_it', 'r_it', and 's_it' , and jumps to 
/// corresponding label.
/// The comparator 'comp' is used for comparison.
/// Once the first sequence [p_it, p_end) is exhausted, it jumps to the 
/// 'finish_label'.
#define PROCESS_4_SORTED_ITEMS_LR( p_it, p_end, q_it, r_it, s_it, \
		out_it, comp, \
		pqrs_label, qprs_label, qrps_label, qrsp_label, finish_label ) \
	*((out_it)++) = std::move( *((p_it)++) ); \
	if ( (p_it) == (p_end) ) \
		goto finish_label; \
	if ( comp( *(p_it), *(r_it) ) ) { \
		if ( comp( *(p_it), *(q_it) ) ) \
			goto pqrs_label; \
		else \
			goto qprs_label; \
	} \
	else { \
		if ( comp( *(p_it), *(s_it) ) ) \
			goto qrps_label; \
		else \
			goto qrsp_label; \
	}

/// Same as 'PROCESS_4_SORTED_ITEMS_LR', but the comparisons are 
/// made in a bit different order, so the order of jumps is also 
/// different.
#define PROCESS_4_SORTED_ITEMS_RL( p_it, p_end, q_it, r_it, s_it, \
		out_it, comp, \
		pqrs_label, qprs_label, qrps_label, qrsp_label, finish_label ) \
	assert( (p_it) != (p_end) ); \
	assert( ! comp( *(q_it), *(p_it) ) );  /* "p <= q" */ \
	assert( ! comp( *(r_it), *(q_it) ) );  /* "q <= r" */ \
	assert( ! comp( *(s_it), *(r_it) ) );  /* "r <= s" */ \
	*((out_it)++) = std::move( *((p_it)++) ); \
	if ( (p_it) == (p_end) ) \
		goto finish_label; \
	if ( comp( *(r_it), *(p_it) ) ) { \
		if ( comp( *(p_it), *(s_it) ) ) \
			goto qrps_label; \
		else \
			goto qrsp_label; \
	} \
	else { \
		if ( comp( *(p_it), *(q_it) ) ) \
			goto pqrs_label; \
		else \
			goto qprs_label; \
	}


/// Performs merge of four sorted sequences, according to the 
/// "Guided merge" algorithm.
template< typename InIt1, typename InIt2, 
		typename InIt3, typename InIt4, 
		typename OutIt, typename Comp >
inline OutIt guided_4_merge(
		InIt1 a_it, InIt1 a_end, InIt2 b_it, InIt2 b_end,
		InIt3 c_it, InIt3 c_end, InIt4 d_it, InIt4 d_end,
		OutIt out, 
		Comp comp )
{
	if ( a_it != a_end && b_it != b_end 
			&& c_it != c_end && d_it != d_end ) {
		// Initial decision on which order to start from
		if ( comp( *a_it, *b_it ) ) {
			DECIDE_OVER_PQ_ITEMS_RS( *a_it, *b_it, *c_it, *d_it, comp, 
					dabc, adbc, abdc, abcd, 
					dacb, adcb, acdb, acbd,
					dcab, cdab, cadb, cabd );
		}
		else {
			DECIDE_OVER_PQ_ITEMS_RS( *b_it, *a_it, *c_it, *d_it, comp, 
					dbac, bdac, badc, bacd, 
					dbca, bdca, bcda, bcad,
					dcba, cdba, cbda, cbad );
		}
		// Sequence of the sections
	abcd:
		PROCESS_4_SORTED_ITEMS_LR( a_it, a_end, b_it, c_it, d_it, out, comp, 
			abcd, bacd, bcad, bcda, finish_3 );
	bcda:
		PROCESS_4_SORTED_ITEMS_LR( b_it, b_end, c_it, d_it, a_it, out, comp, 
			bcda, cbda, cdba, cdab, finish_3 );
	cdab:
		PROCESS_4_SORTED_ITEMS_LR( c_it, c_end, d_it, a_it, b_it, out, comp, 
			cdab, dcab, dacb, dabc, finish_3 );
	dabc:
		PROCESS_4_SORTED_ITEMS_RL( d_it, d_end, a_it, b_it, c_it, out, comp, 
			dabc, adbc, abdc, abcd, finish_3 );
	//
	adbc:
		PROCESS_4_SORTED_ITEMS_LR( a_it, a_end, d_it, b_it, c_it, out, comp, 
			adbc, dabc, dbac, dbca, finish_3 );
	dbca:
		PROCESS_4_SORTED_ITEMS_LR( d_it, d_end, b_it, c_it, a_it, out, comp, 
			dbca, bdca, bcda, bcad, finish_3 );
	bcad:
		PROCESS_4_SORTED_ITEMS_LR( b_it, b_end, c_it, a_it, d_it, out, comp, 
			bcad, cbad, cabd, cadb, finish_3 );
	cadb:
		PROCESS_4_SORTED_ITEMS_RL( c_it, c_end, a_it, d_it, b_it, out, comp, 
			cadb, acdb, adcb, adbc, finish_3 );
	//
	acdb:
		PROCESS_4_SORTED_ITEMS_LR( a_it, a_end, c_it, d_it, b_it, out, comp, 
			acdb, cadb, cdab, cdba, finish_3 );
	cdba:
		PROCESS_4_SORTED_ITEMS_LR( c_it, c_end, d_it, b_it, a_it, out, comp, 
			cdba, dcba, dbca, dbac, finish_3 );
	dbac:
		PROCESS_4_SORTED_ITEMS_LR( d_it, d_end, b_it, a_it, c_it, out, comp, 
			dbac, bdac, badc, bacd, finish_3 );
	bacd:
		PROCESS_4_SORTED_ITEMS_RL( b_it, b_end, a_it, c_it, d_it, out, comp, 
			bacd, abcd, acbd, acdb, finish_3 );
	//
	abdc:
		PROCESS_4_SORTED_ITEMS_LR( a_it, a_end, b_it, d_it, c_it, out, comp, 
			abdc, badc, bdac, bdca, finish_3 );
	bdca:
		PROCESS_4_SORTED_ITEMS_LR( b_it, b_end, d_it, c_it, a_it, out, comp, 
			bdca, dbca, dcba, dcab, finish_3 );
	dcab:
		PROCESS_4_SORTED_ITEMS_LR( d_it, d_end, c_it, a_it, b_it, out, comp, 
			dcab, cdab, cadb, cabd, finish_3 );
	cabd:
		PROCESS_4_SORTED_ITEMS_RL( c_it, c_end, a_it, b_it, d_it, out, comp, 
			cabd, acbd, abcd, abdc, finish_3 );
	//
	acbd:
		PROCESS_4_SORTED_ITEMS_LR( a_it, a_end, c_it, b_it, d_it, out, comp, 
			acbd, cabd, cbad, cbda, finish_3 );
	cbda:
		PROCESS_4_SORTED_ITEMS_LR( c_it, c_end, b_it, d_it, a_it, out, comp, 
			cbda, bcda, bdca, bdac, finish_3 );
	bdac:
		PROCESS_4_SORTED_ITEMS_LR( b_it, b_end, d_it, a_it, c_it, out, comp, 
			bdac, dbac, dabc, dacb, finish_3 );
	dacb:
		PROCESS_4_SORTED_ITEMS_RL( d_it, d_end, a_it, c_it, b_it, out, comp, 
			dacb, adcb, acdb, acbd, finish_3 );

	//
	adcb:
		PROCESS_4_SORTED_ITEMS_LR( a_it, a_end, d_it, c_it, b_it, out, comp, 
			adcb, dacb, dcab, dcba, finish_3 );
	dcba:
		PROCESS_4_SORTED_ITEMS_LR( d_it, d_end, c_it, b_it, a_it, out, comp, 
			dcba, cdba, cbda, cbad, finish_3 );
	cbad:
		PROCESS_4_SORTED_ITEMS_LR( c_it, c_end, b_it, a_it, d_it, out, comp, 
			cbad, bcad, bacd, badc, finish_3 );
	badc:
		PROCESS_4_SORTED_ITEMS_RL( b_it, b_end, a_it, d_it, c_it, out, comp, 
			badc, abdc, adbc, adcb, finish_3 );
	}
finish_3:  // Once only three sequences remain, we continue 
	       // from here.
	if ( a_it == a_end )
		out = guided_3_merge( b_it, b_end, c_it, c_end, d_it, d_end, out, comp );
	else if ( b_it == b_end )
		out = guided_3_merge( a_it, a_end, c_it, c_end, d_it, d_end, out, comp );
	else if ( c_it == c_end )
		out = guided_3_merge( a_it, a_end, b_it, b_end, d_it, d_end, out, comp );
	else
		out = guided_3_merge( a_it, a_end, b_it, b_end, c_it, c_end, out, comp );
	return out;
}


/// Overload, for the case when comparison is performed as "a < b".
template< typename InIt1, typename InIt2, typename InIt3, typename InIt4, 
		typename OutIt >
inline OutIt guided_4_merge(
		InIt1 a_it, InIt1 a_end, InIt2 b_it, InIt2 b_end,
		InIt3 c_it, InIt3 c_end, InIt4 d_it, InIt4 d_end, 
		OutIt out )
{
	return guided_4_merge( a_it, a_end, b_it, b_end, 
			c_it, c_end, d_it, d_end, out, std::less<>() );
}

/// Encapsulation of 'guided_4_merge' algorithm into a functional object.
struct guided_4_merge_strategy
{
	template< typename InIt1, typename InIt2, typename InIt3, 
			typename InIt4, 
			typename OutIt, typename Comp >
	OutIt operator()( InIt1 it_1, InIt1 end_1, InIt2 it_2, InIt2 end_2, 
			InIt3 it_3, InIt3 end_3, InIt4 it_4, InIt4 end_4, 
			OutIt out, Comp comp ) const
		{ return guided_4_merge( it_1, end_1, it_2, end_2, 
				it_3, end_3, it_4, end_4, out, comp ); }

	template< typename InIt1, typename InIt2, typename InIt3, 
			typename InIt4, 
			typename OutIt >
	OutIt operator()( InIt1 it_1, InIt1 end_1, InIt2 it_2, InIt2 end_2, 
			InIt3 it_3, InIt3 end_3, InIt4 it_4, InIt4 end_4, 
			OutIt out ) const
		{ return guided_4_merge( it_1, end_1, it_2, end_2, 
				it_3, end_3, it_4, end_4, out ); }
};


} // namespace ml::sorting

#endif // ML__SORTING__GUIDED_MERGE_HPP
