#ifndef __VPUSH_PROTEIN_HPP__
#define __VPUSH_PROTEIN_HPP__

#include <vector>
#include <string>

#include <boost/ref.hpp>
#include <boost/foreach.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/at_key.hpp>
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
	> stacks_t;
	stacks_t stacks;
	
	// co-ordinates
	detail::toroidal_vector location;
	
	template <typename STACK_T, typename EXT_PROTEIN = Protein>
	inline detail::stack<STACK_T>& get() {
		EXT_PROTEIN& c = static_cast<EXT_PROTEIN&>(*this);
		return fus::at_key<STACK_T>(c.stacks);
	}

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

template <> inline detail::stack<bool>& get_stack(Protein& p) { return p.get<bool>(); }
template <> inline detail::stack<int>& get_stack(Protein& p) { return p.get<int>(); }
template <> inline detail::stack<double>& get_stack(Protein& p) { return p.get<double>(); }
template <> inline detail::stack<detail::Code>& get_stack(Protein& p) { return p.get<detail::Code>(); }
template <> inline detail::stack<detail::Exec>& get_stack(Protein& p) { return p.get<detail::Exec>(); }
// template <> inline detail::stack<Name>& get_stack(Protein& p) { return p.name_stack; }

} // namespace vpush

#endif

