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

Protein random_protein(std::size_t s) {
	Protein ret;
	for(std::size_t i = 0; i < s; ++i)
		push<Exec>(ret, functions.get_random());
	return ret;
}

int main(int argc, char** argv) {
	library::initialize();

	Protein p = random_protein(1000);
	p.energy = 2000;
	push<int>(p, 7);
	push<int>(p, 31);
	push<int>(p, 4);
	push<int>(p, 8);

	push<Code>(p, Code(Code::CLOSE));
	push<Code>(p, functions.get_code("RANDOM.CODE"));
	push<Code>(p, Code(Code::CLOSE));
	push<Code>(p, functions.get_code("MAKELIST.CODE"));
	push<Code>(p, Code(Code::OPEN));
	push<Code>(p, Code(Code::OPEN));
	push<Exec>(p, functions.get_code("QUOTE.EXEC"));
	push<Exec>(p, functions.get_code("DUP.CODE"));

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
		try {
			vpush::run_protein(run);
		}
		catch(...) {
			vpush::print_trace(p2, cout);
			throw;
		}
	}
	
	return 0;
}

