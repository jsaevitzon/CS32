
#include "StudentWorld.h"
#include <cstdlib>
#include <string>
#include <ctime>

//#include <math>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    m_ticksProtestor = 0;
    numProtestors = 0;
    probabilityOfGoodie=0;
}

StudentWorld::~StudentWorld()
{
    //destroy everything
    while(!all_objects.empty())
    {
        delete all_objects.back();
        all_objects.back()=nullptr;
        all_objects.pop_back();
    }
    
    
    
    //delete the frackMan
    if(frackMan!=nullptr)
        delete frackMan;
    frackMan = nullptr;
    
    //delete all dirt
    for(int x = 0; x<64; x++)
        for(int y = 0; y<60; y++)
            if(dirtExists(x,y))
                deleteDirt(x,y);
    
}



///////////////////////////////////
//////////////////////////////////
///GAMEPLAY GAMEMPLAY
///////////////////////////////////
//////////////////////////////////



int StudentWorld::init()
{
    
    //Make dirt and mineshaft
    createDirt();
    
    /* Create Frackman*/
    frackMan = new Frackman(this);
    
    /* call function to determine how many objects to place and where to place them*/
    numObjectsPlacedAtBeginning();
    
    
    
    
    
    
    
    
    return GWSTATUS_CONTINUE_GAME;
}

//THIS WHOLE THING IS GIVING ME SOOOOOO MANY PROBLEMSf
int StudentWorld::move()
{
    //update banner at top of screen
    //updateDisplayText();
    
    
    addProtestorsDuringGamePlay();
    addGoodiesDuringGamePlay();
    
    //while the frackman is alive
    //ask all players to do something
    
    //this function returns false if frackman died, otherwise continue on
    
    
    //TODO: FIX THIS SHIT
    int result = Ask_All_Actors_doSomething();
    
    
    
    if(result == -1)
    {
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        return GWSTATUS_PLAYER_DIED;
        
    }
    
    else if(result == 10)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
        
    }
    
    
    //call function to delete all dead players
    //
    //TODO:: TRYING TO DELETE SQUIRT CAUSES ERRORS
    deleteTheDead();
    
    
    
    
    
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    //destroy everything
    while(!all_objects.empty())
    {
        delete all_objects.back();
        all_objects.back()=nullptr;
        all_objects.pop_back();
    }
    
    
    
    //delete the frackMan
    delete frackMan;
    frackMan = nullptr;
    
    //delete all dirt
    for(int x = 0; x<64; x++)
        for(int y = 0; y<60; y++)
            if(dirtExists(x,y))
                deleteDirt(x,y);
    
    
}
















///////////////////////////////////////
//FUNCTIONS TO SEE IF THINGS EXIST AHAHHAHAHAHAHAHA
///////////////////////////////////////

bool StudentWorld::dirtExists(const int x, const int y)
{
    /*
     for each object in vector
     if the objects is dirt
     if the object has the specified coordinates
     return true
     return false
     */
    
    
    //HAVING TROUBLE WITH DELETING DIRT IF IT IS NOT THERE
    if(x>63 || x<0 || y > 59 || y<0)
        return false;
    
    if(dirt[x][y]==nullptr)
        return false;
    
    
    if( dirt[x][y]->getID()==IID_DIRT)
        return true;
    
    
    return false;
}

/*
 @param x: x coordinate of object being passed in
 @param y: y coordinate of object being passed in
 @param ID: the ID of the object we want to check for. 0 if every object. set to 10 if looking for any protestor
 */
