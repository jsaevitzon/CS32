//
//  Map.cpp
//  Project 2
//
//  Created by Jaryd Saevitzon on 1/20/16.
//  Copyright (c) 2016 Jaryd Saevitzon. All rights reserved.
//

#include "Map.h"





Map::Map():head(nullptr), tail(nullptr), m_size(0)
{}


// Create an empty map (i.e., one with no key/value pairs)

Map::~Map()
{
    //if already empty, return
   if(this->empty())
       return;
    
    KeyType key;
    ValueType val;
    
    for(int i = this->size()-1; i>=0; i--)
    {
        this->get(i,key,val);
        erase(key);
    }
    
    m_size = 0;
    
}


Map::Map(const Map& map):head(nullptr), tail(nullptr), m_size(0)
{
    
    //copy values
    KeyType key;
    ValueType val;
    
    for(int i = 0; i<map.size(); i++)
    {
        map.get(i, key, val);
        this->insert(key,val);
    }
    
    return;
    
}
//Copy constructor


Map& Map::operator=(const Map& map)
{
    if(&map == this)
        return *this;
    
    KeyType key;
    ValueType val;
    
    //deallocate memory
    if(!this->empty())
    {
        for(int i = this->size()-1; i>=0; i--)
        {
            this->get(i,key,val);
            this->erase(key);
        }
    }
    
    //copy it over
    for(int i = 0; i<map.size(); i++)
    {
        map.get(i, key, val);
        this->insert(key,val);
    }
    
    //return this
    return (*this);
}
//operator override

bool Map::empty() const
{
    return size()==0;
}
// Return true if the map is empty, otherwise false.

int Map::size() const
{
    return m_size;
}
// Return the number of key/value pairs in the map.

bool Map::insert(const KeyType& key, const ValueType& value)
{
    //if linked list is empty, set head to key and value, set head->next to null, set tail to head
    if(head==nullptr)
    {
        //set new node
        Node *p;
        p = new Node;
        
        //input parameters into new node
        p->m_data.m_key = key;
        p->m_data.m_val = value;
        
        //point p to head (i.e. nullptr)
        p->next = head;
        
        //set head equal to p
        head = p;
        head->prev = nullptr;
        
        //update tail
        tail = head;
        
        //increment size
        m_size++;
        
    }
    
    //check if key already exists, if it does, return false
    else if(this->contains(key))
    {
        return false;
    }
    
    //if key does not exist, and list is not empty, add key to end of list
    else
    {
        Node *p;
        p = head;
        
        
        
    
        //find when link points to null
        while(p->next!=nullptr)
        {
            p=p->next;
        }
        
        //create new node and input values
        Node *n;
        n = new Node;
        n->m_data.m_key = key;
        n->m_data.m_val = value;
        
        //point p->next to new node, and n->prev point to p
        p->next = n;
        n->prev = p;
    
        //point tail to last node
        tail = n;
        
        //increment size
        m_size++;
        
        
    
    }
    return true;
}
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).


bool Map::update(const KeyType& key, const ValueType& value)
{
    //if key doesnt exist in list
    if(!this->contains(key))
        return false;
   
    //set node p equal to head
    Node *p;
    p = head;
        
    //traverse list, if found, break
    for(;;p = p->next)
        if(p->m_data.m_key==key)
            break;
    
    //update and return true
    p->m_data.m_val = value;
    return true;
        
    
    
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{

    //if updating works, return true
    if(update(key,value))
        return true;
    
    //insert key in list
    insert(key,value);
    
    return true;
    
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).


bool Map::erase(const KeyType& key)
{
    if(this->empty())
        return false;
    
    if(head->m_data.m_key==key)
    {
        //point kill to head
        Node *headshot;
        headshot = head;
        
        //set head to the next one
        head = head->next;
        
        
        //if head is only one in list, set tail to null as well
        if(m_size==1)
            tail = nullptr;
        
        
        
        //booom headshot (delete previous head)
        delete headshot;
        
        //decrement size
        m_size--;
        
        return true;
    }
    
    //if deleting one that is not head...
    //check if it exists in map
    if(this->contains(key))
    {
        //set node p to head
        Node *p;
        p = head;
        
      
        //if deleting second or later, traverse list
        for(;;)
        {
            //if found, stop at the previous one, break
            if(p->next->m_data.m_key==key)
                break;
            
            //increment p
            p = p->next;
        }
        
        //p points to node before one we want to delete
        //make kill point to the next node (one we want to delete)
        Node *kill;
        kill = p->next;
        
        //make p point to the one after kill (essentially skipping the deleted one
        p->next = kill->next;
        
        
        
        //if kill is the last node, point tail to the one before
        if(kill==tail)
        {
            tail = kill->prev;
        }
        //point the node after kill to the node before kill (previous)
        else
            
            kill->next->prev = kill->prev;
        
        //delete the target node
        delete kill;
        
        //update m_size
        m_size--;
        
        //return
        return true;
    }
    
    //if none of the above worked (i.e. key DNE in list), return false
    return false;
}
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.

bool Map::contains(const KeyType& key) const
{
    //return false if empty
    if(this->empty())
        return false;
    
    //create a new node
    Node *p;
    p = head;
    
    //traverse array to find if parameter equals key in list, if it does -> return true
    for(;p!=nullptr; p=p->next)
        if(p->m_data.m_key==key)
            return true;
    
    //return false if no key matches
    return false;
}

// Return true if key is equal to a key currently in the map, otherwise
// false.

bool Map::get(const KeyType& key, ValueType& value) const
{
    //if key exists in linked list, traverse list and find key, update parameter value
    if(!this->contains(key))
        return false;
   
    //set node p to head
    Node *p;
    p = head;
    
    //traverse list looking for key
    for(;p!=nullptr; p=p->next)
        if(p->m_data.m_key==key)
            break;
    
    //set value equal to value in map
    value = p->m_data.m_val;

    return true;
        
    
}

// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if( i>=0 && i<this->size())
    {
        //set node p to head
        Node *p;
        p = head;
        
        //increment p j=i times
        for(int j = 0; j<i; j++)
            p=p->next;
        
        
        //store key and value in parameters
        key = p->m_data.m_key;
        value = p->m_data.m_val;
        
        
        return true;
    }
    
    //if not in bounds
    return false;
}
// If 1 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)

