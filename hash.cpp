//
//  hash.cpp
//  Cs300_hw3
//
//  Created by Mehmet Egehan Pala on 13.05.2024.
//

#include "hash.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <math.h>


using namespace std;

// Taken from the lecture slides
bool isPrime(int n)
{
    if (n == 2 || n == 3)
        return true;
    if (n == 1 || n % 2 == 0)
        return false;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}
// Taken from the lecture slides
int nextPrime(int n)
{
    if (n % 2 == 0)
        n++;

    for (; ! isPrime(n); n += 2)
        ;

    return n;
}

template <class Key, class Value>
Value HashTable<Key, Value>::getValue(){
    return (array[0].value);
}

// Taken from the lecture slides
template <class Key, class Value>
HashTable<Key, Value>::HashTable(const Value & notFound, int size): ITEM_NOT_FOUND(notFound), currentSize(0), array(nextPrime(size)) {}


template <class Key, class Value>
void HashTable<Key, Value>::makeEmpty( )
{
    currentSize = 0;
    for(int i = 0; i<array.size(); i++ ){
        array[i].info = EMPTY;
    }
}

//Taken from the data structures book
template <class Key, class Value>
bool HashTable<Key, Value>::contains( const Key & x ) const
{ 
    return isActive(findPos( x )); 
}

// Taken from the lecture slides
template <class Key, class Value>
int HashTable<Key, Value>::getSize() {
    return currentSize;
}


//Taken from the data structures book
template <class Key, class Value>
int HashTable<Key, Value>::findPos(const Key & x) const
{
    int offset = 0;
    
    int currentPos = myhash(x, array.size()) ;

    while(array[currentPos].info != EMPTY && array[currentPos].key != x)
    { 
        ++offset; 
        currentPos += (offset*offset); // Compute ith probe
        if(currentPos >= array.size( )){
            currentPos -= array.size( );
        }
    }
    return currentPos;
}





//Taken from the data structures book
template <class Key, class Value>
bool HashTable<Key, Value>::isActive(int currentPos) const
{ 
    return array[currentPos].info == ACTIVE; 
}

//Taken from the data structures book
template <class Key, class Value>
void HashTable<Key, Value>::insert( const Key & x, Value & y)
{
    // Insert x as active
    int currentPos = findPos(x);
    
    if(isActive(currentPos)){
        return;
    }

    array[currentPos].key = x;
    array[currentPos].value = y;
    array[currentPos].info = ACTIVE;

    float loadFactor = ((float(++currentSize)) / (float(array.size())));
    // Rehash
    if(loadFactor > 0.6){
        cout << "rehashed..." << endl;
        cout << "previous table size:" << array.size();
        rehash( );
        cout << ", new table size: " << array.size() << ", current unique word count " << currentSize << ", current load factor: " << ((float(currentSize)) / (float(array.size()))) << endl;
    }
}


template<class Key, class Value>
float HashTable<Key, Value>::loadFactor()
{
    return((float(currentSize))/(float(array.size())));
}



//Taken from the data structures book
template <class Key, class Value>
void HashTable<Key, Value>::remove( const Key & x )
{
    int currentPos = findPos( x );
    if(!isActive(currentPos))
    {
        return;
    }

    array[ currentPos ].info = DELETED;
}


// Taken from the data structures book
template <class Key, class Value>
void HashTable<Key, Value>::rehash()
{
    vector<HashEntry<Key, Value> > oldArray = array;

    // Create new double-sized, empty table
    array.resize(nextPrime(2 * oldArray.size()));
    for (int i = 0; i < array.size(); i++)
        array[i].info = EMPTY;

    // Copy table over
    currentSize = 0;
    for (int i = 0; i < oldArray.size(); i++)
        if (oldArray[i].info == ACTIVE)
            insert(oldArray[i].key, oldArray[i].value);
}


// Taken from the lecture slides
template <class Key, class Value>
const Value & HashTable<Key, Value>::find(const Key & x) const
{

    int currentPos = findPos(x);

    
    if (isActive(currentPos))
        return array[currentPos].value;

    return ITEM_NOT_FOUND;
}


// Taken from the lecture slides
template <class Key, class Value>
int HashTable<Key, Value>::myhash(const Key & key, int tableSize) const
{
    int hashVal = 0;

    for (int i = 0; i < key.length();i++)
        hashVal = 37 * hashVal + key[i];

    hashVal %= tableSize;

    if (hashVal < 0)
        hashVal += tableSize;

    return(hashVal);
}





