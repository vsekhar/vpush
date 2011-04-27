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
			member<Protein, util::toroidal_dimension, &Protein::x>
		>,
		ordered_non_unique<
			tag<byY>, 
			member<Protein, util::toroidal_dimension, &Protein::y>
		>,
		ordered_non_unique<
			tag<byZ>, 
			member<Protein, util::toroidal_dimension, &Protein::z>
		>,
		ordered_non_unique<
			tag<byEnergy>,
			member<Protein, double, &Protein::energy>
		>
	>
> soup_container;

struct soup_t {
	soup_t() {}
	soup_t(const soup_t& s) : container(s.container) {}
	void add(std::size_t, std::size_t protein_size = 0, double starting_energy = 0);
	void set_size(std::size_t soup_size, std::size_t protein_size = 0, double starting_energy = 0);
	inline void push_back(const Protein& e) { container.push_back(e); }

	template <typename FUNCTOR>
	void for_each(FUNCTOR& f) const {
		BOOST_FOREACH(const Protein& p, container)
			f(p);
	}
	
	template <typename FUNCTOR>
	void for_each(FUNCTOR& f) {
		typedef soup_container::index<bySeq>::type index;
		index& c = container.get<bySeq>();
		index::iterator i = c.begin();
		for(; i != c.end(); ++i)
			c.modify(i, f);
	}

	inline std::size_t size() const { return container.size(); }
	std::size_t deep_size() const;
	std::size_t deep_count() const;
	void clear() { container.clear(); }
	
	double energy() const;	

	double run(bool trace=false);
	void swap(soup_t& s) { container.swap(s.container); }
	
	typedef soup_container::index<bySeq>::type::const_iterator const_iterator;
	typedef soup_container::value_type value_type;
	const_iterator begin() const { return container.begin(); }
	const_iterator end() const { return container.end(); }
	
private:
	friend class ::boost::serialization::access;
	template <typename A> void serialize(A& a, unsigned int) { a & container; }
	soup_container container;
};

extern soup_t soup;

} // namespace vpush

#endif

