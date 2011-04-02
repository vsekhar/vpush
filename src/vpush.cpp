#include <iostream>
#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <vpush/vpush.hpp>

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
using vpush::functions;
using vpush::soup;

namespace library = ::vpush::library;

double func(Protein& p) {
	pop<int>(p);
	return 1;
}

double func2(Protein& p) {
	push<int>(p, 81);
	return 1;
}

template <typename T>
struct pusher {
	pusher(T i) : _val(i) {}
	void operator()(ExtendedProtein& e) { push<T>(e, _val); }
	T _val;
};

int main(int argc, char** argv) {
	ExtendedProtein p;
	push<int>(p, 7);
	push<int>(p, 31);
	push<int>(p, 47);
	push<int>(p, 42);
	push_second<int>(p, 77);
	push<char>(p, 'c');
	push<std::string>(p, "hello world");
	
	library::initialize();
	functions.add("FUNC", func, vpush::type<int>());
	functions.add("FUNC2", func2);
	push<Code>(p, functions.get_fptr("FUNC"));

	cout << "Original:" << endl;
	cout << size<int>(p) << " ints: " << stack<int>(p) << endl;
	cout << size<char>(p) << " chars: " << stack<char>(p) << endl;
	cout << size<std::string>(p) << " strings: " << stack<std::string>(p) << endl;
	
	soup.push_back(p);
	{
		std::ofstream out("tmp");
		boost::archive::text_oarchive ar(out);
		ar & soup[0];
		ar & soup;
	}

	ExtendedProtein p2;
	{
		std::ifstream in("tmp");
		boost::archive::text_iarchive ar(in);
		ar & p2;
		ar & soup;
	}
	
	cout << endl << "New:" << endl;
	cout << size<int>(p2) << " ints: " << stack<int>(p2) << endl;
	cout << size<char>(p2) << " chars: " << stack<char>(p2) << endl;
	cout << size<std::string>(p2) << " strings: " << stack<std::string>(p2) << endl;

	cout << "Popping second int: " << pop_second<int>(p2) << endl;
	cout << size<int>(p2) << " ints: " << stack<int>(p2) << endl;
	cout << "Swapping first and third" << endl;
	vpush::swap<int>(p2, 0, 2);
	cout << size<int>(p2) << " ints: " << stack<int>(p2) << endl;
	cout << "Popping string: " << pop<std::string>(p2) << endl;
	cout << size<std::string>(p2) << " strings: " << stack<std::string>(p2) << endl;
	
	cout << "Running code..." << endl;
	pop<Code>(p2).fptr(p2);
	cout << size<int>(p2) << " ints: " << stack<int>(p2) << endl;
	
	return 0;
}

