#include <iostream> // std::cout
#include <memory> // unique_ptr
#include <stdexcept> // std::runtime_error
#include <utility> // std::pair

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
		std::unique_ptr<Node> parent;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;

		Node(const TK& key, const TV& val, Node * p, Node * l, Node * r) 
		: keyVal{key, val}, parent{p}, left{l}, right{r} {}

		~Node() = default;

		void display()
		{
			std::cout << keyVal.first << " "; 
		}
	};

	// pointer to root node of the binary tree
	// by default it is initialize to nullptr
	std::unique_ptr<Node> root;

	public:

	void insert(const TK& k, const TV& v) 
	{
		if (root == nullptr)
		{
			root.reset(new Node{k, v, nullptr, nullptr, nullptr});
			return;      
		}

		insertDir dir = insertDir::left_dir; 

		Node* tmp = root.get();
		Node* dad_node = nullptr;

		while (tmp != nullptr)
		{
			dad_node = tmp;

			if (k < tmp->keyVal.first)
			{
				tmp = tmp->left.get();
				dir = insertDir::left_dir;
			} 
			else if (k > tmp->keyVal.first)
			{
				tmp = tmp->right.get();
				dir = insertDir::right_dir; 
			}
			else 
			{
				(tmp->keyVal.second)++;
				return;
			}
		}

		switch (dir)
		{
			case insertDir::left_dir:
				dad_node->display();
				std::cout << "go left -> " << k << std::endl;
				dad_node->left.reset(new Node{k, v, dad_node, nullptr, nullptr});   
				break;
			case insertDir::right_dir:
				dad_node->display();
				std::cout << "go right -> " <<  k << std::endl;
				dad_node->right.reset(new Node{k, v, dad_node, nullptr, nullptr});   
				break;
			default:
				throw std::runtime_error{"unknown direction\n"};
		};
	}
};






int main() 
{
	BinaryTree<int, int> tree;

	tree.insert(8,0);  //left
	tree.insert(3,0);  //left
	tree.insert(10,0);  //left
	tree.insert(6,0);  //left
	tree.insert(7,0);  //left
	tree.insert(1,0);  //left
	tree.insert(4,0);  //left
	tree.insert(14,0);  //left
	tree.insert(13,0);  //left

	return 0;
}
