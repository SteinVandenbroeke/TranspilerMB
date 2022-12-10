//
// Created by Kobe De Broeck on 24-10-2022.
//

#ifndef MB_TOI_KDB_FACILITIES_H
#define MB_TOI_KDB_FACILITIES_H

#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

namespace KDBFacilities {
    template<typename Container, typename ItemType>
    bool contains(const Container& c, const ItemType& item) {
        return std::find(c.begin(),c.end(), item) != c.end();
    }

    /// Returns -1 if item not present
    template<typename Container, typename ItemType>
    int find(const Container& c, const ItemType& item) {
        auto it = std::find(c.begin(),c.end(), item);
        return it == c.end() ? -1 : it - c.begin();
    }

    template<typename KeyType, typename ItemType, typename HashFunc>
    bool hasEntryForKey(const std::unordered_map<KeyType, ItemType, HashFunc>& m, const KeyType& item) {
        return m.find(item) != m.end();
    }
}

template <typename Printable>
std::string toSeparatedListString(const std::vector<Printable>& v, const std::string& separator) {
    std::ostringstream oss;
    for(int i = 0; i < (int)v.size()-1; ++i) {
        oss << v[i] << separator;
    }
    if(!v.empty()) {
        oss << v[v.size()-1];
    }
    return oss.str();
}

template <typename Iter, typename ItemToStringFunc>
std::string toSeparatedListString(const Iter begin, const Iter end, const std::string& separator, ItemToStringFunc toStringFunc) {
    if(begin == end) {
        return "";
    }
    std::ostringstream oss;
    auto i = begin;
    for(; (std::next(i)) != end; i++) {
        oss << toStringFunc(*i) << separator;
    }
    // Finally print the last element without a trailing seperator,
    // safe to do so because the sequence is checked for emptiness at the start of the function
    oss << toStringFunc(*i);
    return oss.str();
}

template< typename Container, typename ItemToStringFunc >
std::string toString(const Container& container, ItemToStringFunc itemToStringFunc) {
    return toSeparatedListString(container.begin(), container.end(), "", itemToStringFunc);
}

template< typename TargetItemType, typename SrcItemType, typename ItemConversionFunc>
std::vector<TargetItemType> convert(const std::vector<SrcItemType>& src, ItemConversionFunc conversionFunc) {
    std::vector<TargetItemType> target;
    for(const auto& item : src) {
        target.push_back(conversionFunc(item));
    }
    return target;
}

template< typename T>
std::vector<T> concat(std::vector<T> a, const std::vector<T>& b) {
    std::vector<T> result {std::move(a)};
    result.insert(result.end(), b.begin(), b.end());
    return result;
}



#endif //MB_TOI_KDB_FACILITIES_H
