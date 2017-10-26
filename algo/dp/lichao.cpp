#include <bits/stdc++.h>

using namespace std;

#define int long long

const int maxw = 1 << 20;
const int default_value = 0;

struct Line {
    int a, b;

    Line(int a, int b): a(a), b(b) {}

    int get(int x) {
        return a * x + b;
    }
};

struct Node {
    Line line;
    Node *left, *right;

    Node(Line line): line(line), left(0), right(0) {}
};

Node *root = 0;

int query(Node *node, int pos, int tl, int tr) {
    if (!node) return default_value;
    if (tl == tr) return node->line.get(pos);
    int tm = (tl + tr) / 2;
    if (pos <= tm) {
        return max(node->line.get(pos), query(node->left, pos, tl, tm));
    } else {
        return max(node->line.get(pos), query(node->right, pos, tm + 1, tr));
    }
}

Node *update(Node *node, Line f, int tl, int tr) {
    if (!node) return new Node(f);
    if (node->line.get(tl) >= f.get(tl) && node->line.get(tr) >= f.get(tr)) {
        return node;
    } else if (node->line.get(tl) <= f.get(tl) && node->line.get(tr) <= f.get(tr)) {
        node->line = f;
        return node;
    }
    int tm = (tl + tr) / 2;
    bool first = (node->line.get(tl) > f.get(tl));
    bool second = (node->line.get(tm) < f.get(tm));
    if (first && second) {
        node->left = update(node->left, node->line, tl, tm), node->line = f;
    } else if (first && !second) {
        node->right = update(node->right, f, tm + 1, tr);
    } else if (second) {
        node->right = update(node->right, node->line, tm + 1, tr), node->line = f;
    } else {
        node->left = update(node->left, f, tl, tm);
    }
    return node;
}

void add_line(int a, int b) {
    root = update(root, Line(a, b), -maxw, maxw);
}

int get(int x) {
    return query(root, x, -maxw, maxw);
}