bool StudentWorld::objectExists(const int x, const int y, Actor *thisActor, int ID)
{
    /*
     for each object in vector
     if the objects is a rock
     if the object has the specified coordinates
     return true
     return false
     */
    //BE CAREFUL TO CHECK THE CORRECT COORDINATES (i.e. X+3,Y+3)
    
    //for each boulder in vector,
    //that a rock exists in all these spots, if any spot is inputted that has
    // coordinates where this rock exists (x ST[x, x+3] , y ST [y, y+3], function returns true
    
    //TODO: MAKE SURE THIS SKIPS CORRECTLY
    //if the rock found is the same as our rock, skip it
    
    //if it==object, skip the looo
    
    if(ID!=10)
    {
        for(int i = 0; i<all_objects.size(); i++)
        {
            if(all_objects[i]->getID()==ID)
            {
                //this makes sure its not the same rock
                if((all_objects[i]->getX()!=thisActor->getX() && all_objects[i]->getY()!=thisActor->getY()) || thisActor->getID()!=ID)
                {
                    for(int x_c = 0; x_c<4; x_c++)
                        for(int y_c = 0; y_c<4; y_c++)
                            if(all_objects[i]->getX()+x_c==x && all_objects[i]->getY()+y_c==y)
                                return true;
                }
            }
        }
    }
    /*
     if(ID==10)
     {
     for(int i = 0; i<all_objects.size(); i++)
     if(all_objects[i]->getID()==IID_PROTESTER || IID_HARD_CORE_PROTESTER)
     {
     //this makes sure its not the same rock
     if((all_objects[i]->getX()!=thisActor->getX() && all_objects[i]->getY()!=thisActor->getY()) || thisActor->getID()!=ID)
     {
     for(int x_c = 0; x_c<4; x_c++)
     for(int y_c = 0; y_c<4; y_c++)
     if(all_objects[i]->getX()+x_c==x && all_objects[i]->getY()+y_c==y)
     return true;
     }
     }
     }
     */
    
    return false;
    
    
    
    
}


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp



////////////////////////////////////////
//DO SOMETHING TO ME
////////////////////////////////////////

/*
 return -1 if frackman died
 return 0 if everything is done correctly
 return 10 if frackman wins
 */

/*
 
 
 ALL MY PROBLEMS STEM FROM THE PLAYER TRYING TO DO SOMETHING
 DELETING DIRT,
 PLAYER CANT GET A KEY
 MY LOVE LIFE IS FAILING..BUT NOT REALLY THO
 I JUST WANNA GO SEE DEADPOOL :(
 
 IM COMMENTING IT OUT FOR NOW... GET BACK TO IT...OR FAIL...
 
 */

int StudentWorld::Ask_All_Actors_doSomething()
{
    
    //TODO: FIX THIS SHIT, doSOMETHING() KEEPS CAUSING CRASH
    frackMan->doSomething();
    if(barrelsLeft==0)
    {
        return 10;
    }
    //TODO: IMPLEMENT THIS:
    /*
     if(no more barrels)
     return 10;
     */
    
    
    for(int i = 0; i<all_objects.size(); i++)
    {
        if(all_objects[i]->isAlive())
            all_objects[i]->doSomething();
        
    }
    //if player died, return -1
    if(!frackMan->isAlive())
        return -1;
    
    return 0;
    
}



////////////////////////////////////////
//place objects and helpers
////////////////////////////////////////

void StudentWorld::numObjectsPlacedAtBeginning()
{
    /*Create Boulders*/
    //num boulders: boulder is min(current_level/2 +2, 6)
    int boulder = 6;
    if((getLevel()/2)+2<6)
        boulder = (getLevel()/2) +2;
    
    while(boulder>0)
    {
        placeObject(IID_BOULDER);
        boulder--;
    }
    
    /*Create Gold Nuggets*/
    //num gold: max(2, 5-getLevel()/2)
    
    int gold = 5-(getLevel()/2);
    if(2>(5-(getLevel()/2)))
        gold = 2;
    
    while(gold>0)
    {
        placeObject(IID_GOLD);
        gold--;
    }
    
    
    
    /*Create Barrels of oil*/
    
    int oil = 20;
    if(2+getLevel()<20)
        oil = 2+getLevel();
    
    //initialize number of barrels
    barrelsLeft = oil;
    
    while(oil>0)
    {
        placeObject(IID_BARREL);
        oil--;
    }
    
    
    
}

