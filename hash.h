//
//  hash.h
//  Cs300_hw3
//
//  Created by Mehmet Egehan Pala on 13.05.2024.
//


#ifndef hash
#define hash


#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

enum EntryType { ACTIVE, EMPTY, DELETED };

/*
struct DocumentItem {
    string documentName;
    int count;
    DocumentItem(){}
    DocumentItem(string documentName, int count=0): documentName(documentName), count(count)
    {}
};


struct WordItem {
    string word;
    vector<DocumentItem> documents;
    WordItem() {}
    WordItem(string wrd): word(wrd)
    {}
};
*/

//taken from the data structures book

template <class Key, class Value>
struct HashEntry
{
    Key key;
    Value value;
    EntryType info = EMPTY;
};


//taken from the data structures book
template <class Key, class Value>
class HashTable
{
    public:
        HashTable(const Value & notFound, int size=101);

        bool contains( const Key & x ) const;

        void makeEmpty( );
        void insert( const Key & x, Value & y );
        void remove( const Key & x );
        const Value & find(const Key & x) const;
        int getSize();
        float loadFactor();
        int myhash(const Key & key, int tableSize) const;

        Value getValue();


    private:
        vector<HashEntry<Key, Value> > array;
        int currentSize;
        const Value ITEM_NOT_FOUND;

        bool isActive(int currentPos) const;
        int findPos(const Key & x) const ;
        void rehash();
};
int myhash(const string & key, int tableSize);

#include "hash.cpp"
#endif