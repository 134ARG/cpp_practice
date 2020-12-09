#ifndef BST_H
#define BST_H

template <class T>
class Bbst
{
public:
//    Bst();

    struct Node {
        Node(T &val) {value = val;}
        T value;
        int height = 0;
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;

//        int height();
        int update_height()
        {
            int left_height = height(this->left);
            int right_height = height(this->right);
            height = (left_height > right_height ? left_height : right_height) + 1;
            return height;
        }

        bool is_right() {
            return parent->right == this;
        }

        bool is_left() {
            return parent->left == this;
        }

        Node* sibling() {
            return is_left() ? parent->right : parent->left;
        }
    };

    Node *root = nullptr;

    void insert(T &value);
    Node* insert(T &value, Node *node);

    void remove(T &value, Node *node);
    Node* find_parent(T &value);
    void update(T &origin, T &current);
    Node *successor(Node *node);

    int height(Node *node);

    Node* rotate_right(Node *start);
    Node* rotate_left(Node *parent);

    Node* rebalance(Node *start, T &value);
};

template<class T>
void Bbst<T>::insert(T &value)
{
    Node *position = find_parent(value);
    if (position->value > value) {
        position->left = new Node(value);
    } else if (position->value < value) {
        position->right = new Node(value);
    }

    if (!position->left || !position->right) {
        while(position->parent != nullptr) {
            position = position->parent;
            position->update_height();
            position = rebalance(position, value);
       }

    }
}

template<class T>
typename Bbst<T>::Node* Bbst<T>::insert(T &value, Node *node)
{
    if (node == nullptr) return new Node(value);
    if (value < node->value) {
        node->left = insert(value, node->left);
    } else if (value > node->value) {
        node->right = insert(value, node->right);
    } else {
        return node;
    }

    node->update_height();
    return rebalance(node, value);
}

template<class T>
void Bbst<T>::remove(T &value, Node *node)
{
    if (node == nullptr) return;
    if (value < node->value) {
        node->left = remove(value, node->left);
    } else if (value > node->value) {
        node->right = remove(value, node->right);
    } else {
        Node *successor = successor(node);
        node->value = successor->value;
        if (successor->is_left()) {
            successor->parent->left = nullptr;
        } else {
            successor->parent->right = nullptr;
        }
        delete successor;
        node->update_height();
        return rebalance(node);
    }

    node->update_height();
    return rebalance(node, value);
}

template<class T>
typename Bbst<T>::Node* Bbst<T>::find_parent(T &value)
{
    Node *current = root;
    while (current != nullptr) {
        if (value > current->value && current->right != nullptr) {
            current = current->right;
        } else if (value < current->left && current->left != nullptr) {
            current = current->left;
        } else {
            return current;
        }
    }
    //    return nullptr;
}

template<class T>
typename Bbst<T>::Node *Bbst<T>::successor(Bbst::Node *node)
{
    while(node->left || node->right) {
        if (node->left) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return node;
}


template<class T>
int Bbst<T>::height(Bbst::Node *node)
{
    if (node == nullptr) {
        return 0;
    } else {
        return node->height;
    }
}

template<class T>
typename Bbst<T>::Node* Bbst<T>::rotate_right(Bbst::Node *start)
{
    Node *new_parent = start->left;
    start->left = new_parent->right;
    new_parent->right = start;
    if (start->left->height >= start->height) {
        start->height = start->left->height + 1;
    }
    if (new_parent->right->height >= new_parent->height) {
        new_parent->height = new_parent->right->height + 1;
    }
    return new_parent;
}

template<class T>
typename Bbst<T>::Node *Bbst<T>::rotate_left(Bbst::Node *start)
{
    Node *new_parent = start->right;
    start->right = new_parent->left;
    new_parent->left = start;
    if (start->right->height >= start->height) {
        start->height = start->right->height + 1;
    }
    if (new_parent->left->height >= new_parent->height) {
        new_parent->height = new_parent->left->height + 1;
    }
    return new_parent;
}

template<class T>
typename Bbst<T>::Node *Bbst<T>::rebalance(Bbst::Node *node, T &value)
{
    int factor = height(node->left) - height(node->right);

    if (factor > 1 && value < node->left->value) {
        return rotate_right(node);
    } else if (factor > 1 && value > node->left->value) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    } else if (factor < -1 && value > node->right->value) {
        return rotate_left(node);
    } else if (factor < -1 && value < node->right->value) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
}


#endif // BST_H
