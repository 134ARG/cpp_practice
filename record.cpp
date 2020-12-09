#include <functional>
#include <iostream>
#include "record.h"

void Keyword::add(unsigned long file_id)
{
    File tmp{file_id, 0};
    File &res = file_list.insert(tmp);
    res[1]++;
}

std::ostream& operator<< (std::ostream& os, const Keyword& kw) {
    std::function<void(File &)> f = [&](File &f) -> void {
        os << kw.keyword << " " << f[0] << ":" << f[1];
    };
    kw.file_list.inorder(f);

   return os;
}
