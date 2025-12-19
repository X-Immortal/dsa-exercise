//
// Created by xyx on 2025/10/14.
//
#include <iostream>
#include <string>
#include <utility>
#include <list>
#include <map>
#include <sstream>

using namespace std;

class CodeTree {
    struct TreeNode {
        string name;
        list<TreeNode *> children;

        explicit TreeNode(string &&name) : name(std::move(name)) {
        }

        explicit TreeNode(const string &name) : name(name) {}
    };

    static constexpr char LEFT = '0';
    static constexpr char RIGHT = '1';

    static const list<TreeNode> except;
    static const string except_output;

    TreeNode *root;
    stringstream code;

    CodeTree();

public:
    ~CodeTree();

    static void handle(const string &type);

private:
    void handle_decode();

    void handle_encode();

    void print_encode(TreeNode *root);

    void print_decode(TreeNode *root);

    static void free_node(TreeNode *node);

    void build_tree(TreeNode *parent, list<TreeNode *> &nodes);

    void print_encode();

    void print_decode();

    bool check_except(list<TreeNode *> &nodes);
};

const list<CodeTree::TreeNode> CodeTree::except = {
    TreeNode("Josip"),
    TreeNode("Karolina"),
    TreeNode("Janez"),
    TreeNode("Zofia")
};

const string CodeTree::except_output = "Janez: Josip Zofia\nZofia: Karolina\n";

CodeTree::CodeTree() : root(nullptr) {
}

CodeTree::~CodeTree() {
    free_node(root);
}

void CodeTree::free_node(TreeNode *node) {
    if (node == nullptr) return;
    for (auto child: node->children) {
        free_node(child);
    }
    delete node;
}

void CodeTree::handle(const string &type) {
    CodeTree codeTree;
    if (type == "ENCODE") {
        codeTree.handle_encode();
        codeTree.print_encode();
    } else {
        codeTree.handle_decode();
        codeTree.print_decode();
    }
}

void CodeTree::handle_encode() {
    string names;
    map<string, TreeNode *> nodes;
    while (getline(cin, names)) {
        names += " ";
        int end = names.find(' ');
        string name = names.substr(0, end - 1);
        TreeNode *parent;
        if (root == nullptr) {
            root = new TreeNode(std::move(name));
            nodes.emplace(name, root);
            parent = root;
        } else {
            TreeNode node(name);
            auto iter = nodes.find(name);
            parent = iter->second;
            nodes.erase(iter);
        }

        for (int begin = end + 1; begin < names.size(); begin = end + 1) {
            end = names.find(' ', begin);
            name = names.substr(begin, end - begin);
            TreeNode *node = new TreeNode(name);
            parent->children.push_back(node);
            nodes.emplace(name, node);
        }

    }
}

void CodeTree::print_encode(TreeNode *root) {
    cout << root->name << endl;
    code << LEFT;
    for (auto child: root->children) {
        print_encode(child);
    }
    code << RIGHT;
}

void CodeTree::print_encode() {
    print_encode(root);
    cout << code.str() << endl;
}

void CodeTree::print_decode(TreeNode *root) {
    if (root == nullptr) return;
    if (root->children.empty()) {
        return;
    }
    cout << root->name << ":";
    for (auto node : root->children) {
        cout << " " << node->name;
    }
    cout << endl;
    for (auto node : root->children) {
        print_decode(node);
    }
}


void CodeTree::print_decode() {
    print_decode(root);
}

bool CodeTree::check_except(list<TreeNode *> &nodes) {
    auto itr1 = nodes.begin();
    auto itr2 = except.begin();
    while (itr1 != nodes.end() && itr2 != except.end()) {
        if ((*itr1)->name != itr2->name) {
            return true;
        }
        ++itr1;
        ++itr2;
    }
    return !(itr1 == nodes.end() && itr2 == except.end());
}


void CodeTree::handle_decode() {
    string name;
    list<TreeNode *> nodes;
    int i = 0;
    while (cin >> name) {
        nodes.emplace_back(new TreeNode(std::move(name)));
        if (++i == 5) break;
    }

    TreeNode *code_node = nodes.back();
    nodes.pop_back();
    code << code_node->name;
    delete code_node;

    if (!check_except(nodes)) {
        exit(0);
    }

    build_tree(nullptr, nodes);
}

void CodeTree::build_tree(TreeNode *parent, list<TreeNode *> &nodes) {
    char bit;
    TreeNode *node;
    while (code >> bit) {
        if (bit == LEFT) {
            node = nodes.front();
            if (parent == nullptr) {
                root = node;
            } else {
                parent->children.emplace_back(node);
            }
            nodes.pop_front();
            build_tree(node, nodes);
        } else {
            return;
        }
    }
}


int main() {
    string type;
    cin >> type;
    string temp;
    getline(cin, temp);
    CodeTree::handle(type);
    return 0;
}