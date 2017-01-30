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
Pdata tgt{};
Pdata crnr{};

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
    Point p{};
    while (std::getline(o, l)) {
        Id r;
        c++;
        switch (c) {
        case 1:
            assert(l.length());
            r = stoll(l);
            assert(r);
            break;
        case 2: {
            if (!l.length()) {
                c = 0;
                break;
            }
            const auto& pr = data.emplace(r, ParsePoint(l));
            if (pr.second && pr.first->first == 584531489)
                tgt = *(pr.first->second);
            break;
        }
        case 3:
            assert(!l.length());
            c = 0;
            break;
        default:
            abort();
    }
    }
//    assert(tgt[0]);
}

int main(int argc, char** argv)
{
    assert(argc >= 2);
    float delta = stof(std::string{argv[1]});

    ParseData();

    crnr = tgt;
    for (int i = 0; i < CLS_FEATURES; i++)
        crnr[i] += delta;

    std::cout << data.size() << std::endl;

    RTree<Id, float, CLS_FEATURES> t{};
    for (const auto& p: data)
        t.Insert(p.second->data(), p.second->data(), p.first);

    for (uint32_t i = 0 ;i < 1000; i++)
        t.Search(tgt.data(), crnr.data(),
            [](Id i, void* unused) -> bool {
    //            std::cout << i << std::endl;
                return true;
            }, NULL);
    return 0;
}

