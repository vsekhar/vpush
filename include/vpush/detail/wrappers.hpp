#ifndef __VPUSH_WRAPPERS_HPP__
#define __VPUSH_WRAPPERS_HPP__


namespace vpush {
namespace detail {

template <typename T> struct type_wrapper {type_wrapper() {}};

template <typename T>
struct wrap {	typedef type_wrapper<T> type; };

} // namespace detail
} // namespace vpush

#endif

