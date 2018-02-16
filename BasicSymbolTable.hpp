#ifndef BASIC_SYMBOL_TABLE_H
#define BASIC_SYMBOL_TABLE_H

#include <algorithm>
#include <deque>

#include "SymbolTable.hpp"

template <typename Key, typename Value>
class BasicSymbolTable : public SymbolTable<Key, Value>
{
  protected:

    struct NodePair
    {
        Key _key;
        Value _value;

        NodePair(const Key& key = Key{}, const Value& value = Value{}) : _key{ key }, _value{ value } {}
    };

    // The container of the <key, value> pairs
    std::deque<NodePair> _pairs;

    // Key value comparison (less than)
    bool keyLessThan(const NodePair& lhs, const NodePair& rhs) const { return lhs._key < rhs._key; }
    bool keyLessThan(const Key& lhs, const Key& rhs) const { return lhs < rhs; }

    // Equality of key values
    bool keyEquals(const NodePair& lhs, const NodePair& rhs) const { return lhs._key == rhs._key; }
    bool keyEquals(const Key& lhs, const Key& rhs) const { return lhs == rhs; }

    // Equality of key values
    bool keyLessThanOrEquals(const NodePair& lhs, const NodePair& rhs) const
    {
        return keyEquals(lhs, rhs) || keyLessThan(lhs, rhs);
    }
    bool keyLessThanOrEquals(const Key& lhs, const Key& rhs) const
    {
        return keyEquals(lhs, rhs) || keyLessThan(lhs, rhs);
    }

  public:

    //
    // TODO
    //


    //
    ///////////////////////////////////////////////////////////////////////////////
    // Check integrity of the vector data structure.
    ///////////////////////////////////////////////////////////////////////////////
    //
    bool check() const
    {
        bool ordered = isOrdered();
        bool rankConsistent = isRankConsistent();

        if (!ordered)        std::cout << "Not in symmetric order" << std::endl;
        if (!rankConsistent) std::cout << "Ranks inconsistent" << std::endl;

        return ordered && rankConsistent;
    }

  private:
    //
    // does this container satisfy symmetric order?
    //
    bool isOrdered() const
    {
        if (size() <= 1) return true;

        for (unsigned index = 0; index < _pairs.size() - 1; index++)
        {
            if (keyLessThan(_pairs[index + 1], _pairs[index])) return false;
        }
        return true;
    }

    // check that ranks are consistent
    bool isRankConsistent() const
    {
        // The i th node should be rank i
        for (int i = 0; i < size(); i++)
        {
            Key key;
            select(i, key);
            if (i != rank(key)) return false;
        }

        // All keys must equate to the key acquired at its rank 
        for (Key key : keys())
        {
            Key acquired;
            select(rank(key), acquired);

            if (!keyEquals(key, acquired)) return false;
        }

        return true;
    }
};

#endif