#ifndef __VPUSH_USER_TYPE_HPP__
#define __VPUSH_USER_TYPE_HPP__

#include <boost/serialization/export.hpp>

#include <vpush/detail/stack.hpp>

#define VPUSH_EXPORT(T, N)	BOOST_CLASS_EXPORT_GUID(::vpush::detail::stack<T>,N)

#endif // __VPUSH_USER_TYPE_HPP__

