#include <iostream>

void test_func(const int * pi)
{
	*pi = 42;
	return;
}
int main()
{
	int i = 0;
	int *pi = &i;
	test_func(pi);
	std::cout << *pi << std::endl;
	return 0;
}