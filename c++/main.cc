#include <iostream> // std::cout
#include <memory> // unique_ptr
#include <stdexcept> // std::runtime_error
#include <utility> // std::pair

enum class method { push_back, push_front };

template <class TK, class TV>
class BinaryTree 
{
  // Elements from nested class are 
  // not visible from the container class. 
  // That's why we choose structs.
  struct Node 
  {
    std::pair <TK,TV> keyVal;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node(TK& key, TV& val, Node * l, Node * r) : keyVal{key, val}, left{l}, right{r} {}
    ~Node() = default;
  };

  // pointer to root node of the binary tree
  // by default it is initialize to nullptr
  std::unique_ptr<Node> root;

  public:

  template <class TK, class TV>
  void insert(const TK& k, const TV& v) 
  {
    
    if (head == nullptr)
      head.reset(new Node{k, v, nullptr, nullptr});
      return;

    // auto tmp = head.get();
    Node* tmp = head.get();
    Node* tmp2;
     
    while (tmp != nullptr)
    {
      tmp2 = tmp;
      if (k < tmp->key) tmp = tmp->left.get();
      else if (k > tmp->key) tmp = tmp->right.get();
      else {(tmp->val)++; return;}
    }
    tmp2.reset(new Node{k, v, nullptr, nullptr}); 
  }
};


int main() 
{
  List<int> list;
  list.push(3);                       // calls push_front
  list.push(5, method::push_back);    // calls push_back
  list.push(42, method::push_front);  // calls push_front

  for (auto& x : list) {
    ++x;
  }

  
  // for (auto& x : list) {
  //   ++x;
  // }
  std::cout << list;
  return 0;
}
