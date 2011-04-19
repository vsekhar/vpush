#ifndef __VPUSH_SOUP_HPP__
#define __VPUSH_SOUP_HPP__

#include <boost/serialization/access.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <vpush/protein.hpp>

namespace vpush {

using namespace ::boost::multi_index;

struct bySeq;
struct byX;
struct byY;
struct byZ;
struct byEnergy;

typedef multi_index_container <
	Protein,
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
> soup_container;

struct soup_t {
	void add(std::size_t);
	void set_size(std::size_t);
	const Protein& operator[](std::size_t) const;

	inline void push_back(const Protein& e) { container.push_back(e); }
	inline std::size_t size() const { return container.size(); }
	std::size_t deep_size() const;
	void clear() { container.clear(); }
	
	typedef void (*modifier)(Protein&);
	
	void run();
	void swap(soup_t& s) {container.swap(s.container);}
	
private:
	friend class ::boost::serialization::access;
	template <typename A> void serialize(A& a, unsigned int) { a & container; }
	soup_container container;
};

extern soup_t soup;

} // namespace vpush

#endif

