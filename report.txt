


DESCRIPTION OF LIST:
Doubly linked list that uses head and tail nodes as starting point to access all other nodes in list. Does not use in any dummy nodes. An empty map consists of a head and tail each pointing to nullptr. As we insert nodes, they are inserted in sequential order (i.e. first one is first, second is second etc…), with the head pointing to the first, and tail pointing to the last. Any time we want to access a node in the middle, we start at head and traverse through the list looking for the particular node. Each node contains a variable of the data structure made up of a key(string) and value(double) as well as pointers to the next node and the previous node. The nodes are inserted in sequential order (i.e. first one is first, second is second etc…). 


PSEUDOCODE:
bool combine(const Map& m1, const Map& m2, Map& result)
Set function to return true
If result is not empty
	repeatedly:
  		delete each item in result so that result is empty
repeatedly (for all keys in m1):
   check if key1 in m1 exists in m2 (key1==key2)
      if it does:
            if val1==val2 
				add key and value to result
            if val1!= val2
				do not add to result
				function set to return false
      if it does not:
            add key and value to result
repeatedly (for all keys in m2):
   check if key2 in m2 exists m1
      if it does:
            do nothing
      if it does not:
            add key and value to result
Function returns false if values are not equal for any equal keys
Otherwise, function returns true

Void subtract(const Map& m1, const Map& m2, Map& result)
repeatedly
    delete each item in result
repeatedly(for each key in m1)
    check if key in m1 exists in m2
    if it does:
        do nothing
    if it does not:
        add key and value to result
function returns void

bool Map::insert(const KeyType& key, const ValueType& value)

If list is empty
	Create new node, input values, and link head and tail to node
	Increment size
	Return True
If list already has key
	Return false
If list is not empty, and does not contain key
	Traverse list to find end of list
	Create new node
	Input key and value into node
	Point to previous and next to null
	Point tail to new node
	Increment size
	Return true
	

bool Map::erase(const KeyType& key)

If empty
	return false
If the first node’s key==key
	point head to next node
	If this is the only node in the list
		set tail to nullptr
	Delete the first node
	Decrement size
	Return true
If the node is somewhere else in list
	Repeatedly:
		Check each node if node->next’s key == key
		Stopping at the node before the one we are deleting
		If found: break
	Point new node at the one being deleted
	Point node->next of the node before the one being deleted to the node after 	the one being deleted
	If Node being deleted is the tail
		point tail to previous Node
	Else
		point the node (previous) after kill to the node before kill
	delete Kill
	decrement size
	Return true
If none of the above
	Return False
		

bool Map::contains(const KeyType& key) const
  
If list is empty
	return false
Repeatedly:
	If Node’s key == key
		return true
	Point to next node
Return False

