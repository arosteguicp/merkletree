#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

constexpr uint32_t ROTRIGHT(uint32_t x, uint32_t n) {
    return ((x >> n) | (x << (32 - n)));
}

constexpr uint32_t CH(uint32_t x, uint32_t y, uint32_t z) {
    return ((x & y) ^ ((~x) & z));
}

constexpr uint32_t MAJ(uint32_t x, uint32_t y, uint32_t z) {
    return ((x & y) ^ (x & z) ^ (y & z));
}

constexpr uint32_t EP0(uint32_t x) {
    return (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22));
}

constexpr uint32_t EP1(uint32_t x) {
    return (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25));
}

constexpr uint32_t SIG0(uint32_t x) {
    return (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ (x >> 3));
}

constexpr uint32_t SIG1(uint32_t x) {
    return (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ (x >> 10));
}

class SHA256 {
public:
    SHA256() {
        initialize();
    }

    std::string hash(const std::string& input) {
        initialize();
        process(input);
        finalize();

        std::stringstream ss;
        for (int i = 0; i < 8; ++i) {
            ss << std::hex << std::setw(8) << std::setfill('0') << h[i];
        }
        return ss.str();
    }

private:
    void initialize() {
        h[0] = 0x6a09e667;
        h[1] = 0xbb67ae85;
        h[2] = 0x3c6ef372;
        h[3] = 0xa54ff53a;
        h[4] = 0x510e527f;
        h[5] = 0x9b05688c;
        h[6] = 0x1f83d9ab;
        h[7] = 0x5be0cd19;
        len = 0;
        total_len = 0;
    }

    void process(const std::string& input) {
        for (char c : input) {
            buffer[len++] = c;
            if (len == 64) {
                transform();
                total_len += 512;
                len = 0;
            }
        }
    }

    void finalize() {
        uint64_t bits = total_len + len * 8;
        buffer[len++] = 0x80;
        while (len < 56) {
            buffer[len++] = 0x00;
        }
        for (int i = 7; i >= 0; --i) {
            buffer[len++] = (bits >> (8 * i)) & 0xFF;
        }

        transform();

        for (int i = 0; i < 8; ++i) {
            h[i] = swap_uint32(h[i]);
        }
    }

    void transform() {
        uint32_t w[64];
        uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2;

        for (int i = 0; i < 16; ++i) {
            w[i] = (buffer[i * 4] << 24) | (buffer[i * 4 + 1] << 16) | (buffer[i * 4 + 2] << 8) | buffer[i * 4 + 3];
        }
        for (int i = 16; i < 64; ++i) {
            w[i] = SIG1(w[i - 2]) + w[i - 7] + SIG0(w[i - 15]) + w[i - 16];
        }

        a = h[0];
        b = h[1];
        c = h[2];
        d = h[3];
        e = h[4];
        f = h[5];
        g = h[6];
        h = h[7];

        for (int i = 0; i < 64; ++i) {
            t1 = h + EP1(e) + CH(e, f, g) + k[i] + w[i];
            t2 = EP0(a) + MAJ(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += h;
    }

    uint32_t swap_uint32(uint32_t num) {
        return ((num >> 24) & 0xff) | ((num << 8) & 0xff0000) | ((num >> 8) & 0xff00) | (num << 24);
    }

private:
    uint32_t h[8];
    uint8_t buffer[64];
    uint64_t len;
    uint64_t total_len;
};

int main() {
    std::string data = "Hello, World!";
    SHA256 sha256;
    std::string hashed = sha256.hash(data);

    std::cout << "Original Data: " << data << std::endl;
    std::cout << "SHA-256 Hash: " << hashed << std::endl;

    return 0;
}
