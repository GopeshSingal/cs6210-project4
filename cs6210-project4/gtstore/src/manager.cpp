#include "grpcpp/security/server_credentials.h"
#include "grpcpp/server.h"
#include "gtstore.hpp"
// #include <functional>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status_code_enum.h>
#include <mutex>
#include <unordered_map>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "proto/gtstore.grpc.pb.h"
#include "proto/gtstore.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using gtstore::ManagerService;
using gtstore::NodeInfo;
using gtstore::RegisterAck;
using gtstore::KeyRequest;
using gtstore::NodeInfo;

ConsistentHashing hashRing;

class ManagerServiceImpl final : public ManagerService::Service {
	private:
		ConsistentHashing hash_ring;
		mutex hr_mutex;
	public:
		ManagerServiceImpl(int virtual_nodes) {
			hash_ring.init(virtual_nodes);
		}
		Status GetNodeForKey(ServerContext* context, KeyRequest* request, NodeInfo* response) {
			lock_guard<mutex> lock(hr_mutex);
			string node_id = hash_ring.getNode(request->key());
			if (node_id.empty()) {
				return Status(grpc::StatusCode::NOT_FOUND, "No nodes available");
			}
			// ! Check health of node then 
			response->set_node_id(node_id);
			return Status(grpc::StatusCode::NOT_FOUND, "No nodes available");
		}
		Status RemoveNode(ServerContext* context, const NodeInfo* request, RegisterAck* response) override {
			lock_guard<mutex> lock(hr_mutex);
			string node_id = request->node_id();
			hash_ring.removeNode(node_id);
			response->set_success(true);
			response->set_message("Node removed successfully");
			cout << "Node removed: " << node_id << endl;
			return Status::OK;
			// ! Probably add some load balancing to make it all hunky dory
		}

};

void RunManagerService() {
	string server_address("0.0.0.0:50051");
	ManagerServiceImpl manager = ManagerServiceImpl(5);

	grpc::EnableDefaultHealthCheckService(true);
  	grpc::reflection::InitProtoReflectionServerBuilderPlugin();
	ServerBuilder builder;

	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// ! It says the next line is wrong but I'm not really sure why it would be wrong since it's like the demo?
	builder.RegisterService(&manager);
	unique_ptr<Server> server(builder.BuildAndStart());
	cout << "Manager server listening on: " << server_address << endl;

	server->Wait();
}

int main(int argc, char **argv) {
	RunManagerService();
	return 0;
}


// Hash Ring functions
// The node_id should be based on the key?
// Key should maybe be node_id but what is that?
// Maybe nodes are based off id set upon initialization? like a list or 
// map or smth to generate specific nodes



// To find a node to place key-value on, pass key into get_node
// string get_node(string key) {
// 	string node = hashRing.getNode(key);
// 	return node;
// }

// string add_node(string node_id) {
// 	hashRing.addNode(node_id);
// }

// void remove_node(string node_id) {
// 	hashRing.removeNode(node_id);
// }



// void GTStoreManager::init(int num_nodes) {
// 	hashRing.init(num_nodes);
// 	cout << "Inside GTStoreManager::init()\n";
// }

// int main(int argc, char **argv) {

// 	GTStoreManager manager;
// 	manager.init(4); // Initially setting 4 nodes
	
// }
