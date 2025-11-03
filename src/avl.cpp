#include "avl.hpp"
#include <algorithm>
#include <queue>
#include <sstream>
#include <vector>
#include <cassert>

using namespace std; 

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    freeTree(root);
}

void AVLTree::freeTree(Node* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

int AVLTree::getHeight(Node* node) const {
    return node ? node->height : 0;
}

void AVLTree::updateHeight(Node* node) {
    if (node) node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

int AVLTree::balanceFactor(Node* node) const {
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

AVLTree::Node* AVLTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

AVLTree::Node* AVLTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

AVLTree::Node* AVLTree::rebalance(Node* node) {
    if (!node) return node;

    updateHeight(node);
    int bf = balanceFactor(node);

    //Left
    if (bf > 1) {
        if (balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left); // Left-Right
        }
        return rotateRight(node); // Left-Left
    }

    //Right
    if (bf < -1) {
        if (balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right); // Right-Left
        }
        return rotateLeft(node);  // Right-Right
    }

    return node;
}

AVLTree::Node* AVLTree::insert(Node* node, int value) {
    if (!node) return new Node(value);

    if (value < node->val) {
        node->left = insert(node->left, value);
    } else if (value > node->val) {
        node->right = insert(node->right, value);
    } else {
        return node;
    }

    return rebalance(node);
}

void AVLTree::insert(int value) {
    root = insert(root, value);
}

AVLTree::Node* AVLTree::minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left) current = current->left;
    return current;
}

AVLTree::Node* AVLTree::remove(Node* node, int value) {
    if (!node) return node;

    if (value < node->val) {
        node->left = remove(node->left, value);
    } else if (value > node->val) {
        node->right = remove(node->right, value);
    } else {
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            if (!temp) {
                temp = node; //no child
                node = nullptr;
            } else {
                *node = *temp; //one child
            }
            delete temp;
        } else {
            Node* temp = minValueNode(node->right); //two children
            node->val = temp->val;
            node->right = remove(node->right, temp->val);
        }
    }

    if (!node) return node;

    return rebalance(node);
}

void AVLTree::remove(int value) {
    root = remove(root, value);
}

void AVLTree::deleteValue(int value) {
    remove(value);
}

string AVLTree::serialize() const {
    if (!root) return "";

    queue<Node*> q;
    q.push(root);
    vector<string> tokens;

    while (!q.empty()) {
        Node* cur = q.front(); q.pop();
        if (!cur) {
            tokens.push_back("nil");
            continue;
        } else {
            tokens.push_back(to_string(cur->val));
            q.push(cur->left);
            q.push(cur->right);
        }
    }

    int last = (int)tokens.size() - 1;
    while (last >= 0 && tokens[last] == "nil") --last;
    if (last < 0) return ""; 

    ostringstream oss;
    for (int i = 0; i <= last; ++i) {
        if (i) oss << ',';
        oss << tokens[i];
    }
    return oss.str();
}

void AVLTree::splitTokens(const string &s, vector<string> &out) {
    out.clear();
    istringstream iss(s);
    string token;
    while (getline(iss, token, ',')) {
        size_t start = token.find_first_not_of(" \t\r\n");
        size_t end = token.find_last_not_of(" \t\r\n");
        if (start == string::npos) token = "";
        else token = token.substr(start, end - start + 1);
        out.push_back(token);
    }
}

void AVLTree::computeHeights(Node* node) {
    if (!node) return;
    computeHeights(node->left);
    computeHeights(node->right);
    updateHeight(node);
}

AVLTree AVLTree::deserialize(const string &str) {
    AVLTree tree;
    if (str.empty()) return tree;

    vector<string> tokens;
    splitTokens(str, tokens);
    if (tokens.empty()) return tree;

    if (tokens[0] == "nil" || tokens[0].empty()) {
        return tree;
    }

    //BFS order
    queue<Node*> q;
    Node* rootNode = new Node(stoi(tokens[0]));
    q.push(rootNode);
    size_t idx = 1;
    while (!q.empty() && idx < tokens.size()) {
        Node* parent = q.front(); q.pop();

        //left child
        if (idx < tokens.size()) {
            if (tokens[idx] != "nil") {
                Node* leftN = new Node(stoi(tokens[idx]));
                parent->left = leftN;
                q.push(leftN);
            }
            ++idx;
        }

        //right child
        if (idx < tokens.size()) {
            if (tokens[idx] != "nil") {
                Node* rightN = new Node(stoi(tokens[idx]));
                parent->right = rightN;
                q.push(rightN);
            }
            ++idx;
        }
    }

    tree.root = rootNode;
    tree.computeHeights(tree.root);
    return tree;
}