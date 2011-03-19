#ifndef __VPUSH_SOUP_HPP__
#define __VPUSH_SOUP_HPP__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <vpush/protein_ext.hpp>

namespace vpush {

using namespace ::boost::multi_index;

struct bySeq;
struct byX;
struct byY;
struct byZ;
struct byEnergy;

typedef multi_index_container <
	ExtendedProtein,
	indexed_by<
		sequenced<tag<bySeq> >,
		ordered_non_unique<
			tag<byX>, 
			member<Protein, detail::toroidal_dimension, &Protein::x>
		>,
		ordered_non_unique<
			tag<byY>, 
			member<Protein, detail::toroidal_dimension, &Protein::y>
		>,
		ordered_non_unique<
			tag<byZ>, 
			member<Protein, detail::toroidal_dimension, &Protein::z>
		>,
		ordered_non_unique<
			tag<byEnergy>,
			member<Protein, double, &Protein::energy>
		>
	>
> Soup_t;

extern Soup_t soup;

} // namespace vpush

#endif

