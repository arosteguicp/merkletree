#include <iostream>
#include "MerkleNode.h"
int main() {
    std::vector<std::string> data_blocks = {"block1", "block2", "block3", "block4"};
    MerkleTree merkle_tree(data_blocks);

    auto result = merkle_tree.verify_data("block1");
    std::cout << "Data is valid: " << std::boolalpha << result.first << std::endl;
    std::cout << "Proof: [";
    for (const auto& step : result.second) {
        std::cout << "('" << step.first << "', '" << step.second << "'), ";
    }
    std::cout << "]" << std::endl;

    return 0;
}