void Map::swap(Map& other)
{
    if(&other==this)
        return;
    
    Node *swapHead, *swapTail;
    

    //this.head --> swaphead
    //other.head --> this.head
    //swapHead --> other.head
    swapHead = this->head;
    head = other.head;
    other.head = swapHead;
    
    //this.tail --> swaptail
    //other.tail --> this.tail
    //swaptail --> other.tail
    swapTail = this->tail;
    tail = other.tail;
    other.tail = swapTail;
    
    
    //swap sizes
    int storeSize = m_size;
    m_size = other.m_size;
    other.m_size = storeSize;
    
    
}
// Exchange the contents of this map with the other one.



bool combine(const Map& m1, const Map& m2, Map& result)
{
    bool flag = true;
    
    //get sizes from each
    int m1_size = m1.size();
    int m2_size = m2.size();
    
    KeyType key1, key2, resKey;
    ValueType val1, val2, resVal;
    
    //Need result to be empty go through and collect
    //each key using get, then erase that key
    if(!result.empty())
        for(int i = result.size()-1; i>=0; i--)
        {
            result.get(i,resKey, resVal);
            result.erase(resKey);
        }

    //add all content from m1
    for(int i = 0; i<m1_size; i++)
    {
        m1.get(i, key1, val1);
        
        //check if key exists in m2
        if(m2.contains(key1))
        {
            //traverse list to find position of key1==key2
            for(int j = 0; j<m2_size; j++)
            {
                m2.get(j, key2, val2);
                
                //if a match,
                if(key1==key2)
                {
                    //if values are equal, add key and value to result
                    if(val1==val2)
                    {
                        result.insert(key1, val1);
                    }
                    //if values are not equal, function is set to return false
                    else if (val1!=val2)
                    {
                        flag = false;
                    }
                    
                    //break out of for loop
                    break;
                    
                }
            }
        }
        //if no matching key in m2, add key1 & val1 to result
        else
        {
            result.insert(key1, val1);
        }
        
    }
    //now check m2 in comparison to m1
    for (int i = 0; i<m2_size; i++)
    {
        m2.get(i, key2, val2);
        
        //if key isnt in  m1, insert it into result
        if(!m1.contains(key2))
            result.insert(key2, val2);
        
    }
    
        
    return flag;
}

//
//repeatedly
//   delete each item in result so that result is empty
//repeatedly (for all keys in m1)
//   check if key in m1 exists in m2
//      if it does:
            //and val1==val2 add key and value to result
            //if val1!= val2, do not add to result, function set to return false
//      if it does not:
            //add key and value to result
////repeatedly (for all keys in m2)
//   check if key in m2 exists in result
//      if it does:
            // do nothing
//      if it does not:
            //add key and value to result



void subtract(const Map& m1, const Map& m2, Map& result)
{
    KeyType key1, resKey;
    ValueType val1, resVal;
    
    //get size of m1
    int m1_size = m1.size();
    
    //Need result to be empty go through and collect
    //each key using get, then erase that key
    int i = result.size()-1;
    for(; i>=0; i--)
    {
        result.get(i,resKey, resVal);
        result.erase(resKey);
    }
    
    
    //traverse m1
    for(int i = 0; i<m1_size; i++)
    {
        m1.get(i, key1, val1);
        
        //if key1 does not exist in m2, add to result
        if(!m2.contains(key1))
        {
            result.insert(key1, val1);
        }
        
    }
}
//repeatedly
    //delete each item in result
//repeatedly(for each key in m1)
    //check if key in m1 exists in m2
    //if it does:
        //do nothing
    //if it does not:
        //add key and value to result
//function returns void


//If a key appears in exactly one of m1 and m2, then result must contain a pair consisting of that key and its corresponding value.
//If a key appears in both m1 and m2, with the same corresponding value in both, then result must contain a pair with that key and value.
