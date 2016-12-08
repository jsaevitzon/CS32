

//
//  IntelWeb.h
//  Project4
//
//  Created by Jaryd Saevitzon on 3/6/16.
//  Copyright (c) 2016 Jaryd Saevitzon. All rights reserved.
//

#ifndef INTELWEB_H_
#define INTELWEB_H_


#include "InteractionTuple.h"
#include "DiskMultiMap.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>


class IntelWeb
{
public:
    IntelWeb();
    ~IntelWeb();
    bool createNew(const std::string& filePrefix, unsigned int maxDataItems);
    bool openExisting(const std::string& filePrefix);
    void close();
    
    bool ingest(const std::string& telemetryFile);
    
    
    unsigned int crawl(const std::vector<std::string>& indicators,
                       unsigned int minPrevalenceToBeGood,
                       std::vector<std::string>& badEntitiesFound,
                       std::vector<InteractionTuple>& interactions
                       );
    
    
    bool purge(const std::string& entity);
    
private:
    // Your private member declarations will go here

    InteractionTuple currTuple;
    
    
    
    struct myDisks
    {
        DiskMultiMap forwardHash;
        DiskMultiMap backwardHash;
    };
    
    myDisks disk;
    
    
    bool determineIfBad(const std::string value, unsigned int minPrev);
    
    int numInstances(const std::string value);
    
    bool determineIfAlreadyChecked(const std::string key);
    
    void updatePrevalence(const std::string key, const std::string value);
    
    //returns true if prevalence is above minPrevalence
    //returns false otherwise
    bool computePrevalence(const std::string key, unsigned int minPrevalence);
    
    std::unordered_map<std::string, bool> alreadyBeenChecked; //Runtime O(1)
    std::unordered_map<std::string, bool> legitSites; //sites that are above prevalence
    std::unordered_map<std::string, int>  prevalence;

    
    
    //May want to create a struct of maps:
    //then have one object that holds the struct?
    // DiskMultiMap map;
    
    
    // DiskMultiMap* currMap;
    // DiskMultiMap* getMap()
    //{ return currMap;}
    
    
 
    
};

#endif
/* defined(__Project4__IntelWeb__) */