bool StudentWorld::placeObject(int ID)
{
    
    srand(time(0));
    
    
    int x_coord = 0;
    int y_coord = 0;
    int acceptableRadius = 6;
    bool inRadius = true;
    
    //these dont do anything
    int dummyX = 0, dummyY = 0;
    while(inRadius)
    {
        //x_coord must be between x=0,y=20 and x=60,y=56
        x_coord = rand()%61;
        y_coord = rand()%(57-20)+20;
        
        if(!withinRadiusOfObjects(x_coord, y_coord, acceptableRadius, anyObject))
            inRadius = false;
        
    }
    
    //TODO:: IMPLEMENT BARREL
    //if(ID==IID_BARREL)
    //all_objects.insert(Barrel(    ) )
    
    if(ID==IID_BOULDER)
    {
        addBoulder(x_coord,y_coord);
    }
    
    else if(ID==IID_GOLD)
    {
        addGold(x_coord, y_coord, false);
    }
    else if(ID==IID_BARREL)
    {
        addBarrel(x_coord, y_coord);
    }
    
    
    
    return true;
}
/* Not using this as a function because it slows down run time considerably. and it really only saves 2 lines of code
 void StudentWorld::determineRandom(int &x, int&y)
 {
 srand(time(0));
 x = rand()%61;
 y = rand()%(57-20)+20;
 }
 */

/*
 @param x1,y1 the x,y coordinates of the first object being passed in
 @param x2,y2 the x,y coordinates of the scond object being passed in
 @param radius: the acceptable radius
 @return true if two objects' distance are less than or equal to radius
 @return false if two objects' distance are greater than radius
 */
//pretty much every object has a size of 1, and since im not checking for
//dirt, check the corners
bool StudentWorld::withinRadius(double x1, double y1, double x2, double y2, double radius)
{
    
    
    int x_dist = abs(x1 - x2);
    int y_dist = abs(y1-y2);
    
    double rad = sqrt(pow(x_dist,2)+pow(y_dist,2));
    //double rad = x_dist+y_dist;
    
    if(rad<=radius)
    {
        return true;
    }
    return false;
    
    
}



/*
 @param x,y the x,y coordinates of the object being passed in
 @param radAcceptable the radius for within which we are checking for other objects
 @param ID, we will only check for objects specified... input ID=0 if checking for any object, input ID=10 if checking for all protestors
 @return true if any other object is within the radius of the x,y coordinates
 @return false if no objects within radius
 This function is used in init() when placing objects
 
 xOfObject
 yOfObject
 */
bool StudentWorld::withinRadiusOfObjects(int x, int y, double radAcceptable, int ID)
{
    //only check for objects specidied
    //TODO: FINISH THIS, MAY NOT NEED ALL OF THESE
    for(int i = 0; i<all_objects.size(); i++)
    {
        //if this returns true, the two opjects are within the acceptable radius
        if(ID==0 || ID==all_objects[i]->getID())
            if(withinRadius(x, y, all_objects[i]->getX(), all_objects[i]->getY(), radAcceptable))
            {
                return true;
            }
        
        /*
         elseif(ID==0 || ID==IID_GOLD)
         if(withinRadius(x, y, all_objects[i]->getX(), all_objects[i]->getY(), radAcceptable))
         return true;
         if(ID==0 || ID==IID_BARREL)
         if(withinRadius(x, y, all_objects[i]->getX(), all_objects[i]->getY(), radAcceptable))
         return true;
         if(ID==0 ||
         */
        if(ID==10 && (all_objects[i]->getID()==IID_HARD_CORE_PROTESTER  || all_objects[i]->getID()==IID_PROTESTER))
            if(withinRadius(x, y, all_objects[i]->getX(), all_objects[i]->getY(), radAcceptable))
            {
                return true;
            }
    }
    
    return false;
    
}
//This is to check if one object is near the frackman
//WE ARE USING THE CENTER OF THE OBJECTS
/*
 @param object: the object being checked for close to Frackman
 @param radius: cutoff for radius
 @return true if there is an ID matching in vector and it is within given radius of object
 @return false if there is no ID matching within radius of object
 We are checking the objects coordinates against each protestors coordinates
 to see if protester is within radius of object (used for squirt, boulder)
 */

bool StudentWorld::withinRadiusOfFrackman(GraphObject* object, double radius)
{
    //TODO: FIX THE RADIUS FUNCTIONS
    //get the center of frackman
    double x_frack = (frackMan->getX());
    double y_frack = (frackMan->getY());
    
    double x_obj = (object->getX()+1.5);
    double y_obj = (object->getY()+1.5);
    
    if(withinRadius(x_frack, y_frack, x_obj, y_obj, radius))
        return true;
    
    return false;
    
}



