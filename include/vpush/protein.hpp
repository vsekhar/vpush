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
#include <vpush/detail/toroidal.hpp>
#include <vpush/util/random.hpp>
#include <vpush/exception.hpp>

namespace vpush {

namespace fus = ::boost::fusion;

struct size_accumulator {
	size_accumulator(const std::size_t& start) : value(start) {}
	template <typename T>
	void operator()(const T& t) const { value+=t.second.size(); }
	mutable std::size_t value;
};

struct clearer {
	template <typename T> void operator()(T& t) const { t.second.clear(); }
};

template <typename A>
struct serializer {
	serializer(A& ar) : archive(ar) {}
	template <typename T> void operator()(T& c) const { archive & c.second; }
	A& archive;
};

struct Protein {
	Protein()
		: x(0), y(0), z(0),	facing_x(0), facing_y(0), facing_z(0), energy(0) {
		using std::pow, std::sqrt;
		util::Random_01 rand();
		facing_x = rand();
		facing_y = rand();
		facing_z = rand();
		double mag = sqrt(pow(facing_x, 2), pow(facing_y, 2), pow(facing_z, 2));
		facing_x /= mag;
		facing_y /= mag;
		facing_z /= mag;
	}
	void reset() {
		fus::for_each(stacks, clearer());
	}
	
	std::size_t size() const {
		size_accumulator s(0);
		fus::for_each(stacks, s);
		return s.value;
	}

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

	// position
	detail::toroidal_dimension x;
	detail::toroidal_dimension y;
	detail::toroidal_dimension z;

	// facing
	detail::toroidal_dimension facing_x;
	detail::toroidal_dimension facing_y;
	detail::toroidal_dimension facing_z;
	
	// energy
	double energy;
	
private:
	friend class ::boost::serialization::access;
	template <class A> void serialize(A& a, unsigned int) {
		fus::for_each(stacks, serializer<A>(a));
		a & x;
		a & y;
		a & z;
		a & energy;
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

void print_trace(Protein&, std::ostream& o = std::cout);

} // namespace vpush

#endif

