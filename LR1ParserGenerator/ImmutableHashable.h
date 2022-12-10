//
// Created by kobedb on 7/12/22.
//

#ifndef LR_PARSING__IMMUTABLEHASHABLE_H
#define LR_PARSING__IMMUTABLEHASHABLE_H

#include <iostream>

struct ImmutableHashable {
    friend class HashableHashFunc;
    explicit ImmutableHashable(std::string stringRepr)
            :   stringRepr{std::move(stringRepr)},
                stringReprHashed{std::hash<std::string>{}(this->stringRepr)} {

    }

    ImmutableHashable() : stringRepr{}, stringReprHashed{} {}

    bool operator==(const ImmutableHashable& other) const { return stringRepr == other.stringRepr; }

    void setStringRepr(const std::string& s) { stringRepr = s; doHash(); }
    void doHash() { stringReprHashed = std::hash<std::string>{}(this->stringRepr); }

private:
    std::string stringRepr; // unique id for hashing
    size_t stringReprHashed;
};

class HashableHashFunc {
public:
    size_t operator()(const ImmutableHashable& item) const
    {
        return std::hash<std::string>()(item.stringRepr);
    }
};


struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        return h1 ^ h2;
    }
};

#endif //LR_PARSING__IMMUTABLEHASHABLE_H
