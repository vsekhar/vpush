#ifndef __VPUSH_RANDOM_HPP__
#define __VPUSH_RANDOM_HPP__

#include <ctime>	// for std::time

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

namespace vpush {
namespace util {

template <class GENERATOR, unsigned int seed>
struct SeededGenerator : GENERATOR {
	SeededGenerator() : GENERATOR(seed) {};
};

template <class GENERATOR>
struct TimeSeededGenerator : GENERATOR {
	TimeSeededGenerator() : GENERATOR(static_cast<unsigned int>(std::time(0))) {};
};

typedef TimeSeededGenerator<boost::mt19937> time_seeded_mersenne_t;
typedef time_seeded_mersenne_t default_generator_t;
extern default_generator_t default_generator;

typedef boost::uniform_01<time_seeded_mersenne_t> RandomBool_base;
struct RandomBool : RandomBool_base {
	RandomBool() : RandomBool_base(default_generator) {}
	bool operator()() {
		return RandomBool_base::operator()() < 0.5;
	}
	bool operator()(double probability_of_true) {
		return RandomBool_base::operator()() < probability_of_true;
	}
};

} // namespace util
} // namespace vpush

#endif /*__VPUSH_RANDOM_HPP__*/
