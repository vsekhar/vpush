#include <boost/numeric/ublas/vector.hpp>

namespace vpush {
namespace util {

typedef ::boost::numeric::ublas::bounded_vector<double,3> vector;

double magnitude(const vector&);
vector normalized(const vector&);
vector random_vector();

} // namespace util
} // namespace vpush

