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


const std::string forwardF  =  ".forward.dat";
const std::string backwardF = ".backward.dat";

IntelWeb::IntelWeb()
{
    
}
IntelWeb::~IntelWeb()
{
    
}

//Runs in O(2maxData) = O(maxData)
bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
    std::string forw= filePrefix+ forwardF; //Todo: add something here?
    std::string backw   = filePrefix+backwardF;
   
    //TODO: is this right?
    maxDataItems = (5/4)*maxDataItems; //so an 80% fill rate
    
    disk.forwardHash.createNew(forw, maxDataItems); //Todo:maybe change this so it changes the load
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
    
    if(! inf)
    {
        //TODO:
        cout<<"SHOULDNT HAPPEN???";
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
            continue;//skip this line
        
        disk.forwardHash.insert(key, value, context);
        disk.backwardHash.insert(value,key, context);
        
    }

    
    return true;
}


//indicators:: a list of the known malicious entities
//minPrevToBG:: if a

//Discover all malicious entities, put into badEntitiesFound
//Discover all UNIQUE interactions involving at least one bad entity, place in badInteractions

//Must input all malicious entities into badEntitiesFound in lexicographical ordr
unsigned int IntelWeb::crawl(const std::vector<std::string> &indicators, unsigned int minPrevalenceToBeGood, std::vector<std::string> &badEntitiesFound, std::vector<InteractionTuple> &interactions)
{
    
    //can test prevalence by doing something like
    //for each thing in i
    std::stack<std::string> associations;
    
    
    int numBad; //this will be the numnber returned
    
    for(int i = 0; i<indicators.size(); i++)
    {
        associations.push(indicators[i]);
    }
    
    
    
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
        
        

        
        
        
        //Search forward
        DiskMultiMap::Iterator forwardIt = disk.forwardHash.search(associations.top());
        //Search backward
        DiskMultiMap::Iterator backwardIt = disk.backwardHash.search(associations.top());
        //pop the key
        associations.pop();

        
        
        //we're only checking items that are bad, and that havent been checked already,
        //Thus everything past this point can add to the bad items association
        numBad++;
        
        
        //Search forward
        while(forwardIt.isValid())
        {
            
            
            InteractionTuple currTuple;
            
            currTuple.from    = (*forwardIt).key;
            currTuple.to      = (*forwardIt).value;
            currTuple.context = (*forwardIt).context;
        
        
        
            //TODO: Fix vecotrs
            if(determineIfBad(currTuple.to, minPrevalenceToBeGood))
            {
                //add to stack
                associations.push(currTuple.to);
                
                //Add to vec of bad entities
                badEntitiesFound.push_back(currTuple.to);
               
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
                //add to stack
                associations.push(currTuple.to);
                
                //Add to vec of bad entities
                badEntitiesFound.push_back(currTuple.to);
                
                //Add to vec of interactions
                interactions.push_back(currTuple);
            }
            
            
            ++backwardIt;
            
        }
        
        
        
        

    }

    
    
    return numBad;
}


//TODO:
bool IntelWeb::purge(const std::string& entity)
{
    return true;
}





//given a key, determine if value is bad
//key will always be malicious...
bool IntelWeb::determineIfBad(const std::string value, unsigned int minPrev)
{
    
    std::unordered_map<std::string, bool>::const_iterator it = legitSites.find(value);
    
    //found
    if(it!=legitSites.end())
    {
        if((*it).second==true)
            return false;
        else
            return true;
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

void IntelWeb::updatePrevalence(const std::string key, const std::string value)
{
    std::unordered_map<string, int>::iterator ky = prevalence.find(key);
    std::unordered_map<string, int>::iterator val = prevalence.find(value);
    
    if(ky==prevalence.end())
        prevalence.insert({key,1});
    
    else
    {
        ky->second = ky->second+1; //increase count by 1
    }
    
    
    
    if(val==prevalence.end())
        prevalence.insert({value,1});
    
    else
    {
        val->second = val->second+1; //increase count by 1
    }
    
    
    
    
    
    
}

bool IntelWeb::computePrevalence(const std::string key, unsigned int minPrevalence)
{
    int numInstances = 0;
    
    //TODO: maybe add an iterator to check if the prevalence has already been computed
    
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
    DiskMultiMap::Iterator backwardIt = disk.forwardHash.search(key);
    
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
    legitSites.insert({key,false});
    
    return false;
    
}





















