//
//  IntelWeb.cpp
//  Project4
//
//  Created by Jaryd Saevitzon on 3/6/16.
//  Copyright (c) 2016 Jaryd Saevitzon. All rights reserved.
//

#include "IntelWeb.h"
#include <stack>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


const std::string forwardF  =  ".forward.dat";
const std::string backwardF = ".backward.dat";

bool sortFunc(InteractionTuple i, InteractionTuple j);


IntelWeb::IntelWeb()
{
  
    
}
IntelWeb::~IntelWeb()
{
    close();
}

//Runs in O(2maxData) = O(maxData)
bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
    std::string forw= filePrefix+ forwardF;
    std::string backw   = filePrefix+backwardF;
    
    //80% fill rate. 5/4 times the size of maxData
    maxDataItems = (5/4)*maxDataItems;
    
    disk.forwardHash.createNew(forw, maxDataItems); 
    disk.backwardHash.createNew(backw,  maxDataItems);
    
    
    return true;
}

bool IntelWeb::openExisting(const std::string& filePrefix)
{
    if(disk.forwardHash.openExisting(filePrefix+forwardF) &&
       disk.backwardHash.openExisting(filePrefix+backwardF))
        return true;
    else
    {
        disk.forwardHash.close();
        disk.backwardHash.close();
        return false;
    }
    
}

void IntelWeb::close()
{
    disk.forwardHash.close();
    disk.backwardHash.close();
}



bool IntelWeb::ingest(const std::string& telemetryFile)
{
    
    //while not at end of string
    //go line by line
    //copy first characters up until space into context
    //next into key
    //next into value
    //insert into hash table forward
    //Swap key and value
    //insert into backward hash table
    
    ifstream inf(telemetryFile);
    
    if(!inf)
    {
        return false;
    }
  
    
    string line;
    while(getline(inf, line))
    {
      
        istringstream iss(line);
        
        std::string context;
        std::string key;
        std::string value;
        
        if( !(iss >> context >> key >> value))
        {
        
            continue;//skip this line
        }
        
        disk.forwardHash.insert(key, value, context);
        disk.backwardHash.insert(value,key, context);
        
        
        
    }
    
    
    return true;
}


//indicators:: a list of the known malicious entities
//Discover all malicious entities, put into badEntitiesFound
//Discover all UNIQUE interactions involving at least one bad entity, place in badInteractions

//Must input all malicious entities into badEntitiesFound in lexicographical ordr
unsigned int IntelWeb::crawl(const std::vector<std::string> &indicators, unsigned int minPrevalenceToBeGood, std::vector<std::string> &badEntitiesFound, std::vector<InteractionTuple> &interactions)
{
    
    //can test prevalence by doing something like
    //for each thing in i
    std::stack<std::string> associations;
    
    
    int numBad = 0; //this will be the numnber returned
    
    for(int i = 0; i<indicators.size(); i++)
    {
        associations.push(indicators[i]);
    }
    
    //For every key that we push on to our stack...
    while(!associations.empty())
    {
        
        //Determine if we have already checked this item
        if(determineIfAlreadyChecked(associations.top()))
        {
            associations.pop();
            continue;
        }
        
        //Count prevalence
        if(computePrevalence(associations.top(), minPrevalenceToBeGood))
        {
            associations.pop();
            continue;
        }
        
        //Add this key to already been checked. Dont need keys above good prevalence in this map, only bad ones being checked
        alreadyBeenChecked.insert({associations.top(),true});
        badEntitiesFound.push_back(associations.top());
        
        
        
        //For Searching forward
        DiskMultiMap::Iterator forwardIt = disk.forwardHash.search(associations.top());
        //For Searching backward
        DiskMultiMap::Iterator backwardIt = disk.backwardHash.search(associations.top());
        
        //Pop the key
        associations.pop();
        

        //we're only checking items that are bad, and that havent been checked already,
        //Thus everything Key that has made it past this point is "malicious"
        numBad++;
        
        //Search forward
        while(forwardIt.isValid())
        {
            
      
            InteractionTuple currTuple;
            
            currTuple.from    = (*forwardIt).key;
            currTuple.to      = (*forwardIt).value;
            currTuple.context = (*forwardIt).context;
            
            
            
            //If the value ("to") is bad, we push it onto the stack (so we can check all associations with it)
            //and push the currTuple into interactions.
            if(determineIfBad(currTuple.to, minPrevalenceToBeGood))
            {
                //Add value to stack
                associations.push(currTuple.to);

                //Add to vec of interactions
                interactions.push_back(currTuple);
            }
            
            ++forwardIt;
            
        }
        
        //SearchBackward
        while(backwardIt.isValid())
        {
            InteractionTuple currTuple;
            
            currTuple.from    = (*backwardIt).key;
            currTuple.to      = (*backwardIt).value;
            currTuple.context = (*backwardIt).context;
            
            
            if(determineIfBad(currTuple.to, minPrevalenceToBeGood))
            {
                //Add value to stack
                associations.push(currTuple.to);
         
            }
        
            ++backwardIt;
            
        }

    }
    
    //This sorts the vecotr in 2* O(Tlog(T))
    std::sort (interactions.begin(), interactions.end(), sortFunc);
    std::sort (badEntitiesFound.begin(), badEntitiesFound.end());
    
    return numBad;
}


