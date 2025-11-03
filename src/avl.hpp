#ifndef AVL_HPP
#define AVL_HPP
#include <string>
#include <vector>

using namespace std;

class AVLTree {
public:
    AVLTree();
    ~AVLTree();

    void insert(int value);

    void remove(int value);
    void deleteValue(int value);

    string serialize() const;

    static AVLTree deserialize(const string &str);

private:
    struct Node {
        int val;
        Node *left;
        Node *right;
        int height;
        Node(int v) : val(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    Node* insert(Node* node, int value);
    Node* remove(Node* node, int value);
    Node* minValueNode(Node* node);
    int getHeight(Node* node) const;
    void updateHeight(Node* node);
    int balanceFactor(Node* node) const;
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    Node* rebalance(Node* node);

    void freeTree(Node* node);
    void computeHeights(Node* node); //post-order

    static void splitTokens(const string &s, vector<string> &out);
};

#endif