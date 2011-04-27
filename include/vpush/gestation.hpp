#include <boost/ptr_container/ptr_vector.hpp>

#include <vpush/soup.hpp>

namespace vpush {

extern Protein* gestator;
typedef ::boost::ptr_vector<Protein> incubator_t;
extern incubator_t incubator;

void ensure_gestator(const Protein&);
bool detach_gestator();
void clear_gestator();
void clear_incubator();
void flush_incubator();

} // namespace vpush

