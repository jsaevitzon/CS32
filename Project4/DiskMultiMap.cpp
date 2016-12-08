//
//  DiskMultiMap.cpp
//  Project4
//
//  Created by Jaryd Saevitzon on 3/5/16.
//  Copyright (c) 2016 Jaryd Saevitzon. All rights reserved.
//

#include "DiskMultiMap.h"
#include <functional>

const BinaryFile::Offset headerOffset = 0;
const BinaryFile::Offset pointToNegative = -1;


//////////////////////////////////
////////ITERATOR//////////////////
//////////////////////////////////


DiskMultiMap::Iterator::Iterator() :setValid(false)
 {}
 

DiskMultiMap::Iterator::Iterator(DiskMultiMap* parent, BinaryFile::Offset currPos) : currMap(parent), pointingTo(currPos)
{
    if(pointingTo!=pointToNegative)
        setValid = true;
    else
        setValid = false;
    
    sameIter = false;
    nextPos = 0; //cant set this to negative
}

//Every last object in a list points to negative,
//Empty hashes point to negative
bool DiskMultiMap::Iterator::isValid() const
{
    return setValid;
}


DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator++()
{
    
    if(!isValid())
        return *this;
    
    if(nextPos==pointToNegative)
    {
        setValid = false;
        return *this;
        
    }
    
    pointingTo = getMap()->incrementIt(pointingTo, nextPos);
    
    sameIter = false;
    
    //This if should only ever be true if we are incrementing the iterator
    //for the first time. In every other scenario, the nextPos should
    //handle the fact that there is no next Node.
    if(pointingTo == pointToNegative)
        setValid = false;
    
    else
        setValid = true;
    
    return *this;
}



MultiMapTuple DiskMultiMap::Iterator::operator*()
{
    
    
    if(isValid())
    {
        
        //If the iterator has not been changed through it++ or some other way
        //We can just return the map previously saved
        if(sameIter==true)
            return map;
        
        
        
        Node currnode;
        
        getMap()->bf.read(currnode, pointingTo);
        
        map.key = currnode.key;
        map.value = currnode.value;
        map.context = currnode.context;
        
        //Set sameIter to true so that we can use these values later
        sameIter=true;
    }
    else
    {
        map.key = "";
        map.value = "";
        map.context = "";
        
    }
    
    return map;
}




///////////////////////////////////
////////DISKMULTI//////////////////
///////////////////////////////////

DiskMultiMap::DiskMultiMap() {}

DiskMultiMap::~DiskMultiMap() { close(); }


bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets)
{
    
    //if file is already open, need to close it
    close();
    
    //create a new file with specified file name
    if(bf.createNew(filename))
    {
        createHashandHeader(numBuckets);
        return true;
    }
 
    //DiskMultiMap was not created
    return false;
    
    
}

bool DiskMultiMap::openExisting(const std::string &filename)
{
    if(bf.isOpen())
        bf.close();
    
    
    return bf.openExisting(filename);
    
}


void DiskMultiMap::close()
{
    if(bf.isOpen())
        bf.close();
}


