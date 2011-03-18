#ifndef __VPUSH_PROTEIN_EXT_HPP__
#define __VPUSH_PROTEIN_EXT_HPP__

#include <string>

#include <vpush/protein.hpp>

namespace vpush {

/******************************************************************************
	How to extend the protein to new types:
	
	Types must be copyable and serializable.

******************************************************************************/

// 1. New Protein struct
struct ExtendedProtein : Protein {
	typedef Protein base_type;

	// 2. New stacks to contain the additional types
	// code and data stacks
	typedef fus::map<
		fus::pair<std::string, detail::stack<std::string> >
		, fus::pair<char, detail::stack<char> >
	> stacks_t
	stacks_t stacks;
	
	// 3. Reset the additional stacks
	void reset() {
		fus::for_each(stacks, clearer());
		base_type::reset();		// base
	}
	
	std::size_t size() const {
		size_accumulator s(0);
		fus::for_each(stacks, s);
		s.value += base_type::size();	// base
		return s.value;
	}
	
private:

	// 4. Serialization
	friend class ::boost::serialization::access;
	template <typename A> void serialize(A& a, unsigned int) {
		a & ::boost::serialization::base_object<base_type>(*this);
		fus::for_each(stacks, serializer<A>(a));
	}
};

// 5. Stack access (can you define these inside the template to templatize both at once?
inline ExtendedProtein& do_cast(Protein& p) { return static_cast<ExtendedProtein&>(p);}
template <> inline detail::stack<std::string>& get_stack(Protein& p) {
	return fus::at_key<std::string>(do_cast(p).stacks);
}
template <> inline detail::stack<char>& get_stack(Protein& e) {
	return fus::at_key<char>(do_cast(p).stacks);
}

} // namespace vpush

#endif

