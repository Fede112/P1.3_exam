
#include <stdlib.h>		// atoi 
#include <iostream> 	// std::cout
#include <utility>		// std::pair
#include <stdexcept>	// std::runtime_error

#include "iterator.h"
#include "binarytree.h"

// Copy semantics /////////////////////////////////////////////
// copy ctor
template <class TK, class TV>
BinaryTree<TK,TV>::BinaryTree(const BinaryTree<TK, TV>& bt)
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


template <class TK, class TV>
std::ostream& operator<<(std::ostream& os, const BinaryTree<TK, TV>& tree) 
{
	if(tree.checkSize()==0){return os <<"" << std::endl;}
	for (const auto& x : tree)
		os << x.first << ":" << x.second << " ";
	std::cout << std::endl;
	return os;
}

// Member Functions:
template <class TK, class TV>
typename BinaryTree<TK,TV>::Node * BinaryTree<TK, TV>::goLeft() const
{
	Node * tmp = root.get();
	while (tmp->left.get()) tmp = tmp->left.get();
	return tmp;
}

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
typename BinaryTree<TK,TV>::Iterator BinaryTree<TK, TV>::find(const TK& key) const 
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
void BinaryTree<TK, TV>::balance(BinaryTree<TK, TV>& balanceTree, BinaryTree<TK,TV>::ConstIterator begin, const std::size_t locSize)
{	
	if (locSize == 1) {balanceTree.insert(*begin); return;}
	
	BinaryTree<TK,TV>::ConstIterator tmp{begin}; // shallow copy constructor of Iterator
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


template BinaryTree<int,int>::BinaryTree(const BinaryTree<int, int>& bt);
template BinaryTree<int,int>& BinaryTree<int,int>::operator=(const BinaryTree<int, int>& bt) ;
template void BinaryTree<int,int>::copy_node(const BinaryTree<int, int>::Node * np);
template BinaryTree<int, int>::BinaryTree(BinaryTree&& bt) noexcept; // move semantics cannot throw exceptions because objects are already allocated;
template BinaryTree<int, int>& BinaryTree<int, int>::operator=(BinaryTree&& bt) noexcept;
template std::ostream& operator<<(std::ostream& os, const BinaryTree<int, int>& tree) ;
template typename BinaryTree<int,int>::Node * BinaryTree<int, int>::goLeft() const;
template typename BinaryTree<int,int>::Node* BinaryTree<int, int>::pos_find(const int& key) const;
template typename BinaryTree<int,int>::Iterator BinaryTree<int, int>::find(const int& key) const ;
template typename BinaryTree<int,int>::Node* BinaryTree<int, int>::insert(const std::pair<int,int>& pair);
template const int& BinaryTree<int, int>::operator[](const int& key) const;
template int& BinaryTree<int, int>::operator[](const int& key);
template void BinaryTree<int, int>::balance(BinaryTree<int, int>& balanceTree, BinaryTree<int,int>::ConstIterator begin, const std::size_t locSize);


template BinaryTree<double,double>::BinaryTree(const BinaryTree<double, double>& bt);
template BinaryTree<double,double>& BinaryTree<double,double>::operator=(const BinaryTree<double, double>& bt) ;
template void BinaryTree<double,double>::copy_node(const BinaryTree<double, double>::Node * np);
template BinaryTree<double, double>::BinaryTree(BinaryTree&& bt) noexcept; // move semantics cannot throw exceptions because objects are already allocated;
template BinaryTree<double, double>& BinaryTree<double, double>::operator=(BinaryTree&& bt) noexcept;
template std::ostream& operator<<(std::ostream& os, const BinaryTree<double, double>& tree) ;
template typename BinaryTree<double,double>::Node * BinaryTree<double, double>::goLeft() const;
template typename BinaryTree<double,double>::Node* BinaryTree<double, double>::pos_find(const double& key) const;
template typename BinaryTree<double,double>::Iterator BinaryTree<double, double>::find(const double& key) const ;
template typename BinaryTree<double,double>::Node* BinaryTree<double, double>::insert(const std::pair<double,double>& pair);
template const double& BinaryTree<double, double>::operator[](const double& key) const;
template double& BinaryTree<double, double>::operator[](const double& key);
template void BinaryTree<double, double>::balance(BinaryTree<double, double>& balanceTree, BinaryTree<double,double>::ConstIterator begin, const std::size_t locSize);

