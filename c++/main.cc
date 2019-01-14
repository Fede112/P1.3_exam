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
		~Node() = default;

		void display() {std::cout << keyVal.first << " "; }
	};


	// pointer to root node of the binary tree
	// by default it is initialize to nullptr
	std::unique_ptr<Node> root;
	Node* far_left = nullptr;
	Node* far_right = nullptr;

	public:

	template <class otk, class otv>
	friend std::ostream& operator<<(std::ostream&, const BinaryTree<otk, otv>&);

	BinaryTree() = default;

	void commit()
	{
		Node* tmpR = root.get();
		Node* tmpL = root.get();
		while (tmpL->left.get()) tmpL = tmpL->left.get();
		while (tmpR->right.get()) tmpR = tmpR->right.get();
		far_left = tmpL;
		far_right = tmpR;
	}

	Node* return_far_left() {return far_left;}
	Node* return_far_right() {return far_right;}


	void insert(const TK& k, const TV& v) 
	{
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

	class Iterator;
	// Iterator begin() { return Iterator{far_left}; }
	Iterator begin() 
	{
		Node * tmp = root.get(); 
		while (tmp->left.get()) tmp = tmp->left.get();
		return Iterator{tmp}; 
	}
	// Iterator end() { return Iterator{far_right}; }
	Iterator end()
	{
		Node * tmp = root.get(); 
		while (tmp->right.get()) tmp = tmp->right.get();
		return Iterator{tmp}; 
	}
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

		// Node* next(Node* pnode) // next(const Node* pnode)
		// {
		// 	Node* tmp = nullptr;

		// 	if (! (pnode->right.get() ) ) {tmp = pnode->Snode; return tmp;}
		// 	pnode = pnode->right.get();
		// 	if ( !(pnode->left.get()) ) {return pnode;}
		// 	while (pnode->left.get()) {pnode = pnode->left.get();}
		// 	return pnode;
		// }
	};



template <class TK, class TV>
std::ostream& operator<<(std::ostream& os, BinaryTree<TK, TV>& tree) 
{
	auto it = tree.begin();
	auto stop = tree.end();
	for(; it!=stop; ++it)
	  os << *it << " ";
	os << *stop << std::endl;
	return os;
}


int main() 
{
	BinaryTree<int, int> tree;
	std::array<int, 9> keys{8, 3, 10, 6, 7, 1, 4, 14, 13};	
	for (auto x: keys) {tree.insert(x,0);}
	// tree.commit();
	std::cout << tree;
	return 0;
}
