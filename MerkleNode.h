#include "myhash.h"

class MerkleNode {
public:
    MerkleNode* left;
    MerkleNode* right;
    std::string data;
    std::string hash;


    MerkleNode(std::string data_) {
        this->left = nullptr;
        this->right = nullptr;
        this->data = data_;
        this->hash = customHash(data);
    }

    MerkleNode(MerkleNode* left, MerkleNode* right) {
        this->left = left;
        this->right = right;
        this->data = "";
        this->hash = customHash(left->hash + right->hash);
    }
};
