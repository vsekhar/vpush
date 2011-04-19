#include <cmath>

#include <boost/preprocessor/cat.hpp>

#include <vpush/util/toroidal.hpp>

namespace vpush {
namespace detail {

double modf(double v) {
	double dummy;
	v = std::modf(v, &dummy);
	if(v < 0) v += 1;
	return v;
}

toroidal_dimension& toroidal_dimension::operator=(double d) {
	value = modf(d);
	return *this;
}

#define TD_OP(op) toroidal_dimension& toroidal_dimension::operator op \
	(const toroidal_dimension& rhs) {value=modf(value op rhs.value); return *this;}

TD_OP(+=)
TD_OP(-=)
TD_OP(*=)
TD_OP(/=)

#define TD_OP_DBL(op) toroidal_dimension& toroidal_dimension::operator op \
	(double d) {value=modf(value op d); return *this;}

TD_OP_DBL(*=)
TD_OP_DBL(/=)

#define TD_OP_BINARY(op) toroidal_dimension operator op \
	(const toroidal_dimension& lhs, const toroidal_dimension& rhs) \
	{ return toroidal_dimension(lhs) BOOST_PP_CAT(op, =) rhs; }

TD_OP_BINARY(+)
TD_OP_BINARY(-)
TD_OP_BINARY(*)
TD_OP_BINARY(/)

toroidal_dimension operator*(double d, const toroidal_dimension& rhs) {
	return toroidal_dimension(rhs) *= d;
}

toroidal_dimension operator*(const toroidal_dimension& lhs, double d) {
	return toroidal_dimension(lhs) *= d;
}

toroidal_dimension operator/(const toroidal_dimension& lhs, double d) {
	return toroidal_dimension(lhs) /= d;
}

std::ostream& operator<<(std::ostream& o, const toroidal_dimension& t) {
	return o << t.value;
}

} // namespace detail
} // namespace vpush