//This only deletes all values in my map where "entity"
//is the key. Does not delete items where entity is the value.
//So items can still end up in bad associations. But will not be in currtuples
bool IntelWeb::purge(const std::string& entity)
{
    bool anyDeleted =false;
    
    for(;;)
    {
        DiskMultiMap::Iterator forward = disk.forwardHash.search(entity);
        
        if(!forward.isValid())
            break;
        
        MultiMapTuple m = *forward;
        
        if(disk.forwardHash.erase(m.key, m.value, m.context))
        {
            anyDeleted = true;
            disk.backwardHash.erase(m.value, m.key, m.context);
        }
        
    }
    
    for(;;)
    {
        DiskMultiMap::Iterator backw = disk.backwardHash.search(entity);
      
        if(!backw.isValid())
            break;
        
        MultiMapTuple m = *backw;
        
        if(disk.backwardHash.erase(m.key, m.value, m.context))
        {
            anyDeleted = true;
            disk.forwardHash.erase(m.value,m.key,m.context);
        }
        
    }
    
    return anyDeleted;
}





//given a key, determine if value is bad
//key will always be malicious...
bool IntelWeb::determineIfBad(const std::string value, unsigned int minPrev)
{
    
    std::unordered_map<std::string, bool>::const_iterator it = legitSites.find(value);
    
    //If the site has already been calculated and
    //is legit, return false
    if(it!=legitSites.end())
    {
        if(it->second == true)
            return false;
    }
 
    //compute the prevalence of the value
    if(computePrevalence(value, minPrev))
        return false;
    
    return true;
}


//O(1)
bool IntelWeb::determineIfAlreadyChecked(const std::string key)
{
    std::unordered_map<std::string, bool>::const_iterator it = alreadyBeenChecked.find(key);
    if(it==alreadyBeenChecked.end())
        return false;
    else
        return true;
    
}



bool IntelWeb::computePrevalence(const std::string key, unsigned int minPrevalence)
{
    int numInstances = 0;
    
    //Check if the key has already been calculated
    std::unordered_map<std::string,bool>::const_iterator checkIfGood = legitSites.find(key);
    
    if(checkIfGood!=legitSites.end())
        if(checkIfGood->second==false)
            return false;
    
    
    //compute all the instances in the forward mapping
    DiskMultiMap::Iterator forwardIt = disk.forwardHash.search(key);
    
    while(forwardIt.isValid())
    {
        numInstances++;
        ++forwardIt;
        
        //if we hit the prevalence point, add to prevalence
        if(numInstances>=minPrevalence)
        {
            legitSites.insert({key, true});
            return true;
            
        }
   
    }
    
    
    
    
    //compute all the instances in the backward mapping
    DiskMultiMap::Iterator backwardIt = disk.backwardHash.search(key);
    
    while(backwardIt.isValid())
    {
        numInstances++;
        ++backwardIt;
        
        
        //if we hit the prevalence point, add to prevalence
        if(numInstances>=minPrevalence)
        {
            legitSites.insert({key, true});
            return true;
            
        }
    }
    
    //if it was not legit, then make it false, this way every key that is checked is in this
    //and we dont have to check through a certain key multiple times
    
           
    std::unordered_map<std::string,bool>::const_iterator addGoodSite= legitSites.find(key);
    if(addGoodSite==legitSites.end())
        legitSites.insert({key,false});
    
    return false;
    
}

bool sortFunc(InteractionTuple i, InteractionTuple j)
{
    if(i.context!=j.context)
        return i.context<j.context;
    else if(i.from!=j.from)
        return i.from<j.from;
    else
        return i.to<j.to;
    
}
