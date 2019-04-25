#include <iostream> 	// std::cout
#include <stdlib.h>		// atoi 
#include <chrono>		// timer
#include <vector>		// std:: vector
#include <map>			// std::map
#include "binarytree.h"
#include "iterator.h"

/*! \brief A binary tree class to store data.
 *
 *  The class is templated in both the Key and the Value.
 *	At the moment it only handles key types which have operations: ==, <, > defined.
 */



void ingenua(const BinaryTree<int,int> &tree)
{
	std::cout << "calling Ingenua!" << std::endl;
	for (auto iter = tree.begin() ; iter!=tree.end() ; ++iter)
	{
		std::cout << (*iter).first << std::endl;
		// (*iter).second = 22;


	}

}


int main(int argc, char const *argv[])
{


	BinaryTree<int, int> test1;
	BinaryTree<int, int> test2;

	std::vector<int> keys_1{8, 3, 10, 6, 7, 1, 4, 14, 13};	
	for (auto x: keys_1) {test1.insert(std::pair<int,int>(x,1));}
	// Random tree
	for (int i = 0; i < 7; ++i)
	{
		test2.insert(std::pair<int,int>(rand()%100,1));
	}


	std::cout << test1 << std::endl;
	std::cout << test2 << std::endl;

	ingenua(test1);
	std::cout << test1 << std::endl;

	return 0;
}