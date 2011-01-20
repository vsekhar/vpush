#include <iostream>
#include <list>
#include <vector>
#include <string>

#include <vpush/register.hpp>
#include <vpush/stacks.hpp>

struct badtype {
	badtype(int) {}
private:
	badtype();
};

int my_func(int i, int j) { return i+j; }
badtype badfunc() { return badtype(1); }

int main(int argc, char** argv) {
	using vpush::push;
	using vpush::pop;
	using vpush::stacks;
	vpush::register_(my_func, "my_func");
	vpush::register_(badfunc, "bad_func");
	std::cout << "Stacks count: " << stacks().size() << std::endl;
	push(5);
	std::cout << "Int stack: " << pop<int>() << std::endl;
	return 0;
}

