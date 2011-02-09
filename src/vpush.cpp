#include <iostream>

#include <vpush/env.hpp>
#include <vpush/detail/functions.hpp>

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
	
	Env e;
	vpush::detail::functions_t functions;

	functions.add("my_func", my_func);
	functions.add("my_pusher", my_pusher);
	functions.add("my_popper", my_popper, type<int>());
	functions.add("my_adder", my_adder, type<int>() * 2);

	e.make_stack<int>();
	e.push_second(1);
	e.push_second(2);
	functions.run("my_adder", e);
	functions.run("my_popper", e);
	e.push_second(3);
	e.push_second(4);
	e.push_second(5);
	cout << "Stack: " << e.list_stack<int>() << endl;
	cout << "size: " << e.size<int>() << endl;
	cout << "Int: " << e.pop_second<int>() << endl;
	cout << "Int: " << e.pop<int>() << endl;
	cout << "Stack: " << e.list_stack<int>() << endl;
	cout << "size: " << e.size<int>() << endl;
	functions.run("my_func", e);
	

	return 0;
}

