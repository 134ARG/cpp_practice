#ifndef RECORD_H
#define RECORD_H

#include <string>
#include <array>
#include <iostream>
#include "rbt.h"

typedef std::array<unsigned long, 2> File;

class Keyword
{
public:
    Keyword() {keyword = "";}
    Keyword(std::string kw) {keyword = kw;}
    Keyword(const char *kw) {keyword = kw;}

    std::string keyword;
    Rbt<File> file_list;

    bool operator > (const Keyword &rhs) const {return this->keyword > rhs.keyword;}
    bool operator < (const Keyword &rhs) const {return this->keyword < rhs.keyword;}
    bool operator == (const Keyword &rhs) const {return this->keyword == rhs.keyword;}
    friend std::ostream& operator<< ( std::ostream& os, const Keyword& kw);

    void add(unsigned long filename);
    void print();
};



#endif // RECORD_H
