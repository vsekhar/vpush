#include <iostream>
#include <list>
#include <vector>
#include <string>

#include <boost/mpl/vector.hpp>

#include <vpush/env.hpp>

int my_func(vpush::Env& e) {return 0;}
int my_pusher(vpush::Env& e) {
	e.push<int>(42);
	return 0;
}

int my_popper(vpush::Env& e) {
	int i = e.pop<int>();
	std::cout << "Popper! " << i << std::endl;
	return 0;
}

int my_adder(vpush::Env& e) {
	e.push<int>(e.pop<int>() + e.pop<int>());
	return 0;
}

int main(int argc, char** argv) {
	using std::cout;
	using std::cin;
	using std::endl;
	using namespace vpush;
	using vpush::type;
	namespace mpl = ::boost::mpl;
	
	Env e;

	e.register_("my_func", my_func);
	e.register_("my_pusher", my_pusher);
	e.register_("my_popper", my_popper, type<int>());
	e.register_("my_adder", my_adder, type<int>() + type<int>());

	e.make_stack<int>();
	e.push_second(1);
	e.push_second(2);
	e.run("my_adder");
	e.run("my_popper");
	e.push_second(3);
	e.push_second(4);
	e.push_second(5);
	cout << "Stack: " << e.list_stack<int>() << endl;
	cout << "size: " << e.size<int>() << endl;
	cout << "Int: " << e.pop_second<int>() << endl;
	cout << "Int: " << e.pop<int>() << endl;
	cout << "Stack: " << e.list_stack<int>() << endl;
	cout << "size: " << e.size<int>() << endl;
	e.run("my_func");
	//e.run("my_pusher");
	

	return 0;
}

