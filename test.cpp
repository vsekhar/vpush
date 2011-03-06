#include <iostream>

#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

struct base {
	virtual ~base() {}
	template <typename ARCHIVE>
	void serialize(ARCHIVE & ar, const unsigned int) {}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(base)

template <typename T>
struct derived : base {
	std::vector<T> _vec;
	template <typename ARCHIVE>
	void serialize(ARCHIVE & ar, const unsigned int) {
		ar & ::boost::serialization::base_object<base>(*this);
		ar & _vec;
	}
};

namespace boost { 


namespace serialization {

template <typename U>
struct guid_defined<derived<U> > : boost::mpl::true_ {};

#if 0
template <typename U>
inline const char * guid<derived<U> >() {
	return std::string("container") + "<" + guid<U> + ">";
}
#endif

}}



//BOOST_CLASS_EXPORT(derived<int>);

int main() {
	using boost::archive::text_oarchive;
	
	boost::archive::detail::
		instantiate_ptr_serialization((derived<int>*)0, 0,
			boost::archive::detail::adl_tag());
	
	text_oarchive to(std::cout);
	//	text_oarchive to2(to);
	derived<int> iv;
	iv._vec.push_back(1776);
	base* bptr = &iv;
	to << bptr;

	return 0;
}

