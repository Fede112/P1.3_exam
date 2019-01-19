#include <stdlib.h>		// atoi 
#include <iostream> 	// std::cout
#include <utility>		// std::pair
#include <stdexcept>	// std::runtime_error
#include <memory>		// unique_ptr
#include <chrono>
#include <vector>
#include <array>
#include <map>

enum class insertDir { left_dir, right_dir };

template <class TK, class TV>
class BinaryTree 
{
	// Elements from nested class are 
	// not visible from the container class. 
	// That's why we choose structs.	
	struct Node 
	{
		std::pair <TK,TV> keyVal;
		Node* ppNode; // std::share_ptr
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;
		
		// constructor that takes key and value separately
		Node(const TK& key, const TV& val, Node * s, Node * l, Node * r) 
		: keyVal{key, val}, ppNode{s}, left{l}, right{r} {}
		
		// constructor that takes key and value as std::pair
		Node(const std::pair <TK,TV> kV , Node * s, Node * l, Node * r) 
		: keyVal{kV}, ppNode{s}, left{l}, right{r} {}

		Node() = default;
		// ~Node() {std::cout << "Calling destructor of node " << keyVal.first << std::endl;}
		~Node() {}
		// void print_node() // sada
		// {
		// 	if(left){std::cout << "k: " << keyVal.first << " l: " << left->keyVal.first << std::endl;}
		// 	if(right){std::cout << "k: " << keyVal.first << " r: " << right->keyVal.first << std::endl;}
		// }
	};

	// pointer to root node of the binary tree by default it is initialize to nullptr
	std::unique_ptr<Node> root;
	std::size_t treeSize;

	public:

	template <class otk, class otv>
	friend std::ostream& operator<<(std::ostream&, const BinaryTree<otk, otv>&);

	// CONSTRUCTORS:
	// BinaryTree() = default;
	// BinaryTree(): treeSize{0} {std::cout << "tree size " << treeSize << std::endl;}
	BinaryTree(): treeSize{0} {}
	
	// Copy constructor
	BinaryTree(const BinaryTree&);
	
	// Copy assignment
	BinaryTree& operator=(const BinaryTree& v);

	// Move constructor
	BinaryTree(BinaryTree&& bt) noexcept;

	// Move assignment
	BinaryTree& operator=(BinaryTree&& bt) noexcept;

	
	// Define Iterators
	class Iterator;
	class ConstIterator;

	// begin() and end() function for Iterators/ConstIterators:
	// Used when the user wants to change the value of a Tree using iterators
	Iterator begin() { std::cout << "Iterator! \n";return Iterator{goLeft()}; }
	Iterator end(){return Iterator{nullptr};}

	// Used so that the user cannot change the state of a Tree using a reference call to a const Tree
	ConstIterator begin() const { std::cout << "ConstIterator! \n"; return ConstIterator{goLeft()}; }
	ConstIterator end() const { return ConstIterator{nullptr}; }

	// Used if we don´t want to change the state of a tree, but we are not using a const Tree 
	ConstIterator cbegin() { std::cout << "ConstIterator! cbegin \n"; return ConstIterator{goLeft()}; }
	ConstIterator cend() { return ConstIterator{nullptr};}

	// To use if the user calls cbegin on a const Tree 
	// (we allow the user to be lazy and not think about const Tree vs. noConst Tree)
	ConstIterator cbegin() const { std::cout << "ConstIterator! cbegin const\n"; return ConstIterator{goLeft()}; }
	ConstIterator cend() const { return ConstIterator{nullptr};}

	// auxilary function to begin() / end() / cbegin() / cend()
	Node * goLeft() const
	{
		Node * tmp = root.get();
		while (tmp->left.get()) tmp = tmp->left.get();
		return tmp;
	}

	// Insert new node
	void insert(const TK& k, const TV& v);
	// Clear all the nodes in the tree
	void clear(){root.reset(); treeSize=0;}
	// Retrieve tree size
	std::size_t checkSize() const {return treeSize;}
	// auxilary function of the copy constructor
	void copy_node(const Node * np);
	// balance the tree
	void balance(BinaryTree& balanceTree, Iterator begin, std::size_t locSize);
	// find key
	Iterator find(const TK& k) const
	{
		Iterator it = begin();
		for (; it!=end(); ++it) {if (k == (*it).first) return it;}
		return end();
	}	
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
	BinaryTree<TK,TV>::insert(np->keyVal.first, np->keyVal.second);
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

	std::pair<TK,TV>& operator*() const { return current->keyVal; }	
	// TK& operator*() const { return current->keyVal.first; }	

