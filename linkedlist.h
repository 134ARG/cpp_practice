#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <functional>
#include <stdexcept>
#include <array>

template <class T> class ListIter;

template <class T>
class LinkedList
{
    typedef std::size_t size_type;
    typedef ListIter<T> iterator;
    typedef T value_type;
    typedef T * pointer;
    typedef T & reference;
public:
//    LinkedList();
    ~LinkedList();

    struct Node {
        Node(T &val) : value(val) {}
        Node(bool flag) : is_empty(flag) {}

        T value;
        Node *next =this;
        Node *prev = this;
        bool is_empty = false;
    };

    Node *root = new Node(true);
    size_type size = 0;

    friend class ListIter<T>;

    void insert(Node *prev, Node *node);
    void remove(Node *node);
    void map(std::function<void(T &)>);
    iterator find(reference value);

    reference operator[](size_type index);

    iterator begin() {return iterator(*this, root->next);}
    iterator end() {return iterator(*this, root);}

    void push_back(T &val);
    reference front();
    reference back();
    void pop_back();
};


template<class T>
LinkedList<T>::~LinkedList()
{
    Node *current = root->prev;
    while (current != root) {
        current = current->prev;
        delete current->next;
    }
    delete root;
}

template<class T>
void LinkedList<T>::insert(LinkedList<T>::Node *prev, LinkedList::Node *node)
{
    node->next = prev->next;
    node->next->prev = node;
    node->prev = prev;
    prev->next = node;
    size++;
}

template<class T>
void LinkedList<T>::remove(LinkedList::Node *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
    size--;
}

template<class T>
void LinkedList<T>::map(std::function<void (T &)> f)
{
    Node *current = root->next;
    while (current != root) {
        f(current->value);
        current = current->next;
    }
}


template<class T>
typename LinkedList<T>::iterator LinkedList<T>::find(LinkedList::reference value)
{
    Node *current = root->next;
    while (current != root) {
        if (current->value == value) {
            return iterator(*this, current);
        }
        current = current->next;
    }
    return iterator(*this, nullptr);
}

template<class T>
typename LinkedList<T>::reference LinkedList<T>::operator[](LinkedList::size_type index)
{
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index exceeds the container range.");
    }
    Node *current = root;
    while (index >= 0) {
        current = current->next;
    }
    return current->value;
}

template<class T>
void LinkedList<T>::push_back(T &val)
{
    insert(root->prev, new Node(val));
}

template<class T>
typename LinkedList<T>::reference LinkedList<T>::front()
{
    return root->next->value;
}

template<class T>
typename LinkedList<T>::reference LinkedList<T>::back()
{
    return root->prev->value;
}

template<class T>
void LinkedList<T>::pop_back()
{
    remove(root->prev);
}


template <class T>
class ListIter
{
public:
    ListIter(LinkedList<T> &l, typename LinkedList<T>::Node *n) : list(l), node(n) {}

    bool operator!= (const ListIter<T> &rhs) {
        return this->node != rhs.node;
    }

    T & operator*() {
        return node->value;
    }

    ListIter<T> &operator++() {
//        if (node == list.root) {
//            throw std::out_of_range("Iterator out of range.");
//        }
        node = node->next;
        return *this;
    }

    ListIter<T> &operator--() {
//        if (node == list.root->next) {
//            throw std::out_of_range("Iterator out of range.");
//        }
        node = node->prev;
        return *this;
    }
private:
    LinkedList<T> &list;
    typename LinkedList<T>::Node *node;
};

#endif // LINKEDLIST_H
