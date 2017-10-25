#include <bits/stdc++.h>

namespace Treap {
    int getrand() {
        /*static std::random_device rd;
        static std::mt19937 generator(rd());
        static std::uniform_int_distribution<int> distribution(0, INT_MAX);
        return distribution(generator);*/
        return rand() ^ (rand() << 15);
    }

    struct Node {
        Node *left;
        Node *right;
        int priority;
        int size;
        long long value;
        long long sum;
        long long add;
        bool isReversed;

        explicit Node(long long value): left(nullptr), right(nullptr), value(value) {
            priority = getrand();
            size = 1;
            sum = value;
            isReversed = false;
            add = 0;
        }
    };

    int getSize(Node *node) {
        return node ? node->size: 0;
    }

    long long getSum(Node *node) {
        return node ? node->sum: 0;
    }

    void addToNode(Node *node, long long value) {
        if (node) {
            node->value += value;
            node->sum += value * getSize(node);
            node->add += value;
        }
    }

    void reverseNode(Node *node) {
        if (node) {
            std::swap(node->left, node->right);
            node->isReversed = !node->isReversed;
        }
    }

    void push(Node *node) {
        if (!node) return;
        if (node->isReversed) {
            reverseNode(node->left);
            reverseNode(node->right);
        }
        if (node->add) {
            addToNode(node->left, node->add);
            addToNode(node->right, node->add);
        }
        node->isReversed = false;
        node->add = 0;
    }

    void recalc(Node *node) {
        node->size = 1 + getSize(node->left) + getSize(node->right);
        node->sum = node->value + getSum(node->left) + getSum(node->right);
    }

    Node* Merge(Node *left, Node *right) {
        if (!right)
            return left;
        if (!left)
            return right;
        push(left);
        push(right);
        if (left->priority > right->priority) {
            left->right = Merge(left->right, right);
            recalc(left);
            return left;
        } else {
            right->left = Merge(left, right->left);
            recalc(right);
            return right;
        }
    }

    std::pair<Node*, Node*> Split(Node *node, int k) {
        /*return (T1, T2). |T1| = max(0, min(k, |node|))*/
        if (!node)
            return {nullptr, nullptr};
        push(node);
        if (getSize(node->left) < k) {
            Node *left, *right;
            std::tie(left, right) = Split(node->right, k - 1 - getSize(node->left));
            node->right = left;
            recalc(node);
            return {node, right};
        } else {
            Node *left, *right;
            std::tie(left, right) = Split(node->left, k);
            node->left = right;
            recalc(node);
            return {left, node};
        }
    }

    std::pair<Node*, Node*> SplitByValue(Node *node, int value) {
        /*use only if tree is sorted*/
        /*return (T1, T2). For all x in T1 x < value*/
        if (!node)
            return {nullptr, nullptr};
        push(node);
        if (node->value < value) {
            Node *left, *right;
            std::tie(left, right) = SplitByValue(node->right, value);
            node->right = left;
            recalc(node);
            return {node, right};
        } else {
            Node *left, *right;
            std::tie(left, right) = SplitByValue(node->left, value);
            node->left = right;
            recalc(node);
            return {left, node};
        }
    }

    void Insert(Node* &node, int pos, long long value) {
        Node *left, *right;
        std::tie(left, right) = Split(node, pos);
        node = Merge(Merge(left, new Node(value)), right);
    }

    void Remove(Node* &node, int pos) {
        Node *left, *mid, *right;
        std::tie(left, right) = Split(node, pos + 1);
        std::tie(left, mid) = Split(left, pos);
        delete mid;
        node = Merge(left, right);
    }

    template<typename Function>
    void queryOnSegment(Node* &node, int l, int r, Function callback) {
        Node *left, *mid, *right;
        std::tie(left, right) = Split(node, r + 1);
        std::tie(left, mid) = Split(left, l);
        callback(mid);
        node = Merge(Merge(left, mid), right);
    }

    long long getSumOnSegment(Node* &root, int l, int r) {
        long long answer;
        queryOnSegment(root, l, r, [&answer] (Node* &node) {answer = getSum(node);});
        return answer;
    }

    void addToSegment(Node* &root, int l, int r, long long value) {
        queryOnSegment(root, l, r, [value] (Node* &node) {addToNode(node, value);});
    }

    void print_tree(Node *node, bool is_root=true) {
        if (node) {
            push(node);
            print_tree(node->left, false);
            std::cerr << node->value << ' ';
            print_tree(node->right, false);
        }
        if (is_root) std::cerr << '\n';
    }

    void clear_tree(Node* &node) {
        if (node) {
            clear_tree(node->left);
            clear_tree(node->right);
            delete node;
            node = nullptr;
        }
    }
}

using namespace std;
using namespace Treap;

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0);

    Node *root = nullptr;
    Node *left, *right;
    for (int i = 0; i < 10; ++i) {
        Insert(root, i, 2 * i);
    }
    Remove(root, 0);
    Remove(root, 0);
    tie(left, right) = SplitByValue(root, 12);
    print_tree(left);
    print_tree(right);
    root = Merge(left, right);
    print_tree(root);
    tie(left, right) = Split(root, 6);
    cerr << getSize(left) << ' ' << getSize(right) << '\n';
    root = Merge(left, right);
    print_tree(root);

    cerr << '\n';
    clear_tree(root);
    for (int i = 0; i < 10; ++i) {
        Insert(root, i, i);
    }
    queryOnSegment(root, 6, 9, reverseNode);
    print_tree(root);
    cerr << getSumOnSegment(root, 2, 5) << '\n';
    addToSegment(root, 3, 9, 2);
    cerr << getSumOnSegment(root, 2, 5) << '\n';
}
