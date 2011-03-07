#include <iostream>
#include <fstream>
#include <string>

#include <boost/mpl/vector.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <vpush/env.hpp>

// Template for registering stack_manipulating functions
template <class ENV>
struct my_stack_funcs {
	static unsigned int stack_func(ENV& e) {
		e.template pop<int>();
		return 0;
	}
	
	static void register_(ENV& e) {
		e.register_(stack_func, "stack_func");
	}
};

// Simple function
unsigned int func(int i) {
	return 0;
}

int main(int argc, char** argv) {
	using std::cout;
	using std::cin;
	using std::endl;
	using namespace vpush;

	typedef boost::mpl::vector<int, std::string, char>::type my_types;
	typedef Env<my_types> MyEnv;
	MyEnv e;
	e.push<int>(7);
	e.push<int>(31);
	e.push<int>(47);
	e.push<int>(42);
	e.push<char>('c');
	e.push<std::string>("hello world");
	e.multi_reg<my_stack_funcs>();
	
	{
		std::ofstream out("tmp");
		boost::archive::text_oarchive ar(out);
		ar & e;
	}
	
	cout << "Int stack size: " << e.size<int>() << endl;
	cout << "Char stack size: " << e.size<char>() << endl;
	cout << "String stack size: " << e.size<std::string>() << endl;
	
	MyEnv e2;
	{
		std::ifstream in("tmp");
		boost::archive::text_iarchive ar(in);
		ar & e2;
	}
	
	cout << "Int stack size: " << e2.size<int>() << endl;
	cout << "Int: " << e2.pop<int>() << endl;
	cout << "Int stack size: " << e2.size<int>() << endl;
	cout << "Char stack size: " << e2.size<char>() << endl;
	cout << "String stack size: " << e2.size<std::string>() << endl;
	cout << "String: " << e2.pop<std::string>() << endl;
	
	return 0;
}

