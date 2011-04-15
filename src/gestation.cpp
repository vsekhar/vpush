#include <vpush/gestation.hpp>

namespace vpush {

Protein* gestator;

void ensure_gestator() {
	if(!gestator)
		gestator = new Protein();
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

