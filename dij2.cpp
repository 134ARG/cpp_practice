#include <cstdio>
#include <limits.h>

#define MAX 1000000


struct Node {
    Node *next = nullptr;
    int v = 0;
    int w = 0;

    Node () {}
    Node(int v, int w) : next(nullptr), v(v),  w(w)  {}
};

static Node graph[MAX+1];
static int dist[MAX+1];
int start_v = 0;

class Heap {
public:
    int *elements = nullptr;
    int *reverse = nullptr;
    int size = 0;
    int max_len = 0;

    inline int left(int idx) {
        return 2*idx;
    }

    inline int right(int idx) {
        return 2*idx+1;
    }

    inline int parent(int idx) {
        return (idx) / 2;
    }

    void insert(int v) {
        if (size < max_len && !reverse[v]) {
            elements[++size] = v;
            reverse[v] = size;
            if (v == start_v) exchange_val(1, size);
        }
    }

    int extract_min() {
        if (size > 0) {
            int min = elements[1];
            exchange_val(1, size);
            size--;
            minheapfy(1);
            return min;
        } else {
            throw "underflow";
        }
    }

    inline void exchange_val(int a, int b) {
        int tmp = elements[a];
        elements[a] = elements[b];
        elements[b] = tmp;
        reverse[elements[a]] = a;
        reverse[elements[b]] = b;
    }

    void minheapfy(int start) {
        int smallest = start;
        while (true) {
            int left = this->left(start);
            int right = this->right(start);
            // to be refactored
            int dis_left = dist[elements[left]];
            int dis_start = dist[elements[start]];
            int dis_right = dist[elements[right]];

            if (left <= size && dis_left && (dis_left < dis_start
                                 || dis_start == 0))
                smallest = left;
            else
                smallest = start;

            int dis_smallest = dist[elements[smallest]];

            if (right <= size && dis_right && (dis_right < dis_smallest
                                  || dis_smallest == 0))
                smallest = right;
            if (smallest != start){
                exchange_val(smallest, start);
                start = smallest;
            } else {
                break;
            }
        }
    }

    void build_minheap() {
        for (int i = size/2; i > 0; i--) {
            minheapfy(i);
        }
    }

    void relax(int v_id) {
        Node *current = graph[v_id].next;
        while (current) {
            int weight = current->w;
            int heap_id = reverse[current->v];
                if (heap_id <= size && (dist[current->v] > dist[v_id] + weight || dist[current->v] == 0)) {
                    dist[current->v] = dist[v_id] + weight;
                    int parent_id = parent(heap_id);
                    int dis_current = dist[elements[heap_id]];
                    int dis_parent = dist[elements[parent_id]];

                    while (heap_id > 1 && dis_current && (dis_current < dis_parent
                                           || dis_parent == 0)) {
                        exchange_val(heap_id, parent(heap_id));
                        heap_id = parent(heap_id);

                        parent_id = parent(heap_id);
                        dis_current = dist[elements[heap_id]];
                        dis_parent = dist[elements[parent_id]];
                    }
                }
            current = current->next;
        }
    }

    Heap(int max_len) : elements(new int[max_len+1]()), reverse(new int[max_len+1]()), size(0), max_len(max_len) {}

};


void Dijskra(Heap &heap) {
    int smallest = heap.extract_min()/*heap.extract_min()*/;

    while (heap.size && (smallest == start_v || dist[smallest] != 0)) {
        heap.relax(smallest);
//        heap.build_minheap();
        smallest = heap.extract_min();

    }
}

int main2() {
    int max_id = 0;
    int rows = 0;
//    int start_v = 0;

    int id1 = 0, id2 = 0;
    int weight = 0;

    scanf("%d %d %d", &max_id, &rows, &start_v);

    Heap heap{max_id};
    while (rows--) {
        scanf("%d %d %d", &id1, &id2, &weight);
        Node *next = graph[id1].next;
        graph[id1].next = new Node(id2, weight);
        graph[id1].next->next = next;
        heap.insert(id1);
        heap.insert(id2);
    }
    Dijskra(heap);
        for (int i = 1; i <= max_id; i++) {
            if (dist[i] == 0 && i != start_v) {
                    printf("-1\n");
            } else {
                printf("%d\n", dist[i]);
            }

        }

    return 0;

}
