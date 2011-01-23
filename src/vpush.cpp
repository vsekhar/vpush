#include <iostream>
#include <list>
#include <vector>
#include <string>

#include <vpush/register.hpp>
#include <vpush/stacks.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>

struct badtype {
	badtype(int a) : _i(a) {}
private:
	badtype();
	int _i;
	friend std::ostream& operator<<(std::ostream&, const badtype&);
};

std::ostream& operator<<(std::ostream& o, const badtype& b) {
	o << b._i;
	return o;
}

int my_func(int i, int j) { return i+j; }
badtype badfunc(badtype) { return badtype(1); }
int inc(int i) { return i+1; }

template <typename T>
void print_stack() {
	while(!vpush::empty<T>())
		std::cout << "value: " << vpush::pop<T>() << std::endl;
}

int main(int argc, char** argv) {
	using namespace vpush;
	
	register_(my_func, "my_func");
	register_(badfunc, "bad_func");
	register_(inc, "inc");
	std::cout << "Stacks count: " << stacks().size() << std::endl;

	std::cout << "badtype stack:" << std::endl;
	print_stack<badtype>();
	push(badtype(7));
	push(badtype(9));
	codes["bad_func"]->exec();
	std::cout << "badtype stack:" << std::endl;
	print_stack<badtype>();

	return 0;
}

