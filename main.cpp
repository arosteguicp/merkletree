
#include "Merkle_Tree.h"

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
