#include <iostream>
#include <iomanip>
#include "cryptopp890/sha3.h"
#include "cryptopp890/hex.h"

int main() {
    // Mensaje que deseas hashear
    std::string message = "Hola, Mundo!";

    // Configura el objeto SHA-3
    CryptoPP::SHA3_256 hash;

    // Calcula el hash SHA-3
    std::string digest;
    CryptoPP::StringSource(message, true,
                           new CryptoPP::HashFilter(hash,
                                                    new CryptoPP::HexEncoder(
                                                            new CryptoPP::StringSink(digest)
                                                    )
                           )
    );

    // Imprime el hash calculado
    std::cout << "Mensaje: " << message << std::endl;
    std::cout << "Hash SHA-3: " << digest << std::endl;

    return 0;
}
