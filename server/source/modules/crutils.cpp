#include "crutils.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <algorithm>
#include <string>

namespace crutils {

    std::string generateSalt()
    {
        const size_t SALT_SIZE = 8;
        CryptoPP::byte salt[SALT_SIZE];
        CryptoPP::AutoSeededRandomPool prng;
        prng.GenerateBlock(salt, SALT_SIZE);
        std::string salt_hex;
        CryptoPP::ArraySource(salt, sizeof(salt), true,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(salt_hex), true));

        while (salt_hex.size() < 16)
        {
            salt_hex = "0" + salt_hex;
        }

        return salt_hex;
    }

    std::string calculateHash(const std::string &data)
    {
        CryptoPP::SHA1 sha1;
        std::string hash;
        CryptoPP::StringSource(data, true,
            new CryptoPP::HashFilter(sha1,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(hash), false)));

        std::transform(hash.begin(), hash.end(), hash.begin(), ::toupper);

        return hash;
    }

}
