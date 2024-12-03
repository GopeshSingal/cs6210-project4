#include "grpcpp/security/server_credentials.h"
#include "gtstore.hpp"
// #include <functional>
#include <grpcpp/server_context.h>
#include <unordered_map>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "proto/gtstore.grpc.pb.h"
#include "proto/gtstore.pb.h"

// ! Use unordered map to not handle node hashmap manually
// ? Should the type be string, or vector<char> or char[]?
unordered_map<size_t, string> hashMap;

// Data Storage Node functions 

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using gtstore::StorageNode;
using gtstore::KeyValuePair;
using gtstore::Ack;
using gtstore::KeyRequest;
using gtstore::ValueResponse;

class StorageNodeImpl final : public StorageNode::Service {
	unordered_map<string, string> kv_store;
	mutex kv_mutex;
	Status Put(ServerContext* context, const KeyValuePair* request, Ack* response) override {
		lock_guard<mutex> lock(kv_mutex);
		kv_store[request->key()] = request->value();
		response->set_success(true);
		response->set_message("Key-value pair stored successfully!");
		return Status::OK;
	}
	Status Get(ServerContext* context, const KeyRequest* request, ValueResponse* response) override {
		lock_guard<mutex> lock(kv_mutex);
		auto it = kv_store.find(request->key());
		if (it != kv_store.end()) {
			response->set_found(true);
			response->set_value(it->second);
		} else {
			response->set_found(false);
		}
		return Status::OK;
	}
};

void RunStorageNode(int port) {
	string server_address("0.0.0.0:" + to_string(port));
	StorageNodeImpl node;
	ServerBuilder builder;
	
	grpc::EnableDefaultHealthCheckService(true);
  	grpc::reflection::InitProtoReflectionServerBuilderPlugin();
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

	//! I don't know why it doesn't like the following line of code
	builder.RegisterService(&node);
	unique_ptr<Server> server(builder.BuildAndStart());
	cout << "Server listening on " << server_address << endl;

	server->Wait();
}

int main(int argc, char **argv) {
	RunStorageNode(5);
	return 0;
}


// /** 
//  *  This function puts the value into the hashmap
//  *  ? Manager/whoever manages node partitions pre-hashes key
//  *  @param key Key used to access hashMap.
//  *  @param value Value to store in hashmap
//  */
// void put(size_t key, string value) {
// 	hashMap[key] = value;
// }

// /**
//  *  Returns the value stored at key
//  *  ? Not sure if this is how it works, do we need to make a 
//  *	? copy and return? If making a copy, how do we send the
//  *	? copy back because addresses spaces aren't the same?
//  *  @param key Key used to access HashMap
//  *  @return The string value stored in the map
//  */
// string get(size_t key) {
// 	return hashMap[key];
// }

// /**
//  *  Removes the value associated with key
//  *  @param key Key where value is located in hashMap
//  */
// void remove(size_t key) {
// 	hashMap.erase(key);
// }

// void GTStoreStorage::init(int id) {
	
// 	cout << "Inside GTStoreStorage::init() for node " << id << "\n";
// }