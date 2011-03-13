#ifndef __VPUSH_DETAIL_TOROIDAL_HPP__
#define __VPUSH_DETAIL_TOROIDAL_HPP__

#include <utility>	// for remaining comparison operators
#include <ostream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace vpush {
namespace detail {

double modf(double);

struct toroidal_dimension {
	toroidal_dimension() : value() {}
	toroidal_dimension(const toroidal_dimension& t) : value(t.value) {}
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
	toroidal_dimension& operator*=(double);
	toroidal_dimension& operator/=(double);
	
private:
	friend std::ostream& operator<<(std::ostream&, const toroidal_dimension&);
	double value;
};

std::ostream& operator<<(std::ostream&, const toroidal_dimension&);

toroidal_dimension operator+(const toroidal_dimension&, const toroidal_dimension&);
toroidal_dimension operator-(const toroidal_dimension&, const toroidal_dimension&);
toroidal_dimension operator*(const toroidal_dimension&, const toroidal_dimension&);
toroidal_dimension operator/(const toroidal_dimension&, const toroidal_dimension&);

toroidal_dimension operator*(const toroidal_dimension&, double);
toroidal_dimension operator/(const toroidal_dimension&, double);

toroidal_dimension operator*(double, const toroidal_dimension&);

typedef ::boost::numeric::ublas::bounded_vector<toroidal_dimension, 3> toroidal_vector;

} // namespace detail
} // namespace vpush

#endif // __VPUSH_DETAIL_TOROIDAL_HPP__

