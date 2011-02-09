#ifndef __VPUSH_TYPEINFO_HPP_
#define __VPUSH_TYPEINFO_HPP_

#include <typeinfo>
#include <ostream>
#include <list>
#include <vector>

#include <boost/ref.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

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
private:
	friend class ::boost::serialization::access;

};

bool operator<(const TypeInfo&, const std::type_info&);
bool operator==(const TypeInfo&, const std::type_info&);
std::ostream& operator<<(std::ostream&, const TypeInfo&);
std::size_t hash_value(const std::type_info &);

typedef std::list<TypeInfo> TypeInfoList;
typedef std::vector<TypeInfo> TypeInfoVector;

/** Functor to be used with mpl::for_each to transfer types from mpl containers
 * into runtime containers.
 */
struct TypeInfoInserter {
	TypeInfoInserter(util::TypeInfoVector &v) : vector(v) {}
	template <class TOINSERT>
	void operator()(TOINSERT) {
		vector.push_back(util::TypeInfo(typeid(TOINSERT)));
	}
	util::TypeInfoVector& vector;
};

struct serializable_wrapper_base {};

template <typename T>
struct serializable_wrapper : serializable_wrapper_base {
	template <typename ARCHIVE>
	void serialize(ARCHIVE& ar, const unsigned int) {
		
	}
};

} // namespace util
} // namespace vpush

namespace boost {
namespace serialization {

template <typename ARCHIVE>
inline void save_construct_data(ARCHIVE & ar, const vpush::util::TypeInfo* t, const unsigned int) {
	
}

} // namespace serialization
} // namespace boost

#endif /*__VPUSH_TYPEINFO_HPP_*/

