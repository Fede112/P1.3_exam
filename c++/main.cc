#include <stdlib.h>		// atoi 
#include <iostream> 	// std::cout
#include <utility>		// std::pair
#include <stdexcept>	// std::runtime_error
#include <memory>		// unique_ptr
#include <chrono>
#include <vector>
#include <array>
#include <map>
#include <stdexcept>

/*! \brief A binary tree class to store data.
 *
 *  The class is templated in both the Key and the Value.
 *	At the moment it only handles key types which have operations: ==, <, > defined.
 */
template <class TK, class TV>
class BinaryTree 
{
	// Elements from nested class are not visible from the container class. That's why we choose structs.
	/*! \brief A struct that contains the data of each node in the BinaryTree.
	*
	* 	We choose a struct so that elements its members are visible from the container class.
	*/
	struct Node 
	{
		std::pair <TK,TV> keyVal; /*!< stores the key and value of the node. */
		Node* ppNode; /*!< Pointer to the proper parent of the node. This is, the node which key is next in the tree (Increasing key order) */
		std::unique_ptr<Node> left;	/*!< Unique pointer to the left node from *this node */
		std::unique_ptr<Node> right; /*!< Unique pointer to the right node from *this node */
		
		/** Constructor that takes key and value separately. */
		Node(const TK& key, const TV& val, Node * s, Node * l, Node * r) 
		: keyVal{key, val}, ppNode{s}, left{l}, right{r} {}
		
		/** Constructor that takes key and value as std::pair */
		Node(const std::pair <TK,TV> kV , Node * s, Node * l, Node * r) 
		: keyVal{kV}, ppNode{s}, left{l}, right{r} {}

		/** Default Constructor */
		Node() = default;

		/** Default Destructor */
		// ~Node() {std::cout << "Calling destructor of node " << keyVal.first << std::endl;}
		~Node() {}
		void print_node() // sada
		{

			if (left && right) {std::cout << left->keyVal.first << " <--- " << keyVal.first << " ---> " << right->keyVal.first << std::endl;}
			if (!left && right) {std::cout << "null" << " <--- " << keyVal.first << " ---> " << right->keyVal.first << std::endl;}
			if (left && !right) {std::cout << left->keyVal.first << " <--- " << keyVal.first << " ---> " << "null" << std::endl;}
			if (!left && !right) {std::cout << "null" << " <--- " << keyVal.first << " ---> " << "null" << std::endl;}
			return;	
		}
	};

	// pointer to root node of the binary tree by default it is initialize to nullptr
	std::unique_ptr<Node> root; /*!< pointer to root node. */
	std::size_t treeSize; /*!< stores the numbers of nodes in the list. */

	public:

	template <class otk, class otv>
	friend std::ostream& operator<<(std::ostream&, const BinaryTree<otk, otv>&);

	// CONSTRUCTORS:
	/** Default constructor. 
     *  Constructs and empty binary tree. treeSize is set to zero. 
     */
	BinaryTree(): treeSize{0}  {}
	
	/** Copy constructor.*/
	BinaryTree(const BinaryTree&);
	
	/** Copy assignment. */
	BinaryTree& operator=(const BinaryTree& v);

	/** Move constructor. */
	BinaryTree(BinaryTree&& bt) noexcept;

	/** Move assignment. */
	BinaryTree& operator=(BinaryTree&& bt) noexcept;

	/** Returns a const reference to the value associated with the key.
	*	If the key is not present, throws an exception.
	*/
	const TV& operator[](const TK& key) const;
	/** Returns a reference to the value associated with the key.
	*	If the key is not present, it inserts the key with TTV{}.
	*/
	TV& operator[](const TK& key);

	/** Iterator of the class */
	class Iterator;
	/** ConstIterator of the class */
	class ConstIterator;

	// begin() and end() function for Iterators/ConstIterators:
	
