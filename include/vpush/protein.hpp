#ifndef __VPUSH_PROTEIN_HPP__
#define __VPUSH_PROTEIN_HPP__

#include <vector>
#include <string>
#include <ostream>

#include <boost/ref.hpp>
#include <boost/foreach.hpp>

#include <boost/serialization/access.hpp>

#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include <vpush/protein_fwd.hpp>
#include <vpush/detail/code.hpp>
#include <vpush/detail/stack.hpp>
#include <vpush/detail/codestack.hpp>
#include <vpush/util/toroidal.hpp>
#include <vpush/util/vector.hpp>
#include <vpush/exception.hpp>

namespace vpush {

namespace fus = ::boost::fusion;

struct Protein {
	Protein()
		: x(0), y(0), z(0),	facing(util::normalized(util::random_vector())),
		  energy(0), running(false) {}

	// code and data stacks
	typedef fus::map<
		fus::pair<detail::Code, detail::codestack<detail::Code> >,
		fus::pair<detail::Exec, detail::codestack<detail::Exec> >,
		fus::pair<bool, detail::stack<bool> >,
		fus::pair<int, detail::stack<int> >,
		fus::pair<double, detail::stack<double> >,
		fus::pair<std::string, detail::stack<std::string> >
	> stacks_t;
	stacks_t stacks;
	
	template <typename STACK_T>
	inline detail::stack<STACK_T>& get() {
		return fus::at_key<STACK_T>(this->stacks);
	}
	
	template <typename FUNCTOR>
	inline void for_each(FUNCTOR &f) {
		::boost::fusion::for_each(stacks, f);
	}

	template <typename FUNCTOR>
	inline void for_each(FUNCTOR &f) const {
		::boost::fusion::for_each(stacks, f);
	}

	void reset();
	std::size_t size() const;
	std::size_t count() const;

	// position
	util::toroidal_dimension x;
	util::toroidal_dimension y;
	util::toroidal_dimension z;

	// facing
	util::vector facing;
	
	// energy
	double energy;
	
	bool running;
	
private:
	friend class ::boost::serialization::access;

	template <typename A>
	struct serializer {
		serializer(A& ar) : archive(ar) {}
		template <typename T> void operator()(T& c) const { archive & c.second; }
		A& archive;
	};

	template <class A> void serialize(A& a, unsigned int) {
		fus::for_each(stacks, serializer<A>(a));
		a & x;
		a & y;
		a & z;
		a & energy;
		a & running;
	}

}; // struct Protein

// stack access
template <> inline detail::stack<detail::Code>& stack(Protein& p) { return p.get<detail::Code>(); }
template <> inline detail::stack<detail::Exec>& stack(Protein& p) { return p.get<detail::Exec>(); }
template <> inline detail::stack<bool>& stack(Protein& p) { return p.get<bool>(); }
template <> inline detail::stack<int>& stack(Protein& p) { return p.get<int>(); }
template <> inline detail::stack<double>& stack(Protein& p) { return p.get<double>(); }
template <> inline detail::stack<std::string>& stack(Protein& p) { return p.get<std::string>(); }
// template <> inline detail::stack<Name>& stack(Protein& p) { return p.name_stack; }

void print_protein(Protein&, std::ostream& o = std::cout);
Protein random_protein(std::size_t);

} // namespace vpush

#endif

