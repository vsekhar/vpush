#ifndef __VPUSH_TYPEINFO_HPP_
#define __VPUSH_TYPEINFO_HPP_

#include <typeinfo>
#include <ostream>
#include <list>
#include <vector>

#include <boost/ref.hpp>

namespace vpush {
namespace util {

/* NB: We can't use a boost::reference_wrapper directly for TypeInfo because
 * it doesn't latch into ADL, meaning our comparison operators won't be found.
 * Doesn't work:
 * 	typedef boost::reference_wrapper<std::type_info const> TypeInfo;
 */
typedef boost::reference_wrapper<std::type_info const> TypeInfo_base;

struct TypeInfo : TypeInfo_base {
	template <class T> TypeInfo(const T& t) :
		TypeInfo_base(t) {}
};

bool operator<(const TypeInfo&, const std::type_info&);
bool operator==(const TypeInfo&, const std::type_info&);
std::ostream& operator<<(std::ostream&, const TypeInfo&);
std::size_t hash_value(const std::type_info &);

typedef std::list<TypeInfo> TypeInfoList;
typedef std::vector<TypeInfo> TypeInfoVector;

} // namespace util
} // namespace vpush

#endif /*__VPUSH_TYPEINFO_HPP_*/

