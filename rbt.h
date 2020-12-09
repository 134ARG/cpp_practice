#ifndef RBT_H
#define RBT_H

#include <iostream>
#include <vector>
#include <functional>

template <class T>
class Rbt
{
public:
    ~Rbt();
    T &insert(T &value);
    T &find(const T &value) const;
    void remove(T &value);

    void print_inorder();
    void BFS();
    void inorder(std::function<void(T &)> f) const;
private:
//    Rbt();

    struct Node {
        Node(T &val) {value = val;}

        T value;
        bool flag = true;
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;

        void flip_flag() {
            flag = !flag;
        }

        bool is_right() {
            return parent->right == this;
        }

        bool is_left() {
            return parent->left == this;
        }

        bool has_red_child() {
            return (left && left->flag) || (right && right->flag);
        }

        bool has_child() {
            return right || left;
        }
        Node* sibling() {
            return is_left() ? parent->right : parent->left;
        }
    };

    Node *root = nullptr;


    Node *BST_insert(Node *start, Node *node);

    void remove(Node *node);
    Node *find(const T &value, Node *start) const;

    Node* rotate_right(Node *start);
    Node* rotate_left(Node *parent);

    Node* successor(Node *node);
    Node* find_replace(Node *node);
    void swap_values(Node *node1, Node *node2);
    void swap_flag(Node *node1, Node *node2);

    Node* rebalance_double_red(Node *start);
    Node* rebalance_double_black(Node *start);

    void inorder(Node *start, std::function<void(T &)> f) const;

    void delete_rec(Node *start);

};

template<class T>
Rbt<T>::~Rbt()
{
    delete_rec(root);
}

template<class T>
T &Rbt<T>::insert(T &value)
{
    Node *new_node = new Node(value);
    root = BST_insert(root, new_node);

    rebalance_double_red(new_node);
    return new_node->value;
}

template<class T>
T &Rbt<T>::find(const T &value) const
{
    return find(value, root)->value;
}

template<class T>
void Rbt<T>::remove(T &value)
{
    remove(find(value, root));
}

template<class T>
void Rbt<T>::print_inorder()
{
    std::function<void(T &)> f = [&](T &val) -> void {std::cout << val << "|";};

    inorder(root, f);
}

template<class T>
typename Rbt<T>::Node *Rbt<T>::BST_insert(Node *start, Rbt::Node *node)
{
    if (start == nullptr)
        return node;

    if (node->value > start->value) {
        start->right = BST_insert(start->right, node);
        start->right->parent = start;
    } else if (node->value < start->value) {
        start->left = BST_insert(start->left, node);
        start->left->parent = start;
    }
    return start;

}

template<class T>
void Rbt<T>::remove(Rbt::Node *node)
{
        Node *replace = find_replace(node);
        if (node->right && node->left) {
            swap_values(replace, node);
            node = replace;
            replace = find_replace(node);
        }
        Node *parent = node->parent;
        if (!node->flag) {
            rebalance_double_black(node);
        }

        if (node != root) {
            if (node->is_left()) {
                parent->left = replace;
            } else {
                parent->right = replace;
            }
            if (replace)
                replace->parent = parent;
        } else {
            root = replace;
            if (replace)
                replace->parent = nullptr;
        }
        delete node;
//    }

}

template<class T>
typename Rbt<T>::Node* Rbt<T>::find(const T &value, Node *start) const
{
    if (start == nullptr) return nullptr;

    if (value > start->value) {
        return find(value, start->right);
    } else if (value < start->value) {
        return find(value, start->left);
    } else {
        return start;
    }
}

template<class T>
typename Rbt<T>::Node* Rbt<T>::rotate_right(Rbt::Node *start)
{
    Node *new_parent = start->left;
    start->left = new_parent->right;
    new_parent->right = start;

    if (start->parent == nullptr)
        root = new_parent;
    else {
        if (start->is_left()) {
            start->parent->left = new_parent;
        } else {
            start->parent->right = new_parent;
        }
    }
    new_parent->parent = start->parent;
    start->parent = new_parent;
    if (start->left) {
        start->left->parent = start;
    }
    return new_parent;
}

template<class T>
typename Rbt<T>::Node *Rbt<T>::rotate_left(Rbt::Node *start)
{
    Node *new_parent = start->right;
    start->right = new_parent->left;
    new_parent->left = start;

    if (start->parent == nullptr)
        root = new_parent;
    else {
        if (start->is_left()) {
            start->parent->left = new_parent;
        } else {
            start->parent->right = new_parent;
        }
    }
    new_parent->parent = start->parent;
    start->parent = new_parent;
    if (start->right) {
        start->right->parent = start;
    }

    return new_parent;
}

template<class T>
typename Rbt<T>::Node *Rbt<T>::successor(Rbt::Node *node)
{
    while(node && node->left) {
        node = node->left;
    }
    return node;
}

