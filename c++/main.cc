#include <iostream> // std::cout
#include <memory> // unique_ptr
#include <stdexcept> // std::runtime_error
#include <utility> // std::pair
#include <array>

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
		~Node() {std::cout << "Calling destructor of node " << keyVal.first << std::endl;}
		// void print_node() // sada
		// {
		// 	if(left){std::cout << "k: " << keyVal.first << " l: " << left->keyVal.first << std::endl;}
		// 	if(right){std::cout << "k: " << keyVal.first << " r: " << right->keyVal.first << std::endl;}
		// }
	};



	// pointer to root node of the binary tree
	// by default it is initialize to nullptr
	std::unique_ptr<Node> root;
	std::size_t treeSize;

	public:

	template <class otk, class otv>
	friend std::ostream& operator<<(std::ostream&, const BinaryTree<otk, otv>&);

	// BinaryTree() = default;
	BinaryTree(): treeSize{0} {std::cout << "tree size " << treeSize << std::endl;}
	
	// Copy constructor
	BinaryTree(const BinaryTree&);
	// Copy assignment
	BinaryTree& operator=(const BinaryTree& v);

	// Move constructor
	BinaryTree(BinaryTree&& bt) noexcept;

	// Move assignment
	BinaryTree& operator=(BinaryTree&& bt) noexcept;

	
	class Iterator;
	class ConstIterator;
	Iterator begin() 
	{
		Node * tmp = root.get(); 
		while (tmp->left.get()) tmp = tmp->left.get();
		return Iterator{tmp}; 
	}
	Iterator end(){return Iterator{nullptr};}

	ConstIterator begin() const 
	{
		Node * tmp = root.get(); 
		while (tmp->left.get()) tmp = tmp->left.get();
		return ConstIterator{tmp}; 
	}
	ConstIterator end() const { return ConstIterator{nullptr}; }

	void insert(const TK& k, const TV& v);
	void clear(){root.reset(); treeSize=0;}
	std::size_t checkSize() const {return treeSize;}

	void balance(BinaryTree<TK, TV>& balanceTree, BinaryTree<TK,TV>::Iterator begin, std::size_t locSize)
	{	
		if (locSize == 1) {balanceTree.insert((*begin).first, (*begin).second); return;}
		
		BinaryTree<TK,TV>::Iterator tmp{begin}; // shallow copy constructor of Iterator
		std::size_t median;
		std::size_t locSize_L;
		std::size_t locSize_R;
		median = locSize/2 + 1;
		std::cout << "entre!" << std::endl;
		for(std::size_t i = 1; i < median; ++i)
			++tmp;
		balanceTree.insert((*tmp).first, (*tmp).second);
		locSize_L = median - 1;
		locSize_R = median -1 - (1 - locSize%2);
		balance(balanceTree, begin, locSize_L);
		if (locSize_R == 0) {return;}
		balance(balanceTree, ++tmp, locSize_R);
	}

	Iterator find(const TK& k) const
	{
		Iterator it = begin();
		for (; it!=end(); ++it) {if (k == (*it).first) return it;}
		return end();
	}
	

	void copy_node(const Node * np);
};

// copy ctor
template <class TK, class TV>
BinaryTree<TK,TV>::BinaryTree(const BinaryTree<TK, TV>& bt) // quionda : Is <TK,TV> needed?
{
	std::cout << "using copy ctor" << std::endl;
	copy_node(bt.root.get());
} 

// copy assignment
template <class TK, class TV>
BinaryTree<TK,TV>& BinaryTree<TK,TV>::operator=(const BinaryTree<TK, TV>& bt) 
{

	(*this).clear();              // first of all clean my memory
	auto tmp = bt;              // then use copy ctor
	(*this) = std::move(tmp);  // finally move assignment
	std::cout << "size of tmp: " << tmp.checkSize() << std::endl;

//   // or we do everything by hand..
//   // and we can do not reset and call new again if the sizes are suitable

	return *this;
}

// move ctor
template <class TK, class TV>
BinaryTree<TK, TV>::BinaryTree(BinaryTree&& bt) noexcept
  : root{std::move(bt.root)}, treeSize{std::move(bt.treeSize)} {
  	bt.treeSize = 0; // quionda : std::move of std::size_t doesnt set bt.treeSize=0. Using bt.root gives error, correct?
	std::cout << "move ctor: " << bt.treeSize << std::endl;
}


// move assignment
template <class TK, class TV>
BinaryTree<TK, TV>& BinaryTree<TK, TV>::operator=(BinaryTree&& bt) noexcept
{
	std::cout << "move assignment\n";
	treeSize = std::move(bt.treeSize);
	root = std::move(bt.root);
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
		// if(current){current->print_node();} // sada
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
  using parent::Iterator; // quionda ?? is this for the constructor?
  // const T& operator*() const { return parent::operator*(); } // quionda why do i need this operator overloaded ?? creo que es para (*this)++
};

template <class TK, class TV>
std::ostream& operator<<(std::ostream& os, const BinaryTree<TK, TV>& tree) 
{
	if(tree.checkSize()==0){return os <<"" << std::endl;}
	for (const auto& x : tree) // quionda el const???
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
			root.reset(new Node{k, v, nullptr, nullptr, nullptr}); // quionda try catch maybe better
			return;
		}

		insertDir dir = insertDir::left_dir; 

		Node* tmpA = root.get();
		Node* tmpB = nullptr;
		Node* ppNode = nullptr;

		while (tmpA != nullptr )
		{
			tmpB = tmpA;

			if (k < tmpA->keyVal.first)
			{
				// ppNode = tmpA;
				tmpA = tmpA->left.get();
				dir = insertDir::left_dir;
				ppNode = tmpB;
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
				tmpB->left.reset(new Node{k, v, ppNode, nullptr, nullptr});   
				break;
			case insertDir::right_dir:
				tmpB->right.reset(new Node{k, v, ppNode, nullptr, nullptr}); 
				break;
			default:
				treeSize--;
				throw std::runtime_error{"unknown direction\n"};
		};
	}


int main() 
{
	BinaryTree<int, int> tree;
	BinaryTree<int, int> test;
	std::array<int, 9> keys_1{8, 3, 10, 6, 7, 1, 4, 14, 13};	
	// std::array<int, 9> keys_2{1,2,3,4,5,6,7,8,9};
	// std::array<int, 3> keys{1, 2, 3};	
	for (auto x: keys_1) {tree.insert(x,1);}
	// for (auto x: keys_2) {test.insert(x,1);}

	// BinaryTree<int, int> test{tree};
	// std::cout << "Mae, voy a hacer una deep copy de tree" << std::endl;
	// test = tree;
	
	// std::cout << tree;
	// std::cout << "Mae, voy a hacer un move de tree" << std::endl;
	BinaryTree<int, int> toto = std::move(tree);
	
	std::cout << "size of tree: " << tree.checkSize() << std::endl;
	std::cout << tree;
	std::cout << "size of toto: " << toto.checkSize() << std::endl;
	std::cout << toto;
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
	toto.clear();
	// std::cout << "tree size after clear: " << tree.checkSize() << std::endl;
	std::cout << "Hoping this is the last print" << std::endl;
	return 0;
}
