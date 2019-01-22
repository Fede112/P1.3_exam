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


int main(int argc, char const *argv[])
{

	/////////////////////////////////////////////////////////////////
	// TEST
	#ifdef TEST

	if(argc != 1)
	{
		printf("Test RUN for %s, no arguments needed for \n", argv[0]);
		exit(1);
	}

	BinaryTree<int, int> test1;
	BinaryTree<int, int> test2;

	std::vector<int> keys_1{8, 3, 10, 6, 7, 1, 4, 14, 13};	
	for (auto x: keys_1) {test1.insert(std::pair<int,int>(x,1));}
	// Random tree
	for (int i = 0; i < 7; ++i)
	{
		test2.insert(std::pair<int,int>(rand()%100,1));
	}

	/////////////////////////////////////////////////////////////////
	// TEST operator[]
	std::cout << "\nBegin operator[] test ...\n" << std::endl;
	std::cout << test1;
	test1[21];
	std::cout << test1;
	std::cout << "\nCompleted operator[] test.\n" << std::endl;

	/////////////////////////////////////////////////////////////////
	// COPY AND MOVE SEMANTICS TEST
	std::cout << "\nBegin copy and move semantics tests ...\n" << std::endl;

	BinaryTree<int, int> test_copy_asg;
	BinaryTree<int, int> test_move_asg;

	BinaryTree<int, int> test_copy_ctr{test1};
	std::cout << test_copy_ctr;
	test_copy_asg = test1;
	std::cout << test_copy_asg;
	BinaryTree<int, int> test_move_ctr = std::move(test1);
	std::cout << test_move_ctr;
	test_move_asg = test_move_ctr;
	std::cout << test_move_asg;	
	std::cout << "\nCompleted copy and move semantics tests.\n" << std::endl;
	/////////////////////////////////////////////////////////////////
	

	/////////////////////////////////////////////////////////////////
	std::cout << "\nBegin Iterators tests ...\n" << std::endl;
	// ITERATORS TEST
	// trying constIterator threw operator<<(std::ostream& os, const BinaryTree<TK, TV>& tree)
	std::cout << test2;

	// trying Iterators
	auto it = test2.begin();
	auto stop = test2.end();
	for(; it!=stop; ++it)
	{
		std::cout << (*it).first << " ";
	  	(*it).second = 2;
	}
	std::cout << std::endl;
	std::cout << test2;

	// trying cbegin and cend
	auto c_it = test2.cbegin();
	auto c_stop = test2.cend();
	for(; c_it!=c_stop; ++c_it)
	{
		std::cout << (*c_it).first << " ";
	//   (*c_it) = std::pair<int,int>(1,1);  // Error because it uses cbegin!
	}
	std::cout << "\nCompleted Iterators tests\n" << std::endl;
	/////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////
	// FIND TEST
	std::cout << "\nBegin find tests ...\n" << std::endl;
	BinaryTree<int, int> tree_ft;
	std::vector<int> keys_ft{8, 3, 10, 6, 7, 1, 4, 14, 13};	
	for (auto x: keys_ft) {tree_ft.insert(std::pair<int,int>(x,1));}
	
	// look for two keys
	auto look1 = tree_ft.find(4);
	auto look2 = tree_ft.find(5);
	auto find_stop = tree_ft.end();

	std::cout << "Looking for 4: ";
	std::cout << (look1 != find_stop) << std::endl;
	std::cout << "Looking for 5: " ;
	std::cout << (look2 != find_stop) << std::endl;
	std::cout << "\nCompleted find tests.\n" << std::endl;
		
	/////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////
	// BALANCE TEST
	std::cout << "\nBegin balance test ...\n" << std::endl;
	BinaryTree<int, int> unbalancedTree;
	BinaryTree<int, int> balancedTree;
	
	// create unbalance tree
	std::vector<int> keys_ub{1,2,3,4,5,6,7,8,9};	
	for (auto x: keys_ub) {unbalancedTree.insert(std::pair<int,int>(x,1));}
	
	unbalancedTree.balance(balancedTree, unbalancedTree.cbegin(), unbalancedTree.checkSize());
	std::cout << "\nCompleted balance tests.\n" << std::endl;
		
	/////////////////////////////////////////////////////////////////

	#endif		
	

	// /////////////////////////////////////////////////////////////////
	// BENCHMARK

	#ifdef BENCHMARK

	if(argc != 3)
	{
		printf("Wrong number of arguments. ./main.x [Tree Size] [Repetitions]\n");
		exit(1);
	}

	const std::size_t N = atoi(argv[1]);
	const std::size_t repetitions = atoi(argv[2]);

	time_t t;
	srand((unsigned) time(&t));

	using namespace std::chrono;
	using time_point = high_resolution_clock::time_point;


	BinaryTree<int, int> nonBalanced_bigTree, balanced_bigTree;
	std::map<int,int> std_map;


	std::vector<int> aLotOfKeys;	
	for (std::size_t i=0; i<N; ++i) {aLotOfKeys.push_back(N-i);}
	for (auto x: aLotOfKeys) { nonBalanced_bigTree.insert(std::pair<int,int>(x,1)); std_map.insert( std::pair<int,int>(x,1) ); }
	nonBalanced_bigTree.balance(balanced_bigTree, nonBalanced_bigTree.cbegin(), nonBalanced_bigTree.checkSize());

	
	BinaryTree<int, int>::Iterator found_at1{nullptr};
	BinaryTree<int, int>::Iterator found_at2{nullptr};
	std::map<int,int>::iterator found_at3{nullptr};

	time_point begin, end;
	double t_non_b=0, t_bal=0, t_map=0;
	std::size_t lookFor;

	for (std::size_t i=0; i<repetitions; ++i)
	{
		lookFor = rand()%N;
	
		begin = high_resolution_clock::now();
		found_at1 = nonBalanced_bigTree.find(lookFor);
		end = high_resolution_clock::now();
		t_non_b += duration_cast<nanoseconds> (end - begin).count();

		begin = high_resolution_clock::now();
		found_at2 = balanced_bigTree.find(lookFor);
		end = high_resolution_clock::now();		
		t_bal += duration_cast<nanoseconds> (end - begin).count();

		begin = high_resolution_clock::now();
		found_at3 = std_map.find(lookFor);
		end = high_resolution_clock::now();		
		t_map += duration_cast<nanoseconds> (end - begin).count();
	}
	
	std::cout << N << " " << t_non_b*10e-6 << " " << t_bal*10e-6<< " " << t_map*10e-6 << std::endl;
	///////////////////////////////////////////////////////////////
	#endif


	return 0;
}


