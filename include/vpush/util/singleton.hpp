//
// Singleton class (Boost Pool's implementation)
//  Contained type must be non-throw default constructbile and non-throw
//	destructible
//

#ifndef __VPUSH_SINGLETON_HPP__
#define __VPUSH_SINGLETON_HPP__

#include <boost/pool/detail/singleton.hpp>

namespace vpush {
namespace util {

template <typename T>
struct singleton : public boost::details::pool::singleton_default<T> {};

} // namespace util
} // namespace vpush

#endif // __VPUSH_SINGLETON_HPP__


