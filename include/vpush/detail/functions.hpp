#ifndef __VPUSH_DETAIL_FUNCTIONS_HPP__
#define __VPUSH_DETAIL_FUNCTIONS_HPP__

#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>

#include <vpush/protein_fwd.hpp>
#include <vpush/detail/types.hpp>
#include <vpush/detail/code_fwd.hpp>

namespace vpush {
namespace detail {

struct function_entry {
	function_entry(const std::string&, op_func_t, const type_container&);
	std::string name;
	op_func_t func;
	type_container func_types;
private:
	function_entry();
};

struct bySeq;
struct byRAC;
struct byName;
struct byFptr;

using namespace ::boost::multi_index;

typedef multi_index_container <
	function_entry,
	indexed_by<
		sequenced<tag<bySeq> >,
		random_access<tag<byRAC> >,
		hashed_unique<
			tag<byName>,
			member<function_entry, std::string, &function_entry::name>
		>,
		hashed_unique<
			tag<byFptr>,
			member<function_entry, op_func_t, &function_entry::func>
		>
	>
> functions_container;

class functions_t {
public:
	void add(const std::string&, op_func_t);
	void add(const std::string&, op_func_t, const type_container&);
	op_func_t get_fptr(const std::string&) const;
	type_container get_types(const op_func_t&) const;
	std::string get_name(const op_func_t&) const;
	Exec get_code(const std::string&) const;
	Exec get_random() const;
	bool contains(const std::string&) const;
	bool contains(op_func_t) const;

	bool is_superset_of(const functions_t&) const;

private:
	typedef const functions_container::index<bySeq>::type functions_by_seq;
	typedef const functions_container::index<byRAC>::type functions_by_rac;
	typedef const functions_container::index<byName>::type functions_by_name;
	typedef const functions_container::index<byFptr>::type functions_by_fptr;

	typedef functions_container::index<byName>::type functions_by_name_mutable;
	functions_container _container;
};

} // namespace detail

extern detail::functions_t functions;

} // namespace vpush

#endif // __VPUSH_DETAIL_FUNCTIONS_HPP__

