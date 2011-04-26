#include "vpush/util/random.hpp"

namespace vpush {
namespace util {

using std::string;

default_generator_t default_generator;

const std::string RandomString::letters = "abcdefghijklmnopqrstuvwxyz";
const std::string RandomString::digits = "0123456789";

string RandomString::operator()(std::size_t len) {
	string ret;
	ret.reserve(len);
	std::size_t count = 0;
	for(; count < len; ++count)
		ret.push_back(choices[rint()]);
	return ret;
}

} // end namespace util
} // end namespace vpush