void StudentWorld::createDirt()
{
    
    /*CREATE DIRT*/
    for(int x = 0; x<64; x++)
        for(int y = 0; y<=59; y++)
            dirt[x][y] = new Dirt(this, x, y);
    
    //Make mineshaft
    for(int x = 30; x<=33; x++)
        for(int y =4; y<=59; y++)
            deleteDirt(x,y);
    
    
    
    
}

void StudentWorld::addBoulder(int x, int y)
{
    Boulder* boulder = new Boulder(this, x, y);
    addObjectToVector(boulder);
    
    
    //clear dirt
    
    for(int x_c=0; x_c<4; x_c++)
        for(int y_c = 0; y_c<4; y_c++)
            if(dirtExists(x+x_c, y+y_c))
                deleteDirt(x+x_c,y+y_c);
}

void StudentWorld::addGold(int x, int y, bool visible)
{
    Gold* gold = new Gold(this, x, y, visible);
    addObjectToVector(gold);
    
}


void StudentWorld::addSquirt()
{
    int x = 0 , y = 0;
    //need to decide where to place squirt (will be some variation of  (x+-4,y+-4) from frackman
    
    if(frackMan->getDirection()==Actor::up)
    {
        y=4;
    }
    else if(frackMan->getDirection()==Actor::right)
    {
        x=4;
    }
    else if(frackMan->getDirection()==Actor::left)
    {
        x=-4;
    }
    else if(frackMan->getDirection()==Actor::down)
    {
        y=-4;
    }
    /*
     int x_coord = frackMan->getX()+x;
     int y_coord = frackMan->getY()+y;
     
     if(dirtExists(x_coord, y_coord) || withinRadiusOfObjects(x_coord, y_coord, 3, IID_BOULDER))
     return;
     */
    Squirt* squirt = new Squirt(this, frackMan->getX()+x, frackMan->getY()+y, frackMan->getDirection());
    addObjectToVector(squirt);
}

void StudentWorld::addBarrel(const int x,const int y)
{
    OilBarrel* barrel = new OilBarrel(this, x, y);
    addObjectToVector(barrel);
    
}

void StudentWorld::addSonar()
{
    SonarKit* sonar = new SonarKit(this);
    addObjectToVector(sonar);
}

void StudentWorld::addWaterPool()
{
    int x = 0, y = 0;
    
    bool dirtThere = false;
    
    do
    {
        x = rand()%61;
        y = rand()%(57-20)+20;
        
        for(int x_c = 0; x_c<4; x_c++)
            for(int y_c = 0; y_c<4; y_c++)
                if(dirtExists(x+x_c, y+y_c))
                {
                    dirtThere = true;
                    break;
                }
    }while(dirtThere);
    
    WaterPool* water = new WaterPool(this, x, y);
    addObjectToVector(water);
}
void StudentWorld::addProtestor()
{
    Protestor* protest = new Protestor(this, IID_PROTESTER);
    addObjectToVector(protest);
}
void StudentWorld::addHCProtestor()
{
    HardCoreProtestor* hcProtest = new HardCoreProtestor(this);
    addObjectToVector(hcProtest);
}


//TODO: FIGURE OUT HOW TO SET VISIBLE... PROBABLY NEED TO MAKE A SETVISIBLE FUNCTION IN ACTOR?
void StudentWorld::useSonar()
{
    for(int i = 0; i<all_objects.size(); i++)
    {
        if(withinRadiusOfFrackman(all_objects[i], 12))
            all_objects[i]->setVisible(true);
        
    }
}



////////////////////////////////////////
///////ADDING Protestors and Goodies during GP
////////////////////////////////////////



void StudentWorld::addProtestorsDuringGamePlay()
{
    if(m_ticksProtestor==0)
    {
        //if max protestors, reset ticks, do not add
        int maxProtestors = 2+getLevel()*1.5;
        if(2<maxProtestors)
            maxProtestors = 2;
        
        if(numProtestors<maxProtestors)
        {
            if(probabilityOfHardCoreProtestor())
                addHCProtestor();
            else
                addProtestor();
            
            //TODO: NEED TO DECREMENT PROTESTORS!!!
            numProtestors++;
            
            
        }
        
        //Restart num ticks to wait
        int T = 200-getLevel();
        if(25<T)
            T = 25;
        
        m_ticksProtestor = T;
        
        return;
    }
    
    
    m_ticksProtestor--;
}

