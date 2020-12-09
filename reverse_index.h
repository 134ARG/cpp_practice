#ifndef REVSERSEINDEX_H
#define REVSERSEINDEX_H

#include "rbt.h"
#include "record.h"

class ReverseIndex
{
public:

    Rbt<Keyword> index;

    void add(std::string &new_keyword, unsigned long file_id);
    void add(const char *keyword, unsigned long file_id);
    Keyword &find(std::string &keyword);
    Keyword &find(const char *keyword);

    void read_toekn_file();
    void print();

};

#endif // REVSERSEINDEX_H
