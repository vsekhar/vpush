#include <iostream>
#include <fstream>

#include <boost/mpl/vector.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <vpush/env.hpp>

int main(int argc, char** argv) {
	using std::cout;
	using std::cin;
	using std::endl;
	using namespace vpush;

	typedef boost::mpl::vector<int, char>::type my_types;
	Env<my_types> e;
	e.push<int>(7);
	e.push<int>(31);
	e.push<int>(47);
	e.push<int>(42);
	e.push<char>('c');
	
	{
		// Save the Env with 4 'ints' and 1 'char' in its stacks
		std::ofstream out("tmp");
		boost::archive::text_oarchive ar(out);
		ar & e;
	}
	
	cout << "Int stack size: " << e.size<int>() << endl;	// 4
	cout << "Char stack size: " << e.size<char>() << endl;	// 1
	
	Env<my_types> e2;
	{
		// Should load ba
		std::ifstream in("tmp");
		boost::archive::text_iarchive ar(in);
		ar & e2;
	}
	
	cout << "Int stack size: " << e2.size<int>() << endl;
	cout << "Int: " << e2.pop<int>() << endl;
	cout << "Int stack size: " << e2.size<int>() << endl;
	cout << "Char stack size: " << e2.size<char>() << endl;
	
	return 0;
}

