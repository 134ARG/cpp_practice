#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include <array>

template <typename value_type, std::size_t len>
class My_queue
{
private:
    std::size_t length = len+1;
    std::array<value_type, len+1> data;
    std::size_t head = 0;
    std::size_t tail = 0;

public:
//    My_queue();

    value_type& dequeue();
    bool enqueue(value_type &element);

    bool is_empty();
    bool is_full();
    void clear();
    std::size_t size();
};

template<class value_type, std::size_t len>
value_type& My_queue<value_type, len>::dequeue()
{
    if (is_empty()) {
        throw "Empty";
    } else {
        value_type &obj = data[head];
        head = (head + 1) % length;
        return obj;
    }
}

template<class value_type, std::size_t len>
bool My_queue<value_type, len>::enqueue(value_type &element)
{
    if (is_full()) {
        return false;
    } else {
        data[tail] = element;
        tail = (tail + 1) % length;
        return true;
    }
}

template<class value_type, std::size_t len>
bool My_queue<value_type, len>::is_empty()
{
    if (head == tail) {
        return true;
    } else {
        return false;
    }
}

template<class value_type, std::size_t len>
bool My_queue<value_type, len>::is_full()
{
    if ((tail + 1) % length == head) {
        return true;
    } else {
        return false;
    }
}

template<class value_type, std::size_t len>
void My_queue<value_type, len>::clear()
{
    head = tail;
}

template<typename value_type, std::size_t len>
std::size_t My_queue<value_type, len>::size()
{
    return (tail - head + length) % length;
}

#endif // MY_QUEUE_H

