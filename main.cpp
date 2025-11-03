#include "src/avl.hpp"
#include <iostream>
 
int main() {
 AVLTree tree;
 
  // Inserts four nodes into the tree
  tree.insert(3);
  tree.insert(4);
  tree.insert(5);
  tree.insert(6);
 
  // Prints out a serialized tree which might look like "4,3,5,nil,nil,nil,6"
  // The actual output will depend on the pattern you use to serialize the tree
  std::cout << tree.serialize();
 
  // Deletes a node from the tree
  tree.deleteValue(6);
 
  // Prints out a serialized tree which might look like "4,3,5"
  // The actual output will depend on the pattern you use to serialize the tree
  std::cout << tree.serialize();
}