	bool operator==(const Iterator& other) { return current == other.current; }
	bool operator!=(const Iterator& other) { return !(*this == other); }

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

template <class TK, class TV>
void BinaryTree<TK, TV>::insert(const TK& k, const TV& v) 
{
	using Node = BinaryTree<TK, TV>::Node;
	treeSize ++;
	if (root == nullptr)
	{
		root.reset(new Node{k, v, root.get(), nullptr, nullptr});
		return;
	}
	insertDir dir; //= insertDir::left_dir; 
	Node* tmpA = root.get();
	Node* tmpB = nullptr;
	// Node* ppNode = nullptr;
	while (tmpA != nullptr )
	{
		tmpB = tmpA;
		if (k < tmpA->keyVal.first)
		{
			// ppNode = tmpA;
			tmpA = tmpA->left.get();
			dir = insertDir::left_dir;
			// ppNode = tmpB;
		} 
		else if (k > tmpA->keyVal.first)
		{	
			tmpA = tmpA->right.get();
			dir = insertDir::right_dir; 
		}
		else 
		{
			(tmpA->keyVal.second)++;
			treeSize--;
			return;
		}
	}
	switch (dir)
	{
		case insertDir::left_dir:
			tmpB->left.reset(new Node{k, v, tmpB, nullptr, nullptr});   
			break;
		case insertDir::right_dir:
			tmpB->right.reset(new Node{k, v, tmpB->ppNode, nullptr, nullptr}); 
			break;
		default:
			treeSize--;
			throw std::runtime_error{"unknown direction\n"};
	};
}

template <class TK, class TV>
void BinaryTree<TK, TV>::balance(BinaryTree<TK, TV>& balanceTree, BinaryTree<TK,TV>::Iterator begin, std::size_t locSize)
{	
	if (locSize == 1) {balanceTree.insert((*begin).first, (*begin).second); return;}
	
	BinaryTree<TK,TV>::Iterator tmp{begin}; // shallow copy constructor of Iterator
	std::size_t median;
	std::size_t locSize_L;
	std::size_t locSize_R;
	median = locSize/2 + 1;
	for(std::size_t i = 1; i < median; ++i)
		++tmp;
	balanceTree.insert((*tmp).first, (*tmp).second);
	locSize_L = median - 1;
	locSize_R = median -1 - (1 - locSize%2);
	balance(balanceTree, begin, locSize_L);
	if (locSize_R == 0) {return;}
	balance(balanceTree, ++tmp, locSize_R);
}



int main(int argc, char const *argv[])
{

	BinaryTree<int, int> test1;
	BinaryTree<int, int> test2;

	std::array<int, 9> keys_1{8, 3, 10, 6, 7, 1, 4, 14, 13};	
	for (auto x: keys_1) {test1.insert(x,1);}
	// Random tree
	for (int i = 0; i < 7; ++i)
	{
		test2.insert(rand()%100,1);
	}

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

	
	// std::cout << tree;
	
	// std::cout << "size of tree: " << tree.checkSize() << std::endl;
	// std::cout << tree;
	// std::cout << "size of toto: " << toto.checkSize() << std::endl;
	// std::cout << toto;
	// std::cout << "Mae, tree se destruyo" << std::endl;
	// auto look1 = tree.find(4);
	// auto look2 = tree.find(5);
	// auto stop = tree.end();

	// std::cout << "Looking for 4: ";
	// std::cout << (look1 != stop) << std::endl;
	// std::cout << "Looking for 5: " ;
	// std::cout << (look2 != stop) << std::endl;
	
	// BinaryTree<int, int> balanceTree;
	// tree.balance(balanceTree, tree.begin(), tree.checkSize());
	// std::cout << tree;
	// std::cout << "tree size: " << tree.checkSize() << std::endl;
	// std::cout << "balance tree size: " << balanceTree.checkSize() << std::endl;
	// toto.clear();
	// std::cout << "tree size after clear: " << tree.checkSize() << std::endl;
	// std::cout << "Hoping this is the last print" << std::endl;


	// const std::size_t N = atoi(argv[1]);
	// std::size_t lookFor = N/2;
	// using namespace std::chrono;
	// using time_point = steady_clock::time_point;

	// BinaryTree<int, int> nonBalanced_bigTree, balanced_bigTree;
	// std::map<int,int> std_map;

	// std::vector<int> aLotOfKeys;	
	// for (std::size_t i=0; i<N; ++i) {aLotOfKeys.push_back(N-i);}
	// for (auto x: aLotOfKeys) { nonBalanced_bigTree.insert(x,1); std_map.insert( std::pair<int,int>(x,0) ); }
	
	// nonBalanced_bigTree.balance(balanced_bigTree, nonBalanced_bigTree.begin(), nonBalanced_bigTree.checkSize());

	
	// time_point begin1 = steady_clock::now();
	// auto found_at1 = nonBalanced_bigTree.find(lookFor);
	// time_point end1= steady_clock::now();

	// time_point begin2 = steady_clock::now();
	// auto found_at2 = balanced_bigTree.find(lookFor);
	// time_point end2= steady_clock::now();
	
	// time_point begin3 = steady_clock::now();
	// auto found_at3 = std_map.find(lookFor);
	// time_point end3= steady_clock::now();

	// std::cout << N << " " << duration_cast<nanoseconds> (end1 - begin1).count();
	// std::cout << " " << duration_cast<nanoseconds> (end2 - begin2).count();
	// std::cout << " " << duration_cast<nanoseconds> (end3 - begin3).count() <<std::endl;

	return 0;
}


