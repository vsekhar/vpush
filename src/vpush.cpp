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
using vpush::stack;
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
	push_second<int>(e, 77);
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

	cout << "Original:" << endl;
	cout << size<int>(e) << " ints: " << stack<int>(e) << endl;
	cout << size<char>(e) << " chars: " << stack<char>(e) << endl;
	cout << size<std::string>(e) << " strings: " << stack<std::string>(e) << endl;
	
	{
		std::ifstream in("tmp");
		boost::archive::text_iarchive ar(in);
		ar & e2;
	}
	
	cout << endl << "New:" << endl;
	cout << size<int>(e2) << " ints: " << stack<int>(e2) << endl;
	cout << size<char>(e2) << " chars: " << stack<char>(e2) << endl;
	cout << size<std::string>(e2) << " strings: " << stack<std::string>(e2) << endl;

	cout << "Popping second int: " << pop_second<int>(e2) << endl;
	cout << size<int>(e2) << " ints: " << stack<int>(e2) << endl;
	cout << "Swapping first and third" << endl;
	vpush::swap<int>(e2, 0, 2);
	cout << size<int>(e2) << " ints: " << stack<int>(e2) << endl;
	cout << "Popping string: " << pop<std::string>(e2) << endl;
	cout << size<std::string>(e2) << " strings: " << stack<std::string>(e2) << endl;
	
	cout << "Running code..." << endl;
	pop<Code>(e2).fptr(e2);
	cout << size<int>(e2) << " ints: " << stack<int>(e2) << endl;
	
	return 0;
}

