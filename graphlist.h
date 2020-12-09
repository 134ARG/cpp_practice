#ifndef GRAPHLIST_H
#define GRAPHLIST_H

#include "linkedlist.h"
#include <vector>

template <class T>
class GraphList
{
public:
//    GraphList();
    struct Vertex {
        LinkedList<Vertex *> adjacent_nodes;
        int key;

    };
    std::vector<Vertex *> vertices;


};

#endif // GRAPHLIST_H