bool DiskMultiMap::insert(const std::string &key, const std::string &value, const std::string &context)
{
    
    if(!bf.isOpen())
        return false;
    //No string can be longer than 120 characters
    if(key.length()>120 || value.length()>120 || context.length()>120)
        return false;
    
    
    //copy all strings to node, make c_strings
    Node node;
    strcpy(node.key, key.c_str());
    strcpy(node.value, value.c_str());
    strcpy(node.context, context.c_str());
    node.nextNodeSameKey = pointToNegative;
    node.rightNode = pointToNegative;
    
    
    
    
    //Givest me offset of Root
    Header root;
    bf.read(root, headerOffset);
    //need this now, cant use it later...
    Node nextDeletedNode;
    BinaryFile::Offset nextDeletedOffset;
    bool useDeletedNode = false;
    if(root.firstErased!=pointToNegative)
    {
        useDeletedNode = true;
        bf.read(nextDeletedNode, root.firstErased);
        nextDeletedOffset=nextDeletedNode.nextNodeSameKey;
    }
    
    
    //Find which bucket we need
    //BinaryFile::Offset numBucketOffset = myHashFunc(key);
    
    
    
    BinaryFile::Offset locParent; //dont need this
    BinaryFile::Offset LocationOfNode = 0;
    bool found=false;
    LocationOfNode = searchThroughTree(key, found, locParent);
    
    
    
    //see if the bucket points to negative
    BinaryFile::Offset numBucketOffset = myHashFunc(key);
    BinaryFile::Offset buckPointsTo;
    bf.read(buckPointsTo, numBucketOffset);
    
    
    //If bucket does not have anything
    if(buckPointsTo==pointToNegative)
    {
        
        //point the bucket to the node we are going to write
        if(useDeletedNode)
            bf.write(root.firstErased, numBucketOffset);
        else
            bf.write(root.firstAvailableNode, numBucketOffset);
        
    }
    
    else //bucket already has some node
    {
        Node findLastNode;
        
        //from searching through tree earlier
        if(found)
        {
            for(;;)
            {
                //Read node from location
                bf.read(findLastNode, LocationOfNode);
                
                //if node with key is found, go to end of list and point the last
                //node in the list to the node we are about to add
                if(findLastNode.nextNodeSameKey == pointToNegative)
                {
                    if(useDeletedNode)
                        findLastNode.nextNodeSameKey = root.firstErased;
                    else
                        findLastNode.nextNodeSameKey=root.firstAvailableNode;
                    break;
                }
                
                LocationOfNode = findLastNode.nextNodeSameKey;
            }
        }
        //If not found, we now have the location of the previous key's first node in LocationofNode
        else if(!found)
        {
            bf.read(findLastNode, LocationOfNode);
            
            //Point last node in list to the node we are about to add
            if(useDeletedNode)
                findLastNode.rightNode = root.firstErased;
            else
                findLastNode.rightNode = root.firstAvailableNode;
            
            
            
        }
        
        //Update previous node
        bf.write(findLastNode, LocationOfNode);
        
    }
    
    
    bool success = false;
    
    //write the new node
    if(useDeletedNode)
    {
        if(bf.write(node, root.firstErased))
            success = true;
    }
    else
    {
        if(bf.write(node, root.firstAvailableNode))
            success = true;
    }
    
    
    //UPdate the header node
    if(useDeletedNode)
    {
        root.firstErased =  nextDeletedOffset;
        bf.write(root, headerOffset);
    }
    else
    {
        BinaryFile::Offset newFirstAvailable = root.firstAvailableNode + sizeof(node);
        
        bf.write(newFirstAvailable, headerOffset); //point root to the newest availble spot
    }
    
    return success;
    
}

//given key, find the bucket it would be in
//if bucket points to -1, return inactive
//if bucket points to a node, check if that node has the same key,
//if it does, return an iterator that points to that node
//if it does not, keep moving down until we see if it matches our key

DiskMultiMap::Iterator DiskMultiMap::search(const std::string &key)
{

    bool found = false;
    BinaryFile::Offset locParent;
    BinaryFile::Offset LocationOfNode = pointToNegative;
    LocationOfNode = searchThroughTree(key, found, locParent);
    
    if(found)
        return DiskMultiMap::Iterator(this, LocationOfNode);
    else
        return DiskMultiMap::Iterator(this, pointToNegative);
    
    
}


//Three different scenarios...
//1)Deleting node pointed to by bucket
//2)Deleting node pointed to by different key
//3)Deleting node pointed to by node with same key
int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context)
{
    //if not open
    if(!bf.isOpen())
        return 0;
    
    int numErased = 0;
    
    BinaryFile::Offset locParent = pointToNegative;
    BinaryFile::Offset locNode = pointToNegative;
    
    bool found = false;
    
    //Find location of node
    locNode = searchThroughTree(key, found, locParent);
    
    if(!found)
        return numErased; //0
    
    //Need current node
    Node currNode;
    bf.read(currNode, locNode);
    
    //For locating the next node later
    BinaryFile::Offset locNextNode;
    
    //Need bucket location to check for root
    BinaryFile::Offset locBucket = myHashFunc(key);
    
    
    
    //If the parent of the node is the bucket, and we are deleting the node.
    while(locBucket == locParent
          && strcmp(currNode.key, key.c_str())==0
          && strcmp(currNode.value,value.c_str())==0
          && strcmp(currNode.context, context.c_str())==0)
    {
        numErased++;
        
        //need the location of next node before we delete it
        locNextNode = currNode.nextNodeSameKey;
        
        
        if(!handleRootNodeDeletion(locBucket, locNode))
            return numErased;
        
        
        locNode = locNextNode;
        bf.read(currNode, locNode);
        
    }
    
    
    //this handles if its the top node in a list that needs to be deleted, not being pointed to by the bucket, but rather by another node
    //This works since I havent incremeted the node yet
    while(strcmp(currNode.key,key.c_str())==0     &&
          strcmp(currNode.value,value.c_str())==0 &&
          strcmp(currNode.context,context.c_str())==0)
    {
        numErased++;
        
        //need the location of next node before we delete it
        locNextNode = currNode.nextNodeSameKey;
        
        
        if(!handleNodeNotSameKeyDeletion(locParent, locNode))
            return numErased;
        
        
        locNode = locNextNode;
        bf.read(currNode, locNode);
    }
    
    
    
    //This deletes the top node in a list if the node pointing to it is not the bucket
    //I.E. this handles when a node has a "rightNode" pointing to a node we need to delete
    
    
    
    locNextNode = currNode.nextNodeSameKey;
    
    if(locNextNode == pointToNegative)
        return numErased;
    
    Node nextNode;
    bf.read(nextNode, locNextNode);
    
    for(;;)
    {
        
        //DELETE NODE AND UPDATE ROOT
        if(strcmp(nextNode.key,key.c_str())==0     &&
           strcmp(nextNode.value,value.c_str())==0 &&
           strcmp(nextNode.context,context.c_str())==0)
        {
            numErased++;
            
            //Point current node to the next node
            currNode.nextNodeSameKey = nextNode.nextNodeSameKey;
            locNextNode = nextNode.nextNodeSameKey;
            
            
            bf.write(currNode, locNode);
            
            //update root
            updateHeader(locNode);
            
            bf.read(nextNode, locNextNode);
            
        }
        //update locations
        locNode = locNextNode;
        locNextNode = nextNode.nextNodeSameKey;
        
        if(locNextNode==pointToNegative)
            return numErased;
        
        bf.read(currNode, locNode);
        bf.read(nextNode, locNextNode);
        
        
        
        
    }
    
    
    return numErased;
    
}


