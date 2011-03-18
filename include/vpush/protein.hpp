#ifndef __VPUSH_PROTEIN_HPP__
#define __VPUSH_PROTEIN_HPP__

#include <vector>
#include <string>

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include <vpush/protein_fwd.hpp>
#include <vpush/detail/stack.hpp>
#include <vpush/detail/codestack.hpp>
#include <vpush/detail/toroidal.hpp>
#include <vpush/detail/code.hpp>
#include <vpush/detail/functions.hpp>
#include <vpush/exception.hpp>

namespace vpush {

namespace fus = ::boost::fusion;

struct size_accumulator {
	size_accumulator(const std::size_t& start) : value(start) {}
	template <typename C>
	void operator()(const T& t) { value+=t.size(); }
	std::size_t value;
};

struct clearer {
	template <typename C> void operator()(T& t) { t.clear(); }
};

template <typename A>
struct serializer {
	serializer(A& ar) : archive(ar) {}
	template <typename C> void operator()(C& c) { archive & c; }
	A& archive;
};

struct Protein {
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
		fus::pair<detail::Code, detail::codestack<detail::Code> >
		, fus::pair<detail::Exec, detail::codestack<detail::Exec> >
		, fus::pair<bool, detail::stack<bool> >
		, fus::pair<int, detail::stack<int> >
		, fus::pair<double, detail::stack<double> >
		, fus::pair<std::string, detail::stack<std::string> >
	> stacks_t
	stacks_t stacks;
	
	// co-ordinates
	detail::toroidal_vector location;

private:
	friend class ::boost::serialization::access;
	template <class A> void serialize(A& a, unsigned int) {
		fus::for_each(stacks, serializer<A>(a));
	}

}; // struct Protein

// stack access
template <typename T> inline detail::stack<T>& get_stack(Protein&) {
	throw detail::no_such_stack(typeid(T));
}

} // namespace vpush

#endif

