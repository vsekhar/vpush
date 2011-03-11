#include <iostream>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <vpush/vpush.hpp>
#include <vpush/detail/toroidal.hpp>

using std::cout;
using std::endl;

using vpush::Env;
using vpush::ExtendedEnv;
using vpush::get_stack;
using vpush::size;
using vpush::clear;
using vpush::pop;
using vpush::push;
using vpush::pop_second;
using vpush::push_second;

unsigned int func(Env& e) {
	return pop<int>(e);
}

int main(int argc, char** argv) {
	ExtendedEnv e, e2;
	push<int>(e, 7);
	push<int>(e, 31);
	push<int>(e, 47);
	push<int>(e, 42);
	push<char>(e, 'c');
	push<std::string>(e, "hello world");
	
	{
		std::ofstream out("tmp");
		boost::archive::text_oarchive ar(out);
		ar & e;
	}
	
	cout << "Int stack size: " << size<int>(e) << endl;
	cout << "Char stack size: " << size<char>(e) << endl;
	cout << "String stack size: " << size<std::string>(e) << endl;
	
	{
		std::ifstream in("tmp");
		boost::archive::text_iarchive ar(in);
		ar & e2;
	}
	
	cout << "Int stack size: " << size<int>(e2) << endl;
	cout << "Int: " << pop<int>(e2) << endl;
	cout << "Int stack size: " << size<int>(e2) << endl;
	cout << "Char stack size: " << size<char>(e2) << endl;
	cout << "String stack size: " << size<std::string>(e2) << endl;
	cout << "String: " << pop<std::string>(e2) << endl;
	
	vpush::detail::toroidal_dimension x(3.7);
	cout << "Toroidal: " << (x+0.4).get() << endl;
	x += 19.2;
	cout << "Toroidal: " << x.get() << endl;
	x -= 6.95;
	cout << "Toroidal: " << x.get() << endl;
	if(vpush::detail::toroidal_dimension(3.7) < 0.9)
		cout << "toroidal(3.7) < toroidal(0.9): yes" << endl;
	else
		cout << "error in toroidal comparison";
	
	return 0;
}

