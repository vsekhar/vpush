#ifndef __VPUSH_DETAIL_TOROIDAL_HPP__
#define __VPUSH_DETAIL_TOROIDAL_HPP__

#include <utility>

namespace vpush {
namespace detail {

double modf(double);

struct toroidal_dimension {
	toroidal_dimension(double d) : value(modf(d)) {}
	inline double get() const { return value; }

	// don't want type coercion as it leads to unusual expressions:
	//   e.g. 3.7 < toroidal_dimension(0.9)
	bool operator==(const toroidal_dimension& rhs) const { return value == rhs.value; }
	bool operator<(const toroidal_dimension& rhs) const { return value < rhs.value; }

	toroidal_dimension& operator+=(const toroidal_dimension&);
	toroidal_dimension& operator-=(const toroidal_dimension&);
	toroidal_dimension& operator*=(const toroidal_dimension&);
	toroidal_dimension& operator/=(const toroidal_dimension&);
	
private:
	double value;
};

toroidal_dimension operator+(const toroidal_dimension&, const toroidal_dimension&);
toroidal_dimension operator-(const toroidal_dimension&, const toroidal_dimension&);
toroidal_dimension operator*(const toroidal_dimension&, const toroidal_dimension&);
toroidal_dimension operator/(const toroidal_dimension&, const toroidal_dimension&);

} // namespace detail
} // namespace vpush

#endif // __VPUSH_DETAIL_TOROIDAL_HPP__

