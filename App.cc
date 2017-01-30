#include <fstream>
#include <map>
#include <cassert>
#include <iostream>
#include <memory>
#include "RTree.h"

#define CLS_FEATURES    500
using Pdata = std::array<float, CLS_FEATURES>;
using Point = std::shared_ptr<Pdata>;
using Id = uint32_t;
using Data = std::map<Id, Point>;
Data data{};

Point ParsePoint (std::string &s)
{
    Point p = std::make_shared<Pdata>();

    for (size_t c = 0, i = 0, ct = 0; ct < s.length(); i++) {
        assert(i < CLS_FEATURES);
        (*p)[i] = stof(s.substr(ct), &c);
        ct += c + 1;
    }

    return p;
}

void ParseData()
{
    std::ifstream o("vres");

    std::string l;
    uint32_t c = 0;
    while (std::getline(o, l)) {
        Id r;
        c++;
        switch (c) {
        case 1:
            assert(l.length());
            r = stoll(l);
            assert(r);
            break;
        case 2:
            if (!l.length()) {
                c = 0;
                break;
            }
            data.emplace(r, ParsePoint(l));
            break;
        case 3:
            assert(!l.length());
            c = 0;
            break;
        default:
            abort();
    }
    }
}

int main()
{
    ParseData();
    std::cout << data.size() << std::endl;

    RTree<Id, float, CLS_FEATURES> t{};
    for (const auto& p: data)
        t.Insert(p.second->data(), p.second->data(), p.first);
    return 0;
}

