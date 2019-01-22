#ifndef BINARYTREE
#define BINARYTREE

#include <stdlib.h>		// atoi 
#include <iostream> 	// std::cout
#include <utility>		// std::pair
#include <memory>		// unique_ptr
#include <stdexcept>	// std::runtime_error
// 

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

		// void print_node()
		// {

		// 	if (left && right) {std::cout << left->keyVal.first << " <--- " << keyVal.first << " ---> " << right->keyVal.first << std::endl;}
		// 	if (!left && right) {std::cout << "null" << " <--- " << keyVal.first << " ---> " << right->keyVal.first << std::endl;}
		// 	if (left && !right) {std::cout << left->keyVal.first << " <--- " << keyVal.first << " ---> " << "null" << std::endl;}
		// 	if (!left && !right) {std::cout << "null" << " <--- " << keyVal.first << " ---> " << "null" << std::endl;}
		// 	return;	
		// }
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
	Node * goLeft() const;

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
	void balance(BinaryTree& balanceTree, ConstIterator begin, const std::size_t locSize);

};

#include "iterator.h"

#endif