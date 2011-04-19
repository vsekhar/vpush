#include <vpush/util/vector.hpp>
#include <vpush/util/random.hpp>

namespace vpush {
namespace util {

double magnitude(const vector& v) { return ::boost::numeric::ublas::norm_2(v); }
vector normalized(const vector& v) { return v / magnitude(v); }
vector random_vector() {
	vector v;
	Random_01 rand;
	v[0] = rand();
	v[1] = rand();
	v[2] = rand();
	return v;
}

} // namespace util
} // namespace vpush

