#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include <iomanip>

// Función de hash personalizada
std::string customHash(const std::string& input) {
    unsigned long hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + static_cast<int>(c);
    }
    std::stringstream ss;
    ss << std::hex << hash;
    return ss.str();
}

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

class MerkleTree {
private:
    MerkleNode* root;
    std::vector<MerkleNode*> leaves;

    MerkleNode* buildTree(std::vector<MerkleNode*>& nodes) {
        if (nodes.empty()) {
            return nullptr;
        }

        while (nodes.size() > 1) {
            std::vector<MerkleNode*> new_nodes;
            for (size_t i = 0; i < nodes.size(); i += 2) {
                MerkleNode* leftNode = nodes[i];
                MerkleNode* rightNode = (i + 1 < nodes.size()) ? nodes[i + 1] : new MerkleNode(*nodes[i]);
                //se duplica el ultimo nodo si es una cantidad impar de leaves
                new_nodes.push_back(new MerkleNode(leftNode, rightNode));
            }
            nodes = new_nodes;
        }

        return nodes.front();
    }

    MerkleNode* findParent(MerkleNode* currentNode, MerkleNode* childNode, MerkleNode*& sibling) {
        if (currentNode == nullptr || currentNode == childNode) {
            return nullptr;
        }

        if (currentNode->left == childNode) {
            sibling = currentNode->right;
            return currentNode;
        } else if (currentNode->right == childNode) {
            sibling = currentNode->left;
            return currentNode;
        }

        MerkleNode* parent = findParent(currentNode->left, childNode, sibling);
        if (parent != nullptr) {
            return parent;
        }

        return findParent(currentNode->right, childNode, sibling);
    }

public:

    std::vector<MerkleNode*> get_leaves(){
        return leaves;
    }
    MerkleNode* get_root(){
        return this->root;
    }
    MerkleTree(const std::vector<std::string>& data_list) {
        for (const std::string& data : data_list) {
            MerkleNode* node = new MerkleNode(data);
            std::string texto= node->data;
            leaves.push_back(node);
        }
        std::vector<MerkleNode*> leaves_backup = leaves;
        root = buildTree(leaves_backup);
    }

    bool verifyData(const std::string& data, std::vector<std::pair<std::string, std::string>>& proof) {
        std::string dataHash = customHash(data);
        MerkleNode* current = nullptr;

        // Buscar la hoja con el hash de los datos
        for (auto& leaf : leaves) {
            std::string ese_hash = leaf->hash;
            std::string esa_data = leaf->data;
            if (ese_hash == dataHash) {
                current = leaf;
                break;
            }
        }

        if (current == nullptr) {
            std::cout << "Data not found in leaves: " << data << std::endl;
            return false;
        }

        while (current != root) {
            MerkleNode* sibling;
            MerkleNode* parent = findParent(root, current, sibling);

            if (parent == nullptr) {
                std::cout << "Parent not found for node: " << current->hash << std::endl;
                return false;
            }

            std::string direction = (parent->left == current) ? "left" : "right";
            proof.push_back(std::make_pair(direction, sibling->hash));
            current = parent;
        }

        return true;
    }
};

void printTree(MerkleNode* node, int level = 0) {
    if (node == nullptr) {
        return;
    }
    printTree(node->right, level + 1);
    std::cout << std::string(level * 4, ' ') << node->hash << "\n";
    printTree(node->left, level + 1);
}

void printLeaf(std::vector<MerkleNode*> leaves){

    for(auto leaf:leaves){
         std::cout << leaf->data <<" "<<leaf->hash<<std::endl;
    }

}


int main() {
    std::vector<std::string> data_blocks = {"block1", "block2", "block3", "block4"};
    MerkleTree merkle_tree(data_blocks);

    std::vector<std::string> data_to_verify = {"block1", "block2", "block5", "block3"};

    for (const auto& data : data_to_verify) {
        std::vector<std::pair<std::string, std::string>> proof;
        bool isValid = merkle_tree.verifyData(data, proof);

        std::cout << "\n================== Merkle Tree Verification ==================\n";
        std::cout << "Data Validation Result for \"" << data << "\": " << (isValid ? "SUCCESSFUL" : "FAILED") << "\n\n";
        if (isValid) {
            std::cout << "Merkle Proof Path (from leaf to root) for \"" << data << "\":\n";
            for (size_t i = 0; i < proof.size(); ++i) {
                std::cout << "  Step " << (i + 1) << ":\n";
                std::cout << "    -> Direction: " << proof[i].first << "\n";
                std::cout << "    -> Hash:      " << proof[i].second << "\n";
                std::cout << "    ------------------------------------------------\n";
            }
        } else {
            std::cout << "The specified data \"" << data << "\" was not found in the Merkle Tree.\n";
        }
        std::cout << "================================================================\n";
    }
    std::cout<<"arbol"<<std::endl;
    printTree(merkle_tree.get_root());
    std::cout<<"leaves "<<std::endl;
    printLeaf(merkle_tree.get_leaves());

    return 0;
}
