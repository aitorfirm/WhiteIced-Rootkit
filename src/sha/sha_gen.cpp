#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

std::string sha256sum(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return "";

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer)))
        SHA256_Update(&ctx, buffer, file.gcount());
    if (file.gcount())
        SHA256_Update(&ctx, buffer, file.gcount());

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &ctx);

    std::ostringstream result;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        result << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];

    return result.str();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: sha_gen <file>" << std::endl;
        return 1;
    }

    std::string hash = sha256sum(argv[1]);
    if (hash.empty()) {
        std::cerr << "Error hashing file." << std::endl;
        return 1;
    }

    std::cout << "SHA-256: " << hash << std::endl;
    return 0;
}
