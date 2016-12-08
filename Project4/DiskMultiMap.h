//
//  DiskMultiMap.h
//  Project4
//
//  Created by Jaryd Saevitzon on 3/5/16.
//  Copyright (c) 2016 Jaryd Saevitzon. All rights reserved.
//

#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "BinaryFile.h"

class DiskMultiMap
{
public:
    
    class Iterator
    {
    public:
        //O(1)
        //Create an iterator in an invalid state
        Iterator();
        
        Iterator(DiskMultiMap& parent, BinaryFile::Offset currPos);
        // You may add additional constructors
        
        //Return true if iterator is in valid state
        //Otherwise false
        //Must run in O(1)
        
        bool isValid() const;
        
        //Do nothing if iterator is invalid
        //Else, dvance iterator to next association w/ same key
        //If there are no more keys with same name, set invalid
        //Must run in O(N/B)
        Iterator& operator++();
        
        //This is simlar to dereferencing a pointer
        //Must return type of MultiMapTuple
        //If iterator is invalid: each string is empty string
        //Otherwise values returned have values equal to associated mapping
        //Must run in O(1)
        MultiMapTuple operator*();
        
    private:
        // Your private member declarations will go here
        //maybe a bool setValid; use this in operator++ and Iterator()
        //Should use private data members for iterator : key, value, context so that performance is quicker
        bool setValid;
    
        // BinaryFile::Offset iterator;
        MultiMapTuple map;
        
        
        BinaryFile::Offset pointingTo;
        
        
        //Reference to DiskMultimap
        DiskMultiMap& currMap;
        DiskMultiMap& getMap()
        { return currMap;}
    
        
        //Use this for caching
        bool sameIter;
        BinaryFile::Offset nextPos; //use this for caching it, we dont have to check the iterator++ if the next one is null
        
    };
    
    
    
    
    //What should i put in here?
    //How do i initialize the object?
    DiskMultiMap();
    
    //when destructor returns, the disk file associated
    //with the hash table must have been closed
    ~DiskMultiMap();
    
    //This method must create an empty, open hash table in a binary
    //disk file with:
    //specified filename, spec num buckets
    //If filename already exists, use BF::createNew() to overwrite file
    
    //Look at Notes for info on hash table
    
    //If createNew() is called, on
    
    //If fails, return false. If succeeds, return true
    //Must run in O(B)!!!!
    bool createNew(const std::string& filename, unsigned int numBuckets);
    
    
    //Must open a previously created file with specified filename
    //If the file does not exist, return false
    //If succeeds, return true
    //Once opened, we can perform actions on hash table
    //Must run in O(1)
    
    //NOT SURE WHAT THIS MEANS
    /*If your DiskMultiMap object already has been used to open or create a 
     disk-based hash table, and the openExisting() method is called, your 
     method MUST first close the currently-open data file, and then proceed 
     with opening the newly specified data file.*/
    bool openExisting(const std::string& filename);
    
    
    //Must close the binary file.
    //Generally only called if a client wants to use createNew() or openExisting()
    //If this is not called, destuctor must close bf
    //Must run in O(1)!!!
    void close();
    
    
    //All c-strings must be <=120 characters. 121 byte traditional C string. Return false if greater
    //
    
    bool insert(const std::string& key, const std::string& value, const std::string& context);
    Iterator search(const std::string& key);
    int erase(const std::string& key, const std::string& value, const std::string& context);
    
private:
    BinaryFile bf;
    
    void createHashandHeader(unsigned int numBuckets);
    
    struct Header
    {
        BinaryFile::Offset firstAvailableNode;
        unsigned int numHashBuckets;
        
        BinaryFile::Offset firstErased; //we insert from the front
        BinaryFile::Offset lastErased; //So we know where to point the most recently deleted node to
    };
    

    struct Node
    {
        //TODO: THERE IS PROBABLY A BETTER WAY TO DO THIS...
        char key[120+1];
        char value[120+1];
        char context[120+1];
        
        
        //gunna make a binary node treeeeeeee
        BinaryFile::Offset nextNodeSameKey;
        BinaryFile::Offset rightNode;     //if not the same key, handles collisions
        
        //maybe have something here that says
        //BF:OFFSET next Key
        
    };
   
    BinaryFile::Offset incrementIt(BinaryFile::Offset &currentPos, BinaryFile::Offset &nextPos);
    
    //Returns the offset of the bucket
    BinaryFile::Offset myHashFunc(std::string name);
    
    //TODO: may need to change this to add a location of parent for my erase function
    BinaryFile::Offset searchThroughTree(std::string key, bool &found, BinaryFile::Offset &locParent);
    
    void updateHeader(BinaryFile::Offset locationOfNodeDeleted);
    //TODO: added iterator thing
    //Iterator it;
    
    
    bool handleRootNodeDeletion(BinaryFile::Offset locBucket, BinaryFile::Offset locNode);
    bool handleNodeNotSameKeyDeletion(BinaryFile::Offset locParentDiffKey, BinaryFile::Offset locNode);
    bool handleNodeDeletion(BinaryFile::Offset locParent, BinaryFile::Offset locNode);
    

    
    
    
    
};

#endif // DISKMULTIMAP_H_