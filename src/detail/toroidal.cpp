#include <vpush/detail/toroidal.hpp>

#include <cmath>

namespace vpush {
namespace detail {

inline double modf(double v) {
	double dummy;
	v = std::modf(v, &dummy);
	if(v < 0) v += 1;
	return v;
}

toroidal_dimension& toroidal_dimension::operator+=(const toroidal_dimension& rhs) {
	value += rhs.value;
	value = modf(value);
}

toroidal_dimension& toroidal_dimension::operator-=(const toroidal_dimension& rhs) {
	value -= rhs.value;
	value = modf(value);
}

toroidal_dimension& toroidal_dimension::operator*=(const toroidal_dimension& rhs) {
	value *= rhs.value;
	value = modf(value);
}

toroidal_dimension& toroidal_dimension::operator/=(const toroidal_dimension& rhs) {
	value /= rhs.value;
	value = modf(value);
}

toroidal_dimension& toroidal_dimension::operator*=(double d) {
	value *= d;
	value = modf(value);
}

toroidal_dimension& toroidal_dimension::operator/=(double d) {
	value /= d;
	value = modf(value);
}

toroidal_dimension operator+(const toroidal_dimension& lhs, const toroidal_dimension& rhs) {
	return toroidal_dimension(lhs) += rhs;
}

toroidal_dimension operator-(const toroidal_dimension& lhs, const toroidal_dimension& rhs) {
	return toroidal_dimension(lhs) -= rhs;
}

toroidal_dimension operator*(const toroidal_dimension& lhs, const toroidal_dimension& rhs) {
	return toroidal_dimension(lhs) *= rhs;
}

toroidal_dimension operator/(const toroidal_dimension& lhs, const toroidal_dimension& rhs) {
	return toroidal_dimension(lhs) /= rhs;
}

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