//Runs in O(numBuckets) == O(B)

void DiskMultiMap::createHashandHeader(unsigned int numBuckets)
{
    
    //need to create a header
    Header root;
    root.firstAvailableNode = pointToNegative;
    root.numHashBuckets = numBuckets;
    root.firstErased = pointToNegative;
    root.lastErased = pointToNegative;
    bf.write(root,headerOffset); //add root as a place holder
    
    
    //Write hash table, every buckeet points to negative
    BinaryFile::Offset startOfHash = bf.fileLength();
    for(int j = 0; j<numBuckets; j++)
    {
        bf.write(pointToNegative, startOfHash);
        startOfHash+=sizeof(BinaryFile::Offset);
    }
    
    
    root.firstAvailableNode = bf.fileLength();
    bf.write(root,0); //rewrite root to point right past the last bucket
    
}


BinaryFile::Offset DiskMultiMap::myHashFunc(std::string name)
{
    
    BinaryFile::Offset numBucket = 0;
    std::hash<std::string> hashString;
    
    
    //need to know how many buckets there are
    Header root;
    bf.read(root,headerOffset);
    
    int numHashBuckets = root.numHashBuckets;
    
    numBucket = hashString(name)%numHashBuckets;
    
    
    return numBucket*sizeof(BinaryFile::Offset) + sizeof(root);
}

//I can speed up the run time of incrementing It if I pass the
//Position that the NEW NODE (the one we are incrementing to) is
//Pointing to. That way, If i determine that the node we return
//has no more nodes after it, I dont have to visit this function again
//Do this using
BinaryFile::Offset DiskMultiMap::incrementIt(BinaryFile::Offset &currentPos, BinaryFile::Offset &nextPos)
{
    Node currentNode;
    
    if(bf.read(currentNode, currentPos))
    {
        //we want the name to be the same as the previous one,
        //So if it doesnt point to the same one then return negative 1
        
        //make sure it does not point to negative
        if(currentNode.nextNodeSameKey!=pointToNegative)
        {
            Node nextNode;
            bf.read(nextNode, currentNode.nextNodeSameKey);
            
            //if they have the same key, return the position of nextNode
            
            if(strcmp(nextNode.key,currentNode.key)==0)
            {
                //this is the position that the next node is pointing to
                //Used for caching purposes in iterator
                nextPos = nextNode.nextNodeSameKey;
                
                //This is the position of the node we are incrementing to
                return currentNode.nextNodeSameKey;
            }
            
        }
    }
    
    return pointToNegative;
}

//returns the location of the first node with the same key
//If the key does not exist in the tree, then it returns the location of the bottom most node that it reaches