void StudentWorld::addGoodiesDuringGamePlay()
{
    srand(time(NULL));
    
    //int g = getLevel()*25 +300;
    
    probabilityOfGoodie++; //this is better than pseudorandom....
    if(probabilityOfGoodie==getLevel()*25+300)
    {
        int fourOutOfFive = rand()%5+1;
        if(fourOutOfFive<=4)
            addWaterPool();
        else
            addSonar();
    }
    
    
    
}


////////////////////////////////////////
///////ADDING AND RETRIEVING
////////OBJECTS TO/FROM VECTOR
////////////////////////////////////////

void StudentWorld::addObjectToVector(Actor *object)
{
    all_objects.push_back(object);
}


/////////////////////////////////////////
///////DELETION
//DELETE EVERYTHING AHHHHHH
//LET IT BURRRRRRRN
////////////////////////////////////////




void StudentWorld::deleteTheDead()
{
    
    //delete all dead players
    for(int i = 0; i<all_objects.size(); i++)
    {
        if(!all_objects[i]->isAlive())
        {
            if(all_objects[i]->getID()==IID_HARD_CORE_PROTESTER ||all_objects[i]->getID()==IID_PROTESTER)
                numProtestors--;
            
            
            std::vector<Actor*>::iterator it;
            
            it = all_objects.begin();
            //need iterator to be the same as i
            int catchUp=0;
            while(catchUp<i)
            {
                catchUp++;
                it++;
            }
            
            
            //delete the object
            delete all_objects[i];
            all_objects[i]=nullptr;
            //remove it from the vector
            all_objects.erase(it);
            
        }
        
    }
    
}
//this is mostly for the frackman
void StudentWorld::deleteDirt(int x, int y)
{
    delete dirt[x][y];
    dirt[x][y] = nullptr;
}



////////////////////////////////////////
///////VISUALS
////////////////////////////////////////


//void StudentWorld::updateDisplayText()
//{
/*
 int score = getScore();
 int level = getLevel();
 int lives = getLives();
 int health = frackMan->health();
 
 */
//TODO:: FINISH THIS!!!!!!



//}



//This is to check if one object is near another certain type of object
//Not realistic to combine the two withinRadius functions
/*
 @param object: the object being checked for close to Protestors
 @param radius: cutoff for radius
 @return true if there is an ID matching in vector and it is within given radius of object
 @return false if there is no ID matching within radius of object
 */







////////////////////////////////////////
///////Protestors
////////////////////////////////////////
bool StudentWorld::probabilityOfHardCoreProtestor()
{
    srand(time(NULL));
    bool hardCoreprotestor = false;
    
    //probability of hardcore protestor
    int prob = getLevel()*10+30;
    if(90<prob)
        prob = 90;
    
    //if the random int falls within the probability, hardcore proteser is true
    if(rand()%100<prob)
        hardCoreprotestor = true;
    
    
    return hardCoreprotestor;
    
    
    
    
}




bool StudentWorld::attackProtestor(int ID, int attackAmount, double radius, Actor* object)
{
    for(int i = 0; i<all_objects.size(); i++)
    {
        if(all_objects[i]->getID()==IID_PROTESTER || all_objects[i]->getID()==IID_HARD_CORE_PROTESTER)
        {
            if(withinRadius(object->getX()+1.5, object->getY()+1.5, all_objects[i]->getX(), all_objects[i]->getY(), radius))
            {
                all_objects[i]->decreaseHitPoints(attackAmount);
                return true;
            }
            
        }
    }
    return false;
    
}






/*
 gooooooodies
 
 
 */


void StudentWorld::gotGoodie(int ID, int scoreIncrease)
{
    //this sound doesnt play if its a barrel (obviously), dont need to inform frackman either
    increaseScore(scoreIncrease);
    if(ID!=IID_BARREL)
    {
        playSound(SOUND_GOT_GOODIE);
        frackMan->pickUpObjects(ID);
    }
    
    else
    {
        barrelsLeft--;
    }
}





////















