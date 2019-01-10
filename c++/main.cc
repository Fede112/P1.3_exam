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
		Node* Snode;
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
		while (tmpL->left.get()) go_left(tmpL);
		while (tmpR->right.get()) go_right(tmpR);
		far_left = tmpL;
		far_right = tmpR;
	}


	void go_left(Node* &pnode) {pnode = pnode->left.get();}
	void go_right(Node* &pnode) {pnode = pnode->right.get();}
	void go_up(Node* &pnode) {pnode = pnode->Snode;}
	Node* return_far_left() {return far_left;}
	Node* return_far_right() {return far_right;}

	void creat_origin_node(const TK& k, const TV& v) 
	{
		root.reset(new Node{k, v, nullptr, nullptr, nullptr});
		return;      		
	}

	void insert(const TK& k, const TV& v) 
	{
		if (root == nullptr) {creat_origin_node(k, v);}

		insertDir dir = insertDir::left_dir; 

		Node* tmpA = root.get();
		Node* tmpB = nullptr;
		Node* Snode = nullptr;

		while (tmpA != nullptr)
		{
			tmpB = tmpA;

			if (k < tmpA->keyVal.first)
			{
				go_left(tmpA);
				dir = insertDir::left_dir;
				Snode = tmpB;
			} 
			else if (k > tmpA->keyVal.first)
			{
				go_right(tmpA);
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

	Node* next(Node* pnode)
	{
		Node* tmp = nullptr;

		if (! (pnode->right.get() ) ) {tmp = pnode->Snode; return tmp;	}

		go_right(pnode);

		if ( !(pnode->left.get()) ) {return pnode;}

		while (pnode->left.get()) {go_left(pnode);}

		return pnode;
	}

	// class Iterator;
	// Iterator begin() { return Iterator{far_left}; }
	// Iterator end() { return Iterator{far_right}; }

	// class Iterator
	// {
	// 		Node* current;
	// 		public:
	// 		Iterator(Node* n) : current{n} {}
	// };
};


template <class TK, class TV>
std::ostream& operator<<(std::ostream& os, BinaryTree<TK, TV>& tree) 
{
	auto begin = tree.return_far_left();
	auto end = tree.return_far_right();
	auto tmp = begin;

	while (tmp != end) 
	{ 	
	  os << tmp->keyVal.first << " ";
	  tmp = tree.next(tmp);
	}

	os << end->keyVal.first << " ";
	os << std::endl;
	
	return os;
}


int main() 
{
	BinaryTree<int, int> tree;

	std::array<int, 9> keys{8, 3, 10, 6, 7, 1, 4, 14, 13};	
	for (auto x: keys) {tree.insert(x,0);}

	tree.commit();
	std::cout << tree;

	return 0;
}
