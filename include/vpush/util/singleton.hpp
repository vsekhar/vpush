//
// Singleton class
//  Contained type must be non-throw default constructbile and non-throw
//	destructible
//

// (thinned down version of boost/pool/detail/singleton.hpp)


#ifndef __VPUSH_SINGLETON_HPP__
#define __VPUSH_SINGLETON_HPP__

#include <typeinfo>

namespace vpush {
namespace util {

template <typename T>
struct singleton {
private:
	struct object_creator {
		// Ensure instance() is called before main() to prevent race issues
		// in multi-threaded programs
		object_creator() { singleton<T>::instance(); }
		inline void do_nothing() const {}
	};
	static object_creator create_object;

public:
	typedef T object_type;
	static object_type & instance() {
		static object_type obj;
		create_object.do_nothing(); // ensure instantiation, optimized away
		return obj;
	}
};

template <typename T>
typename singleton<T>::object_creator
singleton<T>::create_object;

} // namespace util
} // namespace vpush

#endif // __VPUSH_SINGLETON_HPP__

