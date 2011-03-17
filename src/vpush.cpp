#include <iostream>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <vpush/vpush.hpp>
#include <vpush/detail/toroidal.hpp>
#include <vpush/detail/functions.hpp>

using std::cout;
using std::endl;

using vpush::Protein;
using vpush::ExtendedProtein;
using vpush::get_stack;
using vpush::size;
using vpush::clear;
using vpush::pop;
using vpush::push;
using vpush::pop_second;
using vpush::push_second;
using vpush::detail::Code;
using vpush::detail::Exec;

double func(Protein& e) {
	pop<int>(e);
	return 1;
}

double func2(Protein& e) {
	push<int>(e, 81);
	return 1;
}

int main(int argc, char** argv) {
	ExtendedProtein e, e2;
	push<int>(e, 7);
	push<int>(e, 31);
	push<int>(e, 47);
	push<int>(e, 42);
	push<char>(e, 'c');
	push<std::string>(e, "hello world");
	
	using vpush::functions;
	functions.add("FUNC", func, vpush::type<int>());
	functions.add("FUNC2", func2);
	push<Code>(e, functions.get_fptr("FUNC"));

	{
		std::ofstream out("tmp");
		boost::archive::text_oarchive ar(out);
		ar & e;
	}
	
	vpush::print_stack<int>(e, cout) << endl;
	
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
	
	pop<Code>(e2).fptr(e2);
	cout << "Int stack size: " << size<int>(e2) << endl;
	cout << "Int: " << pop<int>(e2) << endl;
	
	return 0;
}

