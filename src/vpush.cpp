#include <iostream>
#include <list>
#include <vector>
#include <string>

#include <vpush/env.hpp>

int my_func(vpush::Env& e) {return 0;}

int main(int argc, char** argv) {
	using std::cout;
	using std::cin;
	using std::endl;
	using namespace vpush;
	
	Env e;
	e.make_stack<int>();
	e.push_second(1);
	e.push_second(2);
	e.push_second(3);
	e.push_second(4);
	e.push_second(5);
	cout << "Stack: " << e.list_stack<int>() << endl;
	cout << "size: " << e.size<int>() << endl;
	cout << "Int: " << e.pop_second<int>() << endl;
	cout << "Int: " << e.pop<int>() << endl;
	cout << "Stack: " << e.list_stack<int>() << endl;
	cout << "size: " << e.size<int>() << endl;
	e.register_("my_func", my_func);

	return 0;
}

