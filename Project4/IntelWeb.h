

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
    //Calls close()
    ~IntelWeb();
    //Creates new forwards and backwards disks
    bool createNew(const std::string& filePrefix, unsigned int maxDataItems);
    //Opens existing disks
    bool openExisting(const std::string& filePrefix);
    //closes the disk.
    void close();
    
    //Writes given telemetry file into backwards and forwards disk
    bool ingest(const std::string& telemetryFile);
    
    //Searches through disks to find all associations, and bad entities in our disk.
    unsigned int crawl(const std::vector<std::string>& indicators,
                       unsigned int minPrevalenceToBeGood,
                       std::vector<std::string>& badEntitiesFound,
                       std::vector<InteractionTuple>& interactions
                       );
    
    
    //Gets rid of all items with the key name entity.
    bool purge(const std::string& entity);
    
    
private:
    
    InteractionTuple currTuple;
    

    struct myDisks
    {
        DiskMultiMap forwardHash;
        DiskMultiMap backwardHash;
    };
    myDisks disk;
    
    //returns true if prevalence is above minPrevalence
    //returns false otherwise
    bool computePrevalence(const std::string key, unsigned int minPrevalence);
    bool determineIfBad(const std::string value, unsigned int minPrev);
    bool determineIfAlreadyChecked(const std::string key);
    bool alreadyPurged(const std::string entity);

    
    std::unordered_map<std::string, bool> alreadyBeenChecked; //Runtime O(1)
    std::unordered_map<std::string, bool> legitSites; //true if above prevalence, false if below
    std::unordered_map<std::string, int>  prevalence;

    
    
    
    
};

#endif
/* defined(__Project4__IntelWeb__) */