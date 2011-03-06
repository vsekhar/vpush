#ifndef __vpush_env_hpp__
#define __vpush_env_hpp__

#include <string>
#include <vector>

#include <boost/preprocessor/cat.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/mpl/fold.hpp>
#include <boost/mpl/for_each.hpp>

#include <vpush/exception.hpp>

namespace vpush {

namespace detail {

// Bastardizing policy composition to build stacks
struct Env_null {
	template <typename T> std::vector<T>& get_stack() {
		throw detail::no_such_stack(typeid(T));
	}
	void reset() {}
};

template <class base_env, class new_type>
struct Env_cons : public base_env {
	typedef new_type value_type;
	typedef base_env base_type;
	typedef Env_cons<base_env, new_type> this_type;

	void reset() { this_stack.clear(); base_env::reset(); }
	template <typename T> inline std::vector<T>& get_stack() { return stack_getter<T, this_type>::get(*this); }

	/* stack-wide operations queries & operations */
	template <typename T> inline std::size_t size() {return get_stack<T>().size();}
	template <typename T> inline bool empty() {return get_stack<T>().empty();}
	template <typename T> inline void clear() {get_stack<T>().clear();}

	/* non-modifying element access */
	template <typename T> inline T& top() { return get_stack<T>().back(); }
	template <class T> inline    T& first() { return top<T>(); }
	template <class T> inline    T& second() { return *(++get_stack<T>().rbegin()); }

	/* modifying element access (pushing and popping) */
	template <typename T> inline void push(const T& v) { get_stack<T>().push_back(v); }
	template <typename T> inline T pop() {
		std::vector<T>& stack = get_stack<T>();
		if(stack.empty()) throw detail::stack_underflow(typeid(T));
		T ret(stack.back());
		stack.pop_back();
		return ret;
	}

	/* non-standard stack operations (for combinatorics operators) */
	template <typename T>
	inline void push_second(const T& t) {
		std::vector<T>& stack = get_stack<T>();
		typename std::vector<T>::reverse_iterator itr = stack.rbegin();
		if(!stack.empty()) itr++;
		stack.insert(itr.base(), t);
	}

	template <typename T>
	inline T pop_second() {
		std::vector<T>& stack = get_stack<T>();
		if(stack.size() < 2) throw detail::stack_underflow(typeid(T));

		// some funny iterator arithmetic for converting between
		// reverse and forward iterators
		typename std::vector<T>::reverse_iterator second
			= ++stack.rbegin();
		T ret = *second;
		stack.erase(--second.base());
		return ret;
	}

private:
	template <typename REQ, typename ENV> struct stack_getter {
		static inline std::vector<REQ>& get(ENV& e) {
			return static_cast<typename ENV::base_type&>(e).template get_stack<REQ>();
		}
	};
	
	template <typename ENV> struct stack_getter<typename ENV::value_type, ENV> {
		static inline std::vector<typename ENV::value_type>& get(ENV& e) {
			return e.this_stack;
		}
	};
	std::vector<new_type> this_stack;
};

template <typename T> struct wrap {};

template <typename Archive, typename Env>
struct stack_serializer {
	stack_serializer(Archive& ar, Env& e) : archive(ar), env(e) {}
	template <typename value_type>
	void operator()(wrap<value_type>&) {
		archive & env.template get_stack<value_type>();
	}
	Archive& archive;
	Env& env;
};

} // namespace detail

namespace mpl = ::boost::mpl;

template <typename types>
struct Env : public mpl::fold<types, detail::Env_null, detail::Env_cons<mpl::_1, mpl::_2> >::type {
private:
	friend class ::boost::serialization::access;
	template <typename ARCHIVE>
	void serialize(ARCHIVE& ar, const unsigned int) {
		detail::stack_serializer<ARCHIVE, Env<types> > ss(ar, *this);
		boost::mpl::for_each<types, typename detail::wrap<mpl::_1> >(ss);
	}
};

} // namespace vpush

#endif