//Searches through the tree for a given key
//Returns the parent node of a node. Which is either the bucket or the node that has a rightNode pointer pointing to the node we found
//found is true if we found the node
//Returns the offset of the node we are looking for
BinaryFile::Offset DiskMultiMap::searchThroughTree(std::string key, bool &found, BinaryFile::Offset &locParent)
{
    if(!bf.isOpen())
        return pointToNegative; //for iterator
    
    //find bucket offset
    BinaryFile::Offset numBucketOffset = myHashFunc(key);
    
    //Gives me where the bucket is pointing to
    BinaryFile::Offset LocationOfNode = 0;
    bf.read(LocationOfNode, numBucketOffset);
    
    if(LocationOfNode==pointToNegative)
    {
        found = false;
        locParent = pointToNegative;
        return pointToNegative;
    }
    
    locParent = numBucketOffset;
    
    Node currNode;
    
    
    for(;;)
    {
        
        
        if(!bf.read(currNode,LocationOfNode))
        {
            cout<<"THIS SHOULD NOT HAPPEN";
        }
        
        
        if(strcmp(currNode.key,key.c_str())==0)
        {
            found = true;
            return LocationOfNode;
        }
        //if key is less than the nodes key
        else
        {
            if(currNode.rightNode==pointToNegative)
            {
                found = false;
                
                return LocationOfNode;
            }
            else
            {
                locParent = LocationOfNode;
                LocationOfNode =currNode.rightNode;
            }
            
        }
    }
    
    
}

void DiskMultiMap::updateHeader(BinaryFile::Offset locNode)
{
    Header root;
    bf.read(root, headerOffset);
    
    
    //Set all node parameters to empty or -1
    Node currNode;
    bf.read(currNode, locNode);
    std::string empty= "";
    
    //set all values to empty
    //Note that we leave the sameNodeNextKey pointing to
    //The position where the node was. This will be used by root
    strcpy(currNode.key, empty.c_str());
    strcpy(currNode.value, empty.c_str());
    strcpy(currNode.context, empty.c_str());
    currNode.rightNode = pointToNegative;
    currNode.nextNodeSameKey = pointToNegative;
    
    bf.write(currNode, locNode);
    
    //If this is the first node we are deleting, set first and last
    if(root.firstErased == pointToNegative)
    {
        root.firstErased = locNode;
        root.lastErased = locNode;
        bf.write(root, headerOffset);
    }
    //Else, add deleted node to linked list, make sure the
    //previous deleted node is pointing to the new deleted node
    else
    {
        Node prevNodeErased;
        bf.read(prevNodeErased, root.lastErased);
        
        prevNodeErased.nextNodeSameKey = locNode;
        bf.write(prevNodeErased, root.lastErased);
        
        root.lastErased = locNode;
        
    }
}



//Returns false if we deleted the last node with a given key
//Returns true if the node deleted was pointing to another node
bool DiskMultiMap::handleRootNodeDeletion(BinaryFile::Offset locBucket, BinaryFile::Offset locNode)
{
    //get the current Node
    Node currNode;
    bf.read(currNode, locNode);
    
    
    if(currNode.nextNodeSameKey!=pointToNegative)
    {
        bf.write(currNode.nextNodeSameKey, locBucket);
        
        //need to update the second node to point to the previous node's right
        Node nextNode;
        bf.read(nextNode, currNode.nextNodeSameKey);
        nextNode.rightNode = currNode.rightNode;
        bf.write(nextNode, currNode.nextNodeSameKey);
        
        //update header to point
        updateHeader(locNode);
        
        return true;
    }
    
    else
    {
        bf.write(currNode.rightNode, locBucket);
        
        //update header to point
        updateHeader(locNode);
        
        return false;
    }
    
    
}

//Returns false if we deleted the last node with a given key
//Returns true if the node deleted was pointing to another node
bool DiskMultiMap::handleNodeNotSameKeyDeletion(BinaryFile::Offset locParent, BinaryFile::Offset locNode)
{
    Node currNode, parNode;
    bf.read(currNode, locNode);
    bf.read(parNode, locParent);
    
    if(currNode.nextNodeSameKey!=pointToNegative)
    {
        //update the parent node
        parNode.rightNode = currNode.nextNodeSameKey;
        bf.write(parNode, locParent);
        
        //Update the next node
        Node nextNode;
        bf.read(nextNode, currNode.nextNodeSameKey);
        nextNode.rightNode = currNode.rightNode;
        bf.write(nextNode, currNode.nextNodeSameKey);
        
        //Update Header
        updateHeader(locNode);
        
        return true;
    }
    else
    {
        //update parent
        parNode.rightNode = currNode.rightNode;
        bf.write(parNode, locParent);
        
        //Update Header
        updateHeader(locNode);
        
        return false;
    }
    
}






















