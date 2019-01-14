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
		Node* Snode; // std::share_ptr
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;

		Node(const TK& key, const TV& val, Node * s, Node * l, Node * r) 
		: keyVal{key, val}, Snode{s}, left{l}, right{r} {}

		Node() = default;
		~Node() {std::cout << "Calling destructor of node " << keyVal.first << std::endl;}

		// void display() {std::cout << keyVal.first << " "; }
	};

	// pointer to root node of the binary tree
	// by default it is initialize to nullptr
	std::unique_ptr<Node> root;

	public:

	template <class otk, class otv>
	friend std::ostream& operator<<(std::ostream&, const BinaryTree<otk, otv>&);

	BinaryTree() = default;

	void insert(const TK& k, const TV& v);
	void clear(){root.reset();}
	
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

};

template <class TK, class TV>
class BinaryTree<TK,TV>::Iterator
{
	using Node = BinaryTree<TK, TV>::Node;
	Node* current;

	public:
	Iterator(Node* n) : current{n} {}

	TK& operator*() const { return current->keyVal.first; }	

	bool operator==(const Iterator& other) { return current == other.current; }
	bool operator!=(const Iterator& other) { return !(*this == other); }

	Iterator& operator++() 
	{
		// current = next(current);
		// return *this;
		if (! (current->right.get() ) ) {current = current->Snode; return *this;}
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
  // const T& operator*() const { return parent::operator*(); } // quionda why do i need this operator overloaded ?? creo que es para ++(*this)
};

template <class TK, class TV>
std::ostream& operator<<(std::ostream& os, const BinaryTree<TK, TV>& tree) 
{
	for (const auto& x : tree) // quionda el const???
		os << x << " ";
	std::cout << std::endl;
	return os;
}

template <class TK, class TV>
void BinaryTree<TK, TV>::insert(const TK& k, const TV& v) 
	{
		using Node = BinaryTree<TK, TV>::Node;
		if (root == nullptr)
			root.reset(new Node{k, v, nullptr, nullptr, nullptr});

		insertDir dir = insertDir::left_dir; 

		Node* tmpA = root.get();
		Node* tmpB = nullptr;
		Node* Snode = nullptr;

		while (tmpA != nullptr )
		{
			tmpB = tmpA;

			if (k < tmpA->keyVal.first)
			{
				// Snode = tmpA;
				tmpA = tmpA->left.get();
				dir = insertDir::left_dir;
				Snode = tmpB;
			} 
			else if (k > tmpA->keyVal.first)
			{	
				tmpA = tmpA->right.get();
				dir = insertDir::right_dir; 
			}
			else 
			{
				(tmpA->keyVal.second)++;
				return;
			}
		}

		switch (dir)
		{
			case insertDir::left_dir:
				tmpB->left.reset(new Node{k, v, Snode, nullptr, nullptr});   
				break;
			case insertDir::right_dir:
				tmpB->right.reset(new Node{k, v, Snode, nullptr, nullptr}); 
				break;
			default:
				throw std::runtime_error{"unknown direction\n"};
		};
	}


int main() 
{
	BinaryTree<int, int> tree;
	std::array<int, 9> keys{8, 3, 10, 6, 7, 1, 4, 14, 13};	
	for (auto x: keys) {tree.insert(x,0);}
	std::cout << tree;
	tree.clear();
	std::cout << "Hoping this is the last print" << std::endl;
	return 0;
}
