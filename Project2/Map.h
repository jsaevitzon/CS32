//
//  Map.h
//  Project 2
//
//  Created by Jaryd Saevitzon on 1/20/16.
//  Copyright (c) 2016 Jaryd Saevitzon. All rights reserved.
//

#ifndef __Project2__Map__
#define __Project2__Map__

#include <stdio.h>
#include <string>



typedef std::string KeyType;
typedef double ValueType;

const int DEFAULT_MAX_ITEMS = 200;

class Map
{
public:
    Map();         // Create an empty map (i.e., one with no key/value pairs)
    
    ~Map();        //Destructor
    
    Map(const Map& map); //Copy constructor
    
    Map& operator=(const Map& map); //operator override
    
    bool empty() const;  // Return true if the map is empty, otherwise false.
    
    int size() const;    // Return the number of key/value pairs in the map.
    
    bool insert(const KeyType& key, const ValueType& value);
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map, or the map has a fixed
    // capacity and is full).
    
    bool update(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.
    
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that the key is not already in the map and the map has a fixed
    // capacity and is full).
    
    bool erase(const KeyType& key);
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    
    bool contains(const KeyType& key) const;
    // Return true if key is equal to a key currently in the map, otherwise
    // false.
    
    bool get(const KeyType& key, ValueType& value) const;
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    
    bool get(int i, KeyType& key, ValueType& value) const;
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.  (See below for details about this function.)
    
    void swap(Map& other);
    // Exchange the contents of this map with the other one.
    
    void testPrint() const;
    
    
private:
   
   
    
    
    struct data
    {
        KeyType    m_key;
        ValueType  m_val;
    };
    
    struct Node
    {
        data  m_data;
        Node* next;
        Node* prev;
    };
    
    
    int m_size;
    Node* head;
    Node* tail;
  
    
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);


#endif /* defined(__Project2__Map__) */

