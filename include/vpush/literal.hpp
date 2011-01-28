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

#include <vpush/code.hpp>
#include <vpush/stack.hpp>

namespace vpush {
namespace detail {

struct literal_base : element_base {
	// virtual functions for serializing from i/ostream
};

template <typename STATE>
struct literal_with_state : literal_base {
	// virtual functions for serializing from i/ostream, and returning
	
private:
	STATE _state;
};

struct state_base {
	virtual void init() = 0;
	virtual void mutate() = 0;
};

template <typename T>
struct default_state {
	simple_state(const simple_state& s) : _value(s._value) {}
	simple_state(const T& t) : _value(t) {}
	void init() { _value = T(); }
	void mutate() {}
	typedef T result_type;
	operator result_type() const { return _value; }
private:
	T _value;
};

template <typename STATE>
struct literal : element_base {
	typedef STATE::result_type result_type;
	literal(const STATE& s) : element_base(LITERAL), _state(s) {}
	
	void init() { _state.init(); }
	void mutate() { _state.mutate(); }
	virtual void push_state() const { push<result_type>(_state); }
	
	

private:
	STATE _state;
};

} // namespace detail
} // namespace vpush

#endif

