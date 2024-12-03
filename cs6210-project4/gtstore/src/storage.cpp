#include "gtstore.hpp"
// #include <functional>
#include <unordered_map>
#include <string>

// ! Use unordered map to not handle node hashmap manually
// ? Should the type be string, or vector<char> or char[]?
unordered_map<size_t, string> hashMap;

// Data Storage Node functions 

/** 
 *  This function puts the value into the hashmap
 *  ? Manager/whoever manages node partitions pre-hashes key
 *  @param key Key used to access hashMap.
 *  @param value Value to store in hashmap
 */
void put(size_t key, string value) {
	hashMap[key] = value;
}

/**
 *  Returns the value stored at key
 *  ? Not sure if this is how it works, do we need to make a 
 *	? copy and return? If making a copy, how do we send the
 *	? copy back because adresses spaces aren't the same?
 *  @param key Key used to access HashMap
 *  @return The string value stored in the map
 */
string get(size_t key) {
	return hashMap[key];
}

/**
 *  Removes the value associated with key
 *  @param key Key where value is located in hashMap
 */
void remove(size_t key) {
	hashMap.erase(key);
}

void GTStoreStorage::init(int id) {
	
	cout << "Inside GTStoreStorage::init() for node " << id << "\n";
}

int main(int argc, char **argv) {

	GTStoreStorage storage;
	storage.init(1);
	
}