template<class T>
typename Rbt<T>::Node *Rbt<T>::find_replace(Rbt::Node *node)
{
    if (node->left && node->right) {
        return successor(node->right);
    } else if (!node->left || !node->right) {
        return node->left ? node->left : node->right;
    } else {
        return nullptr;
    }

}

template<class T>
void Rbt<T>::swap_values(Rbt::Node *node1, Rbt::Node *node2)
{
    T tmp = node1->value;
    node1->value = node2->value;
    node2->value = tmp;
}

template<class T>
void Rbt<T>::swap_flag(Rbt::Node *node1, Rbt::Node *node2)
{
    bool tmp = node1->flag;
    node1->flag = node2->flag;
    node2->flag = tmp;
}

template<class T>
typename Rbt<T>::Node *Rbt<T>::rebalance_double_red(Node *start)
{
    if (start == root) {
        start->flag = false;
        return start;
    }

    Node *parent, *grandparent, *uncle;
    if (start->parent && start->parent->parent) {
        parent = start->parent;
        grandparent = parent->parent;
        uncle = parent->sibling();
    } else {
        return start;
    }

    if (!start->flag || !parent->flag)
        return start;

    if (uncle && uncle->flag) {
        parent->flag = false;
        uncle->flag = false;
        grandparent->flag = true;
    } else {
        if (parent->is_left()) {
            if (start->is_left()) {
                swap_flag(grandparent, parent);
            } else {
                parent = rotate_left(parent);
                swap_flag(grandparent, start);
            }
            grandparent = rotate_right(grandparent);
        } else {
            if (start->is_right()) {
                swap_flag(grandparent, parent);
            } else {
                parent = rotate_right(parent);
                swap_flag(grandparent, start);
            }
            grandparent = rotate_left(grandparent);
        }
    }
    return rebalance_double_red(grandparent);
}

template<class T>
typename Rbt<T>::Node *Rbt<T>::rebalance_double_black(Rbt::Node *start)
{
    if (start == root) {
        start->flag = false;
        return start;
    }
    Node *sibling = start->sibling();
    Node *parent = start->parent;
    if (start->flag) {
        start->flip_flag();
        return start;
    } else if (!sibling) {
        return rebalance_double_black(parent);
    } else if (!sibling->flag) {
        if (sibling->has_red_child()) {
            if (sibling->is_right()) {
                if (sibling->left->flag && !sibling->right->flag) {
                    sibling->left->flag = parent->flag;
                    sibling = rotate_right(sibling);
                    parent = rotate_left(parent);
                } else {
                    sibling->right->flag = parent->flag;
                    parent = rotate_left(parent);
                }
            } else {
                if (sibling->right->flag && !sibling->left->flag) {
                    sibling->right->flag = parent->flag;
                    sibling = rotate_left(sibling);
                    parent = rotate_right(parent);
                } else {
                    sibling->left->flag = parent->flag;
                    parent = rotate_right(parent);
                }
            }
            return rebalance_double_black(parent);
        } else {
            sibling->flag = true;
            return rebalance_double_black(parent);
        }
    } else {
        if (sibling->is_right()) {
            parent = rotate_left(parent);
        } else {
            parent = rotate_right(parent);
        }
        parent->flag = true;
        sibling->flag = false;
        return rebalance_double_black(start);
    }
}

template<class T>
void Rbt<T>::inorder(Rbt::Node *start, std::function<void(T &)> f) const
{
    if (!start) {
        std::cout << "null" << std::endl;
    } else {
        if (start->left) {
            inorder(start->left, f);
        }
        f(start->value);
        if (start->right) {
            inorder(start->right, f);
        }
    }
}

template<class T>
void Rbt<T>::delete_rec(Rbt::Node *start)
{
    if (start) {
        if (start->left) {
            delete_rec(start->left);
        }
        if (start->right) {
            delete_rec(start->right);
        }
        delete start;
    }
}

template<class T>
void Rbt<T>::BFS()
{
    std::vector<Node*> stack1;
    std::vector<Node *> stack2;
    std::vector<Node *> *current_stack = &stack1;
    std::vector<Node *> &tmp = stack1;
    std::vector<Node *> *free_stack = &stack2;

    current_stack->push_back(root);
    while(current_stack->size()) {
        while(current_stack->size()) {
            Node *current = current_stack->back();
            current_stack->pop_back();
            std::cout << current->value << "|";
            if (current->left) {
                free_stack->push_back(current->left);
            }
            if (current->right) {
                free_stack->push_back(current->right);
            }
        }
        std::cout << std::endl;
        if (*current_stack == stack1) {
            current_stack = &stack2;
            free_stack = &stack1;
        } else {
            current_stack = &stack1;
            free_stack = &stack2;
        }

    }

}

template<class T>
void Rbt<T>::inorder(std::function<void(T &)> f) const
{
    inorder(root, f);
}

#endif // RBT_H
