# C++ Exam: Binary Tree

### Jesus Espinoza and Federico Barone

--------------------------------



For this exercise we were asked to implement a binary tree class.

```Main.cpp``` contains the implementation of the class as well as a few simple tests to check the different methods of the class and a benchmark of its performance (how the find method performs).

Full ```doxygen``` documentation of the code is available in the documentation folder. Check ```index.html``` file for a supreme experience.

To run either the test or the benchmark execute the following commands:





Figure 1 shows the results of the benchark. We compared the perfomance of the following data structures:

*  A linked list (actually it is our binary tree with nodes inserted in ascending order)
* A 
* 

![Figure 1: Workflow example](./documentation/images/py_workflow.png)

Comentario de la figura (log(n) vs n) y como es la estadística. 



Lastly, some comments about the implementation of two important functions of the class: `insert()` and `find()`.

The core of both functions is another function, `pos_find(TK& key)`, which works as follows. It takes a key as an argument and returns a pointer to the node which matches that key. If the key is not present in the `BinaryTree` then it returns a pointer to the node directly above this missing key. By doing this, it simplifies the implementation of insert() and find(), which with almost all the work done for them, they just need a simple if condition to return what they are suppose to.

------

**Small comment**: Although optional, we implemented the operator[] in both of its versions. Both of them work, but for the const version we decided to throw an exception if the key you are looking is not in the tree. We are not sure if this is the best implementation. 