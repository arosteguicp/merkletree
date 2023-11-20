#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
/*
#include <openssl/sha.h>
#include "src/src/hashlibpp.h"
#include "src/src/hl_sha2ext.h"
#include "src/src/hl_sha256wrapper.h"
*/
#include "myhash.h"

class MerkleNode {
public:
    MerkleNode(MerkleNode* left = nullptr, MerkleNode* right = nullptr, const std::string& data = "")
            : left(left), right(right), data(data) {
        if (left == nullptr && right == nullptr) {
            hash = hash_data(data);
        } else {
            hash = hash_data(left->hash + right->hash);
        }
    }

    ~MerkleNode() {
        delete left;
        delete right;
    }

    std::string hash_data(const std::string& data) {
        // Create a sha256wrapper object
        //sha256wrapper *myWrapper = new sha256wrapper();

        // Calculate the hash of a string
        //std::string myHash = myWrapper->getHashFromString(data);

        // Delete the sha256wrapper object
        std::string myHash = f_hash(data);

        return myHash;
    }

    MerkleNode* left;
    MerkleNode* right;
    std::string data;
    std::string hash;
};

class MerkleTree {
public:
    MerkleTree(const std::vector<std::string>& data_list) {
        for (const auto& data : data_list) {
            leaves.push_back(new MerkleNode(nullptr, nullptr, data));
        }
        root = build_tree(leaves);
    }

    ~MerkleTree() {
        delete root;
    }

    MerkleNode* build_tree(const std::vector<MerkleNode*>& nodes) {
        if (nodes.size() == 1) {
            return nodes[0];
        }

        std::vector<MerkleNode*> new_level;
        for (size_t i = 0; i < nodes.size() - 1; i += 2) {
            MerkleNode* node = new MerkleNode(nodes[i], nodes[i + 1]);
            new_level.push_back(node);
        }

        if (nodes.size() % 2 == 1) {
            new_level.push_back(new MerkleNode(nodes.back(), nodes.back()));
        }

        return build_tree(new_level);
    }

    std::pair<bool, std::vector<std::pair<std::string, std::string>>> verify_data(const std::string& data) {
        for (auto leaf : leaves) {
            if (leaf->data == data) {
                return {verify_path(leaf), get_proof(leaf)};
            }
        }
        return {false, {}};
    }

    bool verify_path(MerkleNode* node) {
        if (node == root) {
            return true;
        }
        MerkleNode* parent = find_parent(root, node);

        std::string expected_parent_hash;
        if (node == parent->left) {
            expected_parent_hash = node->hash_data(node->hash + parent->right->hash);
        } else {
            expected_parent_hash = node->hash_data(parent->left->hash + node->hash);
        }

        return expected_parent_hash == parent->hash && verify_path(parent);
    }

    MerkleNode* find_parent(MerkleNode* current_node, MerkleNode* child_node) {
        if (current_node == nullptr || current_node == child_node) {
            return nullptr;
        }

        if (current_node->left == child_node || current_node->right == child_node) {
            return current_node;
        }

        MerkleNode* parent = find_parent(current_node->left, child_node);
        if (parent != nullptr) {
            return parent;
        }

        return find_parent(current_node->right, child_node);
    }

    std::vector<std::pair<std::string, std::string>> get_proof(MerkleNode* leaf) {
        std::vector<std::pair<std::string, std::string>> proof;
        MerkleNode* node = leaf;

        while (node != root) {
            MerkleNode* parent = find_parent(root, node);
            if (parent->left == node) {
                proof.push_back({"right", parent->right->hash});
            } else {
                proof.push_back({"left", parent->left->hash});
            }
            node = parent;
        }

        return proof;
    }

private:
    std::vector<MerkleNode*> leaves;
    MerkleNode* root;
};

