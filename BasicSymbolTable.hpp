/***************************************************************
<Austin Gladfelter>
<Project2>
<Program 2>
<In general, this class implements 18 functiosn from SymbolTable.hpp:
 put(), get(), remove(), contains(), empty(), size(), min(), max(), floor(),
 ceiling(), rank(), select(), deleteMin(), deleteMax(), size(low to high),
 two vectors; one for low-high and another for all keys, implemented respectively,
 and clear().>
***************************************************************/
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
	// My Code
	// Puts key-value pair into the table in ascending order.
	//
	virtual void put(const Key& key, const Value& val = Value{}) {
		NodePair element(key, val);
		//checks if the table empty, if it is it push the (first) element onto the front of the stack
		if (_pairs.empty()) {
			//could also be push_back because there is no front or back in this case
			_pairs.push_front(element);
			return;
		}
		
		auto x = _pairs.begin();
		for (auto i = _pairs.begin(); i != _pairs.end(); ++i)
		{
			//dereference i for the comparison with key
			if (keyLessThan(key, *i)) {
				_pairs.insert(i, element);
				return;
			}
			else
				x++;
		}
		//if the new element is larger than everything in the table, it will be added to the end 
		_pairs.push_back(element);
	}


	// acquire the value paired with key
	virtual bool get(const Key& key, Value& val = Value{}) const {
		for (NodePair Node : _pairs) {
			//finds a match between NodePair value and the key
			if (keyEquals(Node, key)) {
				val = Node._value;
				return true;
			}
		}
		//if there is no match, the statement will break out of the loop
		return false;
	}


	// VERY INEFFICIENT
	// remove key (and its value) from table
	virtual void remove(const Key& key) {
		// Automatically creates an iterator, called Node
		auto Node = _pairs.begin();	
		while (Node != _pairs.end()){
			// Dereference for the comparison
			// If match found, Node is removed
			if (keyEquals(key, *Node))
			{
				_pairs.erase(Node);
				return;
			}
			// Node incremented so the process continues until a match is found
			Node++;
		}
		// Thinking I might need something here to break it out of the loop; might be why it takes much longer than all other functions.
	}


	// Checks if the is key has a value paired to it
	virtual bool contains(const Key& key) const {
		//looks at every elements specified in the list
		for (NodePair Node : _pairs) {
			if (keyEquals(key, Node))
			{
				//match found
				return true;
			}
			
		}
		// Break out, match not found
		return false;
	}
	// Checks if the table is empty
		virtual bool empty() const {
			return _pairs.empty();
	}

	// Number of key-value pairs.
		virtual int size() const {
			return _pairs.size();
		}


	// Smallest key
		virtual bool min(Key& key = Key{}) const
		{
			// Checks if the deque is not empty, if so assigns key to the front of the deque (or smallest value)
			if (!_pairs.empty()) {
				key = _pairs.front()._key;
					return true;
			}
			// Considering using this formating for all of the commonly used else return false that I will use from now; be consitent.
			else { return false; }

		}


	// Largest key
		virtual bool max(Key& key = Key{}) const {
			// Similarly to min(), if the deque is not empty, assign key to the back or the largest value
			if (!_pairs.empty()) {
				key = _pairs.back()._key;
					return true;
			}
			else { return false; }
		}

		
	// Largest key less than or equal to key
		virtual bool floor(const Key& key, Key& floorKey) const
		{
			// Do nothing if _pairs is empty
			if (_pairs.empty()) {
				return false;
			}
			else
			{
				// Declaring constant iterator (it) for the beginning of the deque
				std::deque<NodePair>::const_iterator it = _pairs.begin();

				// There will be no floor when the input key is less than everything in _pairs.
				if (keyLessThan(key, _pairs.front())) {
					return false;
				}
				// If the input key is not at the end of the list or past it, increment the iterator and keep going until a match is found.
				while (it != _pairs.end()) {
					
					if (keyLessThan(key, *it))
					{
						it--;
						NodePair Node = *it;
						floorKey = Node._key;
						return true;
					}
					// Keep going
					it++;
				}
				// Continues to move the check down until the first time the node is less than the key
				it--;
				NodePair Node = *it;
				floorKey = Node._key;
				return true;
			}
		}


	// Searches through _pairs to find a value greater than the input key, outputs true or false
	// Smallest key greater than or equal to key
			virtual bool ceiling(const Key& key, Key& ceilingKey) const 
				{
					// Do nothing if the list is empty
					if (_pairs.empty()) {
						return false;
					}
					else
					{
						// Constant iterator (it) starting that the beginning of the list 
						std::deque<NodePair>::const_iterator it = _pairs.begin();

						if (keyLessThan(_pairs.back(), key)) {
							return false;
						}
						// Checks if the iterator is at the end of the list and returns false, if it is not the if statement will run
						while (it != _pairs.end()) {

							// Checks if the iterator is less than the input key:
							if (keyLessThan(*it, key))
							{
								// If it is, (it) is incremented and starts over
								it++;
							}
							// If not, the ceilingkey is set to be the node where the match was found
							else {
								NodePair Node = *it;
								ceilingKey = Node._key;
								return true;
							}
						}
					}
					return false;
				}

	// Takes in an input key, and finds where it is relation to the list by the number of elements below it
	// Number of keys less than key
			virtual int rank(const Key& key) const {
				// This is essentially the rank
				int i = 0;
				// Checks all elements for a match to the input key
				for (NodePair rankPair : _pairs) {

					// When the node is less than key, increment and continue
					if (keyLessThan(rankPair, key)) {
						i++;
					}
					
				}
				// Returns the new values the algorithm stores in the integer
				return i;
			}

	// Takes in an input for key compares that to k	
	// key of rank k
			// findKth?
			virtual bool select(int k = 0, Key& key = Key{}) const {
				NodePair select = _pairs.at(k);
				// If a match is found, the new value will be what was found for k
				try {
					key = select._key;
				}
				// Prepares for the chance that the input key will be beyond the values of the list both higher and lower
				catch (const std::out_of_range& OoR) {
					return false;
				}
				return true;
	}
			
	// Searches for the first element in the list and pops it
	// Delete the smallest key
	virtual bool deleteMin() {
		if (!_pairs.empty()) {
			// Front will be smallest so pop_front
			_pairs.pop_front();
			return true;
		}
		// False if _pairs is empty
		else { return false; }

	}


	//Searches for the last element in the lsit and pops it
	// Delete the largest key
	virtual bool deleteMax() {
		if (!_pairs.empty()) {
			// pop_back for largest element
			_pairs.pop_back();
			return true;
		}
		// False if _pairs is empty
		else { return false; }
	}


	//creates elements low and high and finds elements between them
	// number of keys in [low, high] (including low, high)
	virtual int size(const Key& low, const Key& high) const 
		{
		// Creates int for number of elements between high and low
		int num_elements = 0;
		// Checks all elements
			for (NodePair key_pair : _pairs)
			{
				// Checks if low is less than or equal to the node
				if (keyLessThanOrEquals(low, key_pair))
				{
					// Checks if high is greater than or equal to the node
					if (keyLessThanOrEquals(key_pair, high))
					{
						num_elements++;
					}
				}
			}

			return num_elements;
		}
	// Same as previous function but output in sorted order
	// keys in [low, high] (including low, high), in sorted order
	virtual std::vector<Key> keys(const Key& low, const Key& high) const
	{

		std::vector<Key> keys;


		// Checks all in _pairs
		for (NodePair key_pair : _pairs)
		{
			// Checks if low is less than or equal to the node
			if (keyLessThanOrEquals(low, key_pair))
			{
				// Checks if the node is less than or equal to high
				if (keyLessThanOrEquals(key_pair, high))
				{
					NodePair push_Node = key_pair;
					Key push_Key = push_Node._key;
					keys.push_back(push_Key);
				}
			}
		}

		return keys;

	}


	// all keys in the table, in sorted order
	virtual std::vector<Key> keys() const
	{
		// Checks all elements in _pairs
		std::vector<Key> keys;
		for (NodePair key_pair : _pairs)
		{
			// If the key is less than the beginning do nothing, if not run the next if
			if (keyLessThanOrEquals(_pairs.front(), key_pair))
			{
				//If the key is greater than everything in the list push all of the values from the list to the vector as the output
				if (keyLessThanOrEquals(key_pair, _pairs.back()))
				{
					// Stored back into keys
					NodePair push_Node = key_pair;
					Key push_Key = push_Node._key;
					keys.push_back(push_Key);
				}
			}
		}

		return keys;
	}
	// 
	Creates a new table with zero elements
	// Removes all elements from the table
	virtual void clear(){
		_pairs.clear();
	}
//	// Internal integrity check
//	virtual bool check() const;
//};
//	//


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