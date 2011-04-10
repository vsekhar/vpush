#include <iostream>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <vpush/vpush.hpp>

using std::cout;
using std::endl;

using vpush::Protein;
using vpush::Protein;
using vpush::stack;
using vpush::size;
using vpush::clear;
using vpush::pop;
using vpush::push;
using vpush::pop_second;
using vpush::push_second;
using vpush::detail::Code;
using vpush::detail::Exec;
using vpush::functions;
using vpush::soup;

namespace library = ::vpush::library;

int main(int argc, char** argv) {
	Protein p;
	push<int>(p, 7);
	push<int>(p, 31);
	push<int>(p, 0);
	push<int>(p, 42);
	push_second<int>(p, 77);
	push<std::string>(p, "hello world");
	
	library::initialize();
	for(unsigned int i=0; i < 1000; ++i) {
		Exec e = functions.get_random();
		push<Exec>(p, e);
	}
	p.energy = 500;
	push<int>(p, 4);
	push<int>(p, 8);
	push<Exec>(p, functions.get_code("DO_RANGE.EXEC"));

	soup.push_back(p);
	{
		std::ofstream out("tmp");
		boost::archive::text_oarchive ar(out);
		ar & soup[0];
		ar & soup;
	}

	Protein p2;
	{
		std::ifstream in("tmp");
		boost::archive::text_iarchive ar(in);
		ar & p2;
		ar & soup;
	}
	
	for(unsigned int i = 0; i < 100; ++i) {
		Protein run(p2);
		vpush::run_protein(run);
	}
	
	return 0;
}

