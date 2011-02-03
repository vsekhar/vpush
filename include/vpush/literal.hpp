/***********************************************************************
vpush::detail::literal<> is a template for instantiating literals

Instantiate a literal with a state type and an initial state. For simple
types, use:
	literal<int>(4);

For more complex types, define a struct that stores the state, provides
init and mutate methods, and is convertible to the type to be pushed onto
stacks:

	struct my_int_state_type {
		void init() { // do something }
		void mutate() { // do something }
		operator int() { // convert to type to be put on stacks }
		typedef int result_type;	// tells vpush how to convert it
	private:
		int _internal_state_storage;
	};

Then tell vpush both the state structure and the type it should be
converted to when pushing onto the stack:

	literal<my_int_state_type>
***********************************************************************/

#ifndef __VPUSH_LITERAL_HPP__
#define __VPUSH_LITERAL_HPP__

#include <iostream>

#include <boost/any.hpp>

#include <vpush/code.hpp>
#include <vpush/stack.hpp>

namespace vpush {
namespace detail {

template <typename T>
void literal_func(const T& state) { push<T>(state); }

template <typename T>
struct literal : element_base {
	literal(const T& s) : element_base(LITERAL), _state(s) {}
	virtual void exec() const { push<T>(boost::any_cast<T>(_state)); }
	virtual std::ostream& save_state(std::ostream& o) const{
		return o << boost::any_cast<const T&>(_state);
	}
	virtual std::istream& load_state(std::istream& i) {
		return i >> boost::any_cast<T&>(_state);
	}
	boost::any _state;
};

} // namespace detail
} // namespace vpush

#endif

