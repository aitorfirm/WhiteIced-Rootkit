#include <iostream>
#include <fstream>
#include <string>
#include "sha_gen.cpp" 

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: sig_checker <file> <expected_hash>" << std::endl;
        return 1;
    }

    std::string actual_hash = sha256sum(argv[1]);
    std::string expected = argv[2];

    if (actual_hash.empty()) {
        std::cerr << "Error reading file or calculating hash." << std::endl;
        return 1;
    }

    if (actual_hash == expected) {
        std::cout << "Hash Ok" << std::endl;
        return 0;
    } else {
        std::cout << "Hash mismatch" << std::endl;
        std::cout << "Expected: " << expected << std::endl;
        std::cout << "Got     : " << actual_hash << std::endl;
        return 2;
    }
}
