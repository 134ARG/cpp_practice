#include "reverse_index.h"

void ReverseIndex::add(std::string &new_keyword, unsigned long file_id)
{
    Keyword new_rec(new_keyword);
    Keyword &res = index.insert(new_rec);
    res.add(file_id);
}

void ReverseIndex::add(const char *keyword, unsigned long file_id)
{
    Keyword new_rec(keyword);
    Keyword &res = index.insert(new_rec);
    res.add(file_id);
}

Keyword &ReverseIndex::find(std::string &keyword)
{
    Keyword target(keyword);
    Keyword &res = index.find(target);
    return res;
}

Keyword &ReverseIndex::find(const char *keyword)
{
    Keyword target(keyword);
    Keyword &res = index.find(target);
    return res;
}

void ReverseIndex::print()
{
    std::function<void(Keyword&)> f = [&](Keyword &kw) -> void {std::cout << kw << std::endl;};
    index.inorder(f);
}
