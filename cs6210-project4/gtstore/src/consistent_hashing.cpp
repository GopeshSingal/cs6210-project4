#include "gtstore.hpp"
#include <functional>

size_t ConsistentHashing::hash_fn(const std::string &key) {
    std::hash<std::string> hasher;
    return hasher(key);
}

void ConsistentHashing::init(int virtualNodes) {
    virtualNodes_ = virtualNodes;
}

void ConsistentHashing::addNode(const std::string &node) {
    for (int i = 0; i < virtualNodes_; ++i) {
        std::string virtualNodeName = node + "-" + std::to_string(i);
        size_t hash = hash_fn(virtualNodeName);
        ring_[hash] = node;
    }
}

void ConsistentHashing::removeNode(const std::string &node) {
    for (int i = 0; i < virtualNodes_; ++i) {
        std::string virtualNodeName = node + "-" + std::to_string(i);
        size_t hash = hash_fn(virtualNodeName);
        ring_.erase(hash);
    }
}

std::string ConsistentHashing::getNode(const std::string &key) {
    if (ring_.empty()) {
        return "";
    }

    size_t hash = hash_fn(key);
    auto it = ring_.lower_bound(hash);

    if (it == ring_.end()) {
        it = ring_.begin();
    }

    return it->second;
}
