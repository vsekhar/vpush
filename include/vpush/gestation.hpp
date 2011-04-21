#include <boost/ptr_container/ptr_vector.hpp>

#include <vpush/soup.hpp>

namespace vpush {

extern Protein* gestator;
extern ::boost::ptr_vector<Protein> incubator;

void ensure_gestator(const Protein&);
bool detach_gestator();
void release_incubator();

} // namespace vpush

