#include <iostream>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <boost/archive/text_oarchive.hpp>

//////////////////////////////////////////////////////////////////////////////
// Example code that would reside in the library
//////////////////////////////////////////////////////////////////////////////

struct type_container_base {
private:
	virtual void make_abstract() const {}
	friend class ::boost::serialization::access;
	template <typename ARCHIVE>
	void serialize(ARCHIVE &, const unsigned int) {}	
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(type_container_base)

template <typename USER_TYPE>
struct type_container : type_container_base {
	void add(const USER_TYPE& d) { _vector.push_back(d); }
private:
	std::vector<USER_TYPE> _vector;
	friend class ::boost::serialization::access;
	template <typename ARCHIVE>
	void serialize(ARCHIVE & ar, const unsigned int) {
		ar & ::boost::serialization::base_object<type_container_base>(*this);
		ar & _vector;
	}
};

//////////////////////////////////////////////////////////////////////////////
// Example user code that would use the library
//////////////////////////////////////////////////////////////////////////////

struct user_type {
	user_type(int i) : _val(i) {}
private:
	int _val;
	friend class ::boost::serialization::access;
	template <typename ARCHIVE>
	void serialize(ARCHIVE & ar, const unsigned int) {
		ar & _val;
	}
};

// *** Is there a better way than forcing the user to do this for every
// *** user_type they want to use with the library?
BOOST_CLASS_EXPORT_GUID(type_container<user_type>, "type_container<user_type>")

int main() {
	std::vector<type_container_base*> containers;
	type_container<user_type>* tc = new type_container<user_type>();
	tc->add(user_type(7));
	tc->add(user_type(42));
	tc->add(user_type(1776));
	containers.push_back(tc);
	{
		boost::archive::text_oarchive ar(std::cout);
		const std::size_t size = containers.size();
		ar << size;
		BOOST_FOREACH(type_container_base* p, containers)
			ar << p;
	}
	return 0;
}

