#include <boost/foreach.hpp>

#include <vpush/gestation.hpp>

namespace vpush {

Protein* gestator;
::boost::ptr_vector<Protein> incubator;

void ensure_gestator(const Protein& p) {
	if(!gestator) {
		gestator = new Protein();
		gestator->x = p.x;
		gestator->y = p.y;
		gestator->z = p.z;
		gestator->facing = p.facing;
	}
}

bool detach_gestator() {
	if(gestator) {
		incubator.push_back(gestator);
		gestator = NULL;
		return true;
	}
	else
		return false;
}

void release_incubator() {
	BOOST_FOREACH(const Protein& p, incubator)
		soup.push_back(p);
	incubator.clear();
}

} // namespace vpush