	/** Used when the user wants to change the value of a Tree using iterators.	*/
	Iterator begin() { return Iterator{goLeft()}; }
	Iterator end(){return Iterator{nullptr};}

	/** Used so that the user cannot change the state of a Tree using a reference call to a const Tree.*/
	ConstIterator begin() const { return ConstIterator{goLeft()}; }
	ConstIterator end() const { return ConstIterator{nullptr}; }

	/** Used if we don´t want to change the state of a tree, but we are not using a const Tree. */
	ConstIterator cbegin() { return ConstIterator{goLeft()}; }
	ConstIterator cend() { return ConstIterator{nullptr};}

	/** To use if the user calls cbegin on a const Tree.
	* (we allow the user to be lazy and not think about const Tree vs. noConst Tree)
	*/
	ConstIterator cbegin() const {return ConstIterator{goLeft()}; }
	ConstIterator cend() const { return ConstIterator{nullptr};}

	/** Auxilary function to begin() / end() / cbegin() / cend() 
	* It finds the left most element of the tree (smaller key).
	*/
	Node * goLeft() const
	{
		Node * tmp = root.get();
		while (tmp->left.get()) tmp = tmp->left.get();
		return tmp;
	}

	/** Insert new node to the BinaryTree. */
	Node* insert(const std::pair<TK,TV>& pair);
	/** Find if a given key exists in the tree.
	*	Returns an Iterator to the Node.
	*/
 	Iterator find(const TK& key) const;
	/** auxiliary function for find() and insert() */
	Node* pos_find(const TK& key) const;
	/** Remove ALL nodes from tree */
	void clear(){root.reset(); treeSize=0;}
	/** Retrieve tree size */
	std::size_t checkSize() const {return treeSize;}
	/** auxilary function of the copy constructor */
	void copy_node(const Node * np);
	/** balance the tree
	*	The balance is not in place.
	*/
	void balance(BinaryTree& balanceTree, Iterator begin, std::size_t locSize);


	void dummy_func(Iterator it);
};

// Copy semantics /////////////////////////////////////////////
// copy ctor
template <class TK, class TV>
BinaryTree<TK,TV>::BinaryTree(const BinaryTree<TK, TV>& bt) // quionda : Is <TK,TV> needed?
{
	std::cout << "Copy ctor" << std::endl;
	copy_node(bt.root.get());
} 

// copy assignment
template <class TK, class TV>
BinaryTree<TK,TV>& BinaryTree<TK,TV>::operator=(const BinaryTree<TK, TV>& bt) 
{
	(*this).clear();              // first of all clean my memory
	auto tmp = bt;              // then use copy ctor
	(*this) = std::move(tmp);  // finally move assignment

	std::cout << "Copy assignment" << std::endl;

	return *this;
}

// aux function for copy ctr
template <class TK, class TV>
void BinaryTree<TK,TV>::copy_node(const BinaryTree<TK, TV>::Node * np)
{
	if(!np){return;}
	BinaryTree<TK,TV>::insert(np->keyVal);
	copy_node(np->left.get());
	copy_node(np->right.get());
	return;
}

// Move Semantics ////////////////////////////////////////////
// move ctor
template <class TK, class TV>
BinaryTree<TK, TV>::BinaryTree(BinaryTree&& bt) noexcept // move semantics cannot throw exceptions because objects are already allocated
  : root{std::move(bt.root)}, treeSize{std::move(bt.treeSize)} {
  	bt.treeSize = 0;
	std::cout << "move ctor" << std::endl;
}


// move assignment
template <class TK, class TV>
BinaryTree<TK, TV>& BinaryTree<TK, TV>::operator=(BinaryTree&& bt) noexcept
{
	std::cout << "move assignment" << std::endl;
	treeSize = std::move(bt.treeSize);
	root = std::move(bt.root);
	bt.treeSize = 0;
	return *this;
}

////////////////////////////////////////////////////////////////

