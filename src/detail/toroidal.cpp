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

} // namespace detail
} // namespace vpush

