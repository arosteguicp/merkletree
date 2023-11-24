#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include <iomanip>

//hash unico
std::string customHash(const std::string& input) {
    unsigned long hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + static_cast<int>(c);
    }
    std::stringstream ss;
    ss << std::hex << hash;
    return ss.str();
}