// Iterators //////////////////////////////////////////////////
template <class TK, class TV>
class BinaryTree<TK,TV>::Iterator
{
	using Node = BinaryTree<TK, TV>::Node;
	Node* current;

	public:
	Iterator(Node* n) : current{n} {}
	/** Returns the std::pair keyVal of the node by reference*/
	std::pair<TK,TV>& operator*() const { return current->keyVal; }	
	// TK& operator*() const { return current->keyVal.first; }	

	bool operator==(const Iterator& other) { return current == other.current; }
	bool operator!=(const Iterator& other) { return !(*this == other); }

	/** It points the iterator to the next node in the tree asuming ascending key order. */
	Iterator& operator++() 
	{
		if (! (current->right.get() ) ) {current = current->ppNode; return *this;}
		current = current->right.get();
		while (current->left.get()) {current = current->left.get();}
		return *this;
	}
};

template <class TK, class TV>
class BinaryTree<TK,TV>::ConstIterator: public BinaryTree<TK,TV>::Iterator {
 public:
	using parent = BinaryTree<TK, TV>::Iterator;
	using parent::Iterator; 
	/** Returns a const std::pair keyVal of the node by reference*/
	const std::pair<TK,TV>& operator*() const { return parent::operator*(); }
};

/////////////////////////////////////////////////////////////////

template <class TK, class TV>
std::ostream& operator<<(std::ostream& os, const BinaryTree<TK, TV>& tree) 
{
	if(tree.checkSize()==0){return os <<"" << std::endl;}
	for (const auto& x : tree)
		os << x.first << ":" << x.second << " ";
	std::cout << std::endl;
	return os;
}

// Declaration

template <class TK, class TV>
typename BinaryTree<TK,TV>::Node* BinaryTree<TK, TV>::pos_find(const TK& key) const
{
	// using Node = BinaryTree<TK, TV>::Node;
	Node* tmpA = root ? root.get() : nullptr;
	Node* tmpB = nullptr;
	
	while (tmpA != nullptr && tmpA != tmpB )
	{
		tmpB = tmpA;
		if (key < tmpA->keyVal.first)
			tmpA = tmpA->left.get();
		else if (key > tmpA->keyVal.first)
			tmpA = tmpA->right.get();
	}
	return tmpB;
} 

	
template <class TK, class TV>
typename BinaryTree<TK,TV>::Iterator BinaryTree<TK, TV>::find(const TK& key)
{
	using Node = BinaryTree<TK, TV>::Node;
	Node* pos = BinaryTree<TK, TV>::pos_find(key);
	// pos->print_node();
	if (pos && key == pos->keyVal.first )
	{
		return Iterator{pos};
	}
	return Iterator{nullptr};
}

template <class TK, class TV>
typename BinaryTree<TK,TV>::Node* BinaryTree<TK, TV>::insert(const std::pair<TK,TV>& pair)
{
	using Node = BinaryTree<TK, TV>::Node;
	treeSize ++;

	Node* pos = BinaryTree<TK, TV>::pos_find(pair.first);

	if (pos && pair.first < pos->keyVal.first)
	{
		pos->left.reset(new Node{pair, pos, nullptr, nullptr});
		return pos->left.get();
	}
	else if (pos && pair.first > pos->keyVal.first)
	{	
		pos->right.reset(new Node{pair,  pos->ppNode, nullptr, nullptr});
		return pos->right.get();
	}
	else if (pos && pair.first == pos->keyVal.first)
	{	
		(pos->keyVal.second)++; treeSize--;
		return pos;
	}
	else
	{
		root.reset(new Node{pair, nullptr, nullptr, nullptr});
		return root.get();
	}

}

template <class TK, class TV>
typename BinaryTree<TK,TV>::Iterator BinaryTree<TK, TV>::find(const TK& key) const
{
	using Node = BinaryTree<TK, TV>::Node;
	Node* pos = BinaryTree<TK, TV>::pos_find(key);
	if (pos && key == pos->keyVal.first )
	{
		return Iterator{pos};
	}
	return Iterator{nullptr};
}

