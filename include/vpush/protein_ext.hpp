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
	detail::stack<std::string> string_stack;
	detail::stack<char> char_stack;

	// 3. Reset the additional stacks
	void reset() {
		string_stack.clear();
		char_stack.clear();
		
		// don't forget to reset the stacks in the base class
		base_type::reset();
	}
	
private:

	// 4. Serialization
	friend class ::boost::serialization::access;
	template <typename A> void serialize(A& a, unsigned int) {
		a & ::boost::serialization::base_object<base_type>(*this);
		a & string_stack;
		a & char_stack;
	}
};

// 5. Stack access
template <> inline detail::stack<std::string>& get_stack(Protein& e) {
	return static_cast<ExtendedProtein&>(e).string_stack;
}
template <> inline detail::stack<char>& get_stack(Protein& e) {
	return static_cast<ExtendedProtein&>(e).char_stack;
}

} // namespace vpush

#endif

