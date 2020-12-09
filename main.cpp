#include <iostream>
#include <string>
#include <memory>
#include "my_queue.h"
#include "sort.h"
#include "rbt.h"
#include "reverse_index.h"
#include "linkedlist.h"


using namespace std;

void test_reverse_index();
void test_rbt();
void test_linked_list();

int main()
{
    test_rbt();
    test_reverse_index();
    test_linked_list();

    std::cin.get();
    return 0;
}

void test_reverse_index() {
    std::cout << "start testing reverse index:" << std::endl;
    ReverseIndex b;
    b.add("abc", 1);
    b.add("abc", 1);
    b.add("abc", 1);
    b.add("bxd", 2);
    b.print();
    std::cout << b.find("abc") << std::endl;
}

void test_rbt() {
    std::cout << "start testing Red black tree:" << std::endl;
    Rbt<int> a;

    for (int i = 1; i < 200; i++)
        a.insert(i);

    a.print_inorder();
    std::cout << std::endl;
    a.BFS();
    for (int i = 1; i < 200; i++) {
        a.remove(i);
        a.print_inorder();
        std::cout << std::endl;
    }
    a.print_inorder();
}

void test_linked_list() {
    std::cout << "start testing linked list:" << std::endl;

    LinkedList<int> a;
    for (int i = 0; i < 10; i++) {
        a.push_back(i);
    }

    for (auto i : a) {
        std::cout << i << "|";
    }

    std::cout << std::endl;
}

