#include <vpush/gestation.hpp>

namespace vpush {

Protein* gestator;

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
		soup.push_back(*gestator);
		delete gestator;
		gestator = NULL;
		return true;
	}
	else
		return false;
}

} // namespace vpush

