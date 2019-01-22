#ifndef ITERATOR
#define ITERATOR

#include "binarytree.h"


// Iterator of BinaryTree class
template <class TK, class TV>
class BinaryTree<TK,TV>::Iterator
{
	using Node = BinaryTree<TK, TV>::Node;
 protected:
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

////////////////////////////////////////////////////////////////

// ConstIterator of BinaryTree class
template <class TK, class TV>
class BinaryTree<TK,TV>::ConstIterator: public BinaryTree<TK,TV>::Iterator {
	using parent = BinaryTree<TK, TV>::Iterator;
	using parent::current; 
 public:
	using parent::Iterator;
	/** Returns a const std::pair keyVal of the node by reference*/
	const std::pair<TK,TV>& operator*() const { return parent::operator*(); }

	ConstIterator& operator++() 
	{
		if (! (current->right.get() ) ) {current = current->ppNode; return *this;}
		current = current->right.get();
		while (current->left.get()) {current = current->left.get();}
		return *this;
	}
};


#endif