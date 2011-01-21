#include <iostream>
#include <list>
#include <vector>
#include <string>

#include <vpush/register.hpp>
#include <vpush/stacks.hpp>

struct badtype {
	badtype(int) {}
//private:
	badtype();
};

int my_func(int i, int j) { return i+j; }
badtype badfunc() { return badtype(1); }
int add_1(int i) { return i+1; }

int main(int argc, char** argv) {
	using vpush::stack;
	using vpush::stacks;
	using vpush::register_;
	using vpush::codes;
	using vpush::detail::type_checker;
	
	register_(my_func, "my_func");
	//register_(badfunc, "bad_func");
	register_(add_1, "add_1");
	std::cout << "Stacks count: " << stacks().size() << std::endl;
	stack<int>().push(5);
	std::cout << "Int stack: " << stack<int>().pop() << std::endl;
	stack<int>().push(5);
	codes["add_1"]->check();
	codes["add_1"]->exec();
	std::cout << "add_1: " << stack<int>().pop() << std::endl;
	return 0;
}