template <class TK, class TV>
const TV& BinaryTree<TK, TV>::operator[](const TK& key) const
{
	using Node = BinaryTree<TK, TV>::Node;
	Node* pos = BinaryTree<TK, TV>::pos_find(key);
	if (pos && key == pos->keyVal.first )
	{
		return pos->keyVal.second;
	}
	std::string errMsg = "Cannot insert new node. Const reference to Tree!";
	throw std::runtime_error("Error! " + errMsg);
}

template <class TK, class TV>
TV& BinaryTree<TK, TV>::operator[](const TK& key)
{
	using Node = BinaryTree<TK, TV>::Node;
	Node* pos = BinaryTree<TK, TV>::pos_find(key);
	if (pos && key == pos->keyVal.first )
	{
		return pos->keyVal.second;
	}
	else
	{
		return BinaryTree<TK, TV>::insert(std::pair<TK,TV>(key,TV{}))->keyVal.second;
	}
}

template <class TK, class TV>
void BinaryTree<TK, TV>::balance(BinaryTree<TK, TV>& balanceTree, BinaryTree<TK,TV>::Iterator begin, std::size_t locSize)
{	
	if (locSize == 1) {balanceTree.insert(*begin); return;}
	
	BinaryTree<TK,TV>::Iterator tmp{begin}; // shallow copy constructor of Iterator
	std::size_t median;
	std::size_t locSize_L;
	std::size_t locSize_R;
	median = locSize/2 + 1;
	for(std::size_t i = 1; i < median; ++i)
		++tmp;
	balanceTree.insert(*tmp);
	locSize_L = median - 1;
	locSize_R = median -1 - (1 - locSize%2);
	balance(balanceTree, begin, locSize_L);
	if (locSize_R == 0) {return;}
	balance(balanceTree, ++tmp, locSize_R);
}


template <class TK, class TV>
void BinaryTree<TK, TV>::dummy_func(BinaryTree<TK,TV>::Iterator it)
{
	int a = 5;
	return;
}


int main(int argc, char const *argv[])
{

	/////////////////////////////////////////////////////////////////
	// TEST
	#ifdef TEST

	if(argc != 1)
	{
		printf("Test RUN, no arguments needed\n");
		exit(1);
	}

	BinaryTree<int, int> test1;
	BinaryTree<int, int> test2;

	std::array<int, 9> keys_1{8, 3, 10, 6, 7, 1, 4, 14, 13};	
	for (auto x: keys_1) {test1.insert(std::pair<int,int>(x,1));}
	// Random tree
	for (int i = 0; i < 7; ++i)
	{
		test2.insert(std::pair<int,int>(rand()%100,1));
	}

	/////////////////////////////////////////////////////////////////
	// TEST operator[]
	std::cout << "\noperator[] test:" << std::endl;
	std::cout << test1;
	test1[21];
	std::cout << test1;

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
	
	std::cout << "\nCompleted copy and move semantics tests ..." << std::endl;
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
	std::cout << std::endl;
	std::cout << "\nCompleted Iterators tests ..." << std::endl;
	/////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////

	// FIND TEST
	BinaryTree<int, int> tree;
	for (auto x: keys_1) {tree.insert(std::pair<int,int>(x,1));}
	auto look1 = tree.find(4);
	auto look2 = tree.find(5);
	auto find_stop = tree.end();

	std::cout << "Looking for 4: ";
	std::cout << (look1 != find_stop) << std::endl;
	std::cout << "Looking for 5: " ;
	std::cout << (look2 != find_stop) << std::endl;
		
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
	nonBalanced_bigTree.balance(balanced_bigTree, nonBalanced_bigTree.begin(), nonBalanced_bigTree.checkSize());

	
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


