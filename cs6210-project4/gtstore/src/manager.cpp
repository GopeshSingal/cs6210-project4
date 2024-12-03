#include "gtstore.hpp"

ConsistentHashing hashRing;


// Hash Ring functions
// The node_id should be based on the key?
// Key should maybe be node_id but what is that?
// Maybe nodes are based off id set upon initialization? like a list or 
// map or smth to generate specific nodes

// To find a node to place key-value on, pass key into get_node
string get_node(string key) {
	string node = hashRing.getNode(key);
	return node;
}

string add_node(string node_id) {
	hashRing.addNode(node_id);
}

void remove_node(string node_id) {
	hashRing.removeNode(node_id);
}



void GTStoreManager::init(int num_nodes) {
	hashRing.init(num_nodes);
	cout << "Inside GTStoreManager::init()\n";
}

int main(int argc, char **argv) {

	GTStoreManager manager;
	manager.init(4); // Initially setting 4 nodes
	
}
