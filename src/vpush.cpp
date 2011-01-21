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
int inc(int i) { return i+1; }

int main(int argc, char** argv) {
	using vpush::push;
	using vpush::pop;
	using vpush::stacks;
	using vpush::register_;
	using vpush::codes;
	using vpush::detail::type_checker;
	
	register_(my_func, "my_func");
	//register_(badfunc, "bad_func");
	register_(inc, "inc");
	std::cout << "Stacks count: " << stacks().size() << std::endl;
	push(5);
	push(6);
	std::cout << "Int stack: " << top<int>() << std::endl;
	std::cout << "Int stack: " << second<int>() << std::endl;
	codes["inc"]->check();
	codes["inc"]->exec();
	std::cout << "inc: " << pop<int>() << std::endl;
	return 0;
}