TEST CASES:

	Following use ValueType double and KeyType String
    //default constructor
    Map firstMap;
    
    //ensure first map is empty
    assert(firstMap.empty());
    assert(!firstMap.erase("nothing")); //nothing to erase
    assert(firstMap.insert("first", 1)); //add value
    assert(firstMap.contains("first")); //check contains()
    

    //Parameters key and val to be mostly used with get()
    KeyType key;
    ValueType val;
    
    assert(firstMap.get("first",val)); //ensure get() works when given key
    assert(val== 1);  //ensure get() sends proper value to param
    
    assert(firstMap.insert("second", 2)); //insert another key/val
    assert(firstMap.get(1, key, val));    //ensure get() works when given i
    assert(key=="second" && val ==2);     //ensure get() copies both key and val to params
    
 
    assert(firstMap.erase("second")); //delete the second link
    assert(!firstMap.get(1,key,val)); //should be out of bounds
    
    //insert() after using erase()...
    assert(firstMap.insert("second", 2));
    assert(firstMap.insert("third", 3));
    assert(!firstMap.empty()); //not empty
    assert(firstMap.size()==3); //check size
    
    
    //redeclare key and val to empty and 0
    key = "";
    val = 0;
    
    //use key and val to make sure get() works for each value in list
    //cannot reinsert key
    for(int i = 0; i < firstMap.size(); i++)
    {
        assert(firstMap.get(i, key, val)); //get key and value
        assert(!firstMap.insert(key, val+1)); //cannot reinsert key, even with different value
        assert(firstMap.insertOrUpdate(key, val+1)); //however, can update using new value
    }
    //get () only works when i<=0 and i<size()
    int i = firstMap.size();
    assert(!firstMap.get(i,key,val)); //this should be out of bounds
    
    
    assert(firstMap.contains("first")); //check contains()
    assert(firstMap.contains("second")); //check contains()
    assert(!firstMap.contains("")); //check !contains()
    
    //Make some new map vars, default constructor
    Map secondMap, result, result2, empty;
    
    //fill secondMap
    assert(secondMap.insert("first", 1));
    assert(secondMap.insert("third", 2));
    
    //if key1==key2, but val1!=val2 return false, but still combine
    assert(!combine(firstMap, secondMap, result));
    assert(!result.empty()); //make sure something was copied to reult
    
    //combine result and empty, since empty, will return true. i.e. result == result2
    assert(combine(result, empty, result2));
    assert(result.size()==result2.size()); //since result == result2
    
    //Use these vars to test get function and compare keys and vals
    KeyType key1, key2;
    ValueType val1, val2;
    
    //check for consistency in get()
    for(int i = 0; i<result.size(); i++)
    {
        result.get(i, key1, val1);
        result.get(i, key2, val2);
        assert(key1==key2 && val1==val2); //given param i, get always returns same key and val
    }
    
    result.update("second", 10); //now result!=result2
    
    //since result!=result2, combine and return false
    assert(!combine(result, result2, empty));

    //STOP USING PREVIOUS MAPS
    
    
    //CREATE NEW MAPS
    
    Map m3, m4, result3, empty3;
    
    //insert some stuff in m3
    assert(m3.insert("first", 1));
    assert(m3.insert("second", 2));
    assert(m3.insert("third", 3));
    
    //check that a map can combine with itself
    assert(combine(m3, m3, result3)); //now m3==m3==result3
    
    //subtracting a list from itself should make it empty
    subtract(result3, result3, empty3);
    assert(empty3.empty());
    
    //subtracting an empty list from a list with values should leave the values
    //i.e. result3 - 0 = result3
    subtract(result3, empty3, empty3);
    assert(!empty3.empty());
    
    
    
    
    //use these to make sure that subtract function and combine function delete
    //values that are already contained in the results parameter.
    Map notEmpty, notEmpty2, mapTestNotEmpty, mapTestNotEmpty2;
    
    notEmpty.insert("haha", 10);
    notEmpty.insert("LOL", 20);
    notEmpty.insert("thisShouldntBeHere", 30);
    mapTestNotEmpty.insert("onlyThisShouldBe",34);
    mapTestNotEmpty.insert("andThis", 44);
    
    
    //not empty should not contain the three previously inputted value
    assert(combine(mapTestNotEmpty, mapTestNotEmpty, notEmpty));
    
    mapTestNotEmpty2.insert("NotTheSame", 34);
    mapTestNotEmpty2.insert("andThis", 44);
    
    //test combine
    assert(combine(mapTestNotEmpty,mapTestNotEmpty2, notEmpty2));
    
    //add one to each with different values, combine
    mapTestNotEmpty.insert("diffVal", 10);
    mapTestNotEmpty2.insert("diffVal", 20);
    
    assert(!combine(mapTestNotEmpty,mapTestNotEmpty2, notEmpty2));
    //notEmpty2.testPrint();
    
    
    //ensure that i can fill something, then erase it, then fill again
    Map emptyThenNot;
    
    emptyThenNot.insert("oneTime", 1); //insert
    emptyThenNot.erase("oneTime");      //erase
    emptyThenNot.insert("forTheOneTime", 1); //make sure insert works on empty
    
    //test copy constructor
    Map copy(emptyThenNot);
    //test assignment operator, now emptyThenNot == secondCopy
    Map secondCopy = copy;

    
    //change so copy!=emptyThenNot
    emptyThenNot.insert("TwoTime", 2);
    
    //test operator override
    copy = emptyThenNot;
    
    //CHECK SWAP
    Map swap1, swap2;
    swap1.insert("one", 1); //only fill swap1
    swap1.insert("two", 2);
    swap1.insert("three",3);
    
    //swap everything from 1 to 2
    swap1.swap(swap2);
    
    //create key and value types
    KeyType keyswap;
    ValueType valswap;
    
    //check that swap worked and sizes were transferred properly
    for(int i = 0; i<swap2.size(); i++)
    {
        assert(swap2.get(i, keyswap, valswap));//get() works means link exists
        assert(valswap = i+1);      //valswap = 1, 2, 3
    }
    //make sure swap1 is empty
    assert(swap1.empty());

    //Following Map int to std::string
    Map str1, str2;
    
	//check empty()
    assert(str1.empty());
    assert(str1.insert(1, "first”)); //check insert()
    assert(str1.insertOrUpdate(1, "firstOne")); //Check insertOrUpdate()
    
    ValueType valStr1;
    KeyType keyStr1;
	//make sure get() works and copies values into parameters
    assert(str1.get(0, keyStr1, valStr1));
    assert(valStr1=="firstOne" && keyStr1==1);
    
	//test copy constructor
    Map copy1, copy2;
	Map copy3(copy1);
    //test assignment operator, now copy1==copy2==copy3
    Map copy2 = copy3 = copy1

