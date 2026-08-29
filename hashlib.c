#define XXH_STATIC_LINKING_ONLY
#define XXH_IMPLEMENTATION

#include "xxhash.h"
#include "hashlib.h"

static const uint32_t crc32_table[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

uint64_t hash_djb2(const unsigned char *str)
{
    unsigned long hash = HASH_DJB2_SEED;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

uint64_t hash_djb2_xor(const unsigned char *str)
{
    unsigned long hash = HASH_DJB2_SEED;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) ^ c;

    return hash;
}

uint64_t hash_sdbm(const unsigned char *str)
{
    unsigned long hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

uint32_t hash_fnv1_32(const unsigned char *str)
{
    uint32_t hash = HASH_FNV1_OFFSET_32;
    uint32_t prime = HASH_FNV1_PRIME_32;
    unsigned char c;

    while ((c = *str++))
    {
        hash = hash * prime;
        hash = hash ^ c;
    }

    return hash;
}

uint64_t hash_fnv1_64(const unsigned char *str)
{
    uint64_t hash = HASH_FNV1_OFFSET_64;
    uint64_t prime = HASH_FNV1_PRIME_64;
    unsigned char c;

    while ((c = *str++))
    {
        hash = hash * prime;
        hash = hash ^ c;
    }

    return hash;
}

uint32_t hash_fnv1a_32(const unsigned char *str)
{
    uint32_t hash = HASH_FNV1A_OFFSET_32;
    uint32_t prime = HASH_FNV1A_PRIME_32;
    unsigned char c;

    while ((c = *str++))
    {
        hash = hash ^ c;
        hash = hash * prime;
    }

    return hash;
}

uint64_t hash_fnv1a_64(const unsigned char *str)
{
    uint64_t hash = HASH_FNV1A_OFFSET_64;
    uint64_t prime = HASH_FNV1A_PRIME_64;
    unsigned char c;

    while ((c = *str++))
    {
        hash = hash ^ c;
        hash = hash * prime;
    }

    return hash;
}

void hash_murmur3_x86_32(const void *key, const int len, const uint32_t seed, void *out)
{
    const uint8_t *data = (const uint8_t*)key;
    int nblocks = len / 4;
    uint32_t h1 = seed;

    const uint32_t c1 = HASH_MURMUR3_C1_X86_32;
    const uint32_t c2 = HASH_MURMUR3_C2_X86_32;

    const uint32_t *blocks = (const uint32_t *)data;
    for(int i = 0; i < nblocks; i++)
    {
        uint32_t k1 = blocks[i];

        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rotl32(h1, 13);
        h1 = h1 * 5 + HASH_MURMUR3_MIX_X86_32;
    }

    const uint8_t *tail = (const uint8_t*)(data + nblocks*4);
    uint32_t k1 = 0;
    switch(len & 3)
    {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
            k1 *= c1;
            k1 = rotl32(k1,15);
            k1 *= c2;
            h1 ^= k1;
    };

    h1 ^= len;
    h1 = fmix32(h1);
    *(uint32_t*)out = h1;
}

void hash_murmur3_x64_128(const void *key, const int len, const uint32_t seed, void *out)
{
    const uint8_t *data = (const uint8_t*)key;
    const int nblocks = len / 16;
    uint64_t h1 = seed;
    uint64_t h2 = seed;

    const uint64_t c1 = HASH_MURMUR3_C1_X64_128;
    const uint64_t c2 = HASH_MURMUR3_C2_X64_128;

    const uint64_t *blocks = (const uint64_t *)(data);
    for (int i = 0; i < nblocks; i++)
    {
        uint64_t k1 = blocks[i * 2];
        uint64_t k2 = blocks[i * 2 + 1];

        k1 *= c1;
        k1 = rotl64(k1, 31);
        k1 *= c2;
        h1 ^= k1;

        h1 = rotl64(h1, 27);
        h1 += h2;
        h1 = h1 * 5 + HASH_MURMUR3_MIX_1_X64_128;

        k2 *= c2;
        k2 = rotl64(k2, 33);
        k2 *= c1;
        h2 ^= k2;

        h2 = rotl64(h2, 31);
        h2 += h1;
        h2 = h2 * 5 + HASH_MURMUR3_MIX_2_X64_128;
    }

    const uint8_t *tail = (const uint8_t*)(data + nblocks * 16);
    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch (len & 15)
    {
        case 15: k2 ^= (uint64_t)tail[14] << 48;
        case 14: k2 ^= (uint64_t)tail[13] << 40;
        case 13: k2 ^= (uint64_t)tail[12] << 32;
        case 12: k2 ^= (uint64_t)tail[11] << 24;
        case 11: k2 ^= (uint64_t)tail[10] << 16;
        case 10: k2 ^= (uint64_t)tail[9] << 8;
        case 9:  k2 ^= (uint64_t)tail[8] << 0;
            k2 *= c2;
            k2 = rotl64(k2, 33);
            k2 *= c1;
            h2 ^= k2;
        case 8:  k1 ^= (uint64_t)tail[7] << 56;
        case 7:  k1 ^= (uint64_t)tail[6] << 48;
        case 6:  k1 ^= (uint64_t)tail[5] << 40;
        case 5:  k1 ^= (uint64_t)tail[4] << 32;
        case 4:  k1 ^= (uint64_t)tail[3] << 24;
        case 3:  k1 ^= (uint64_t)tail[2] << 16;
        case 2:  k1 ^= (uint64_t)tail[1] << 8;
        case 1:  k1 ^= (uint64_t)tail[0] << 0;
            k1 *= c1;
            k1 = rotl64(k1, 31);
            k1 *= c2;
            h1 ^= k1;
    };

    h1 ^= len;
    h2 ^= len;
    h1 += h2;
    h2 += h1;
    h1 = fmix64(h1);
    h2 = fmix64(h2);
    h1 += h2;
    h2 += h1;

    ((uint64_t*)out)[0] = h1;
    ((uint64_t*)out)[1] = h2;
}

void hash_murmur3_x86_128(const void *key, const int len, const uint32_t seed, void *out)
{
    const uint8_t *data = (const uint8_t*)key;
    const int nblocks = len / 16;
    uint32_t h1 = seed;
    uint32_t h2 = seed;
    uint32_t h3 = seed;
    uint32_t h4 = seed;

    const uint32_t c1 = HASH_MURMUR3_C1_X86_128;
    const uint32_t c2 = HASH_MURMUR3_C2_X86_128;
    const uint32_t c3 = HASH_MURMUR3_C3_X86_128;
    const uint32_t c4 = HASH_MURMUR3_C4_X86_128;

    const uint32_t *blocks = (const uint32_t *)(data);
    for (int i = 0; i < nblocks; i++) {
        uint32_t k1 = blocks[i * 4 + 0];
        uint32_t k2 = blocks[i * 4 + 1];
        uint32_t k3 = blocks[i * 4 + 2];
        uint32_t k4 = blocks[i * 4 + 3];

        k1 *= c1;
        k1 = rotl32(k1, 15);
        k1 *= c2;
        h1 ^= k1;

        h1 = rotl32(h1, 19);
        h1 += h2;
        h1 = h1 * 5 + 0x561ccd1b;

        k2 *= c2;
        k2 = rotl32(k2, 16);
        k2 *= c3;
        h2 ^= k2;

        h2 = rotl32(h2, 17);
        h2 += h3;
        h2 = h2 * 5 + 0x0bcaa747;

        k3 *= c3;
        k3 = rotl32(k3, 17);
        k3 *= c4;
        h3 ^= k3;

        h3 = rotl32(h3, 15);
        h3 += h4;
        h3 = h3 * 5 + 0x96cd1c35;

        k4 *= c4;
        k4 = rotl32(k4, 18);
        k4 *= c1;
        h4 ^= k4;

        h4 = rotl32(h4, 13);
        h4 += h1;
        h4 = h4 * 5 + 0x32ac3b17;
    }

    const uint8_t *tail = (const uint8_t*)(data + nblocks * 16);
    uint32_t k1 = 0;
    uint32_t k2 = 0;
    uint32_t k3 = 0;
    uint32_t k4 = 0;

    switch (len & 15) {
        case 15: k4 ^= (uint32_t)tail[14] << 16;
        case 14: k4 ^= (uint32_t)tail[13] << 8;
        case 13: k4 ^= (uint32_t)tail[12] << 0;
            k4 *= c4;
            k4 = rotl32(k4, 18);
            k4 *= c1;
            h4 ^= k4;
        case 12: k3 ^= (uint32_t)tail[11] << 24;
        case 11: k3 ^= (uint32_t)tail[10] << 16;
        case 10: k3 ^= (uint32_t)tail[9] << 8;
        case 9:  k3 ^= (uint32_t)tail[8] << 0;
            k3 *= c3;
            k3 = rotl32(k3, 17);
            k3 *= c4;
            h3 ^= k3;
        case 8:  k2 ^= (uint32_t)tail[7] << 24;
        case 7:  k2 ^= (uint32_t)tail[6] << 16;
        case 6:  k2 ^= (uint32_t)tail[5] << 8;
        case 5:  k2 ^= (uint32_t)tail[4] << 0;
            k2 *= c2;
            k2 = rotl32(k2, 16);
            k2 *= c3;
            h2 ^= k2;
        case 4:  k1 ^= (uint32_t)tail[3] << 24;
        case 3:  k1 ^= (uint32_t)tail[2] << 16;
        case 2:  k1 ^= (uint32_t)tail[1] << 8;
        case 1:  k1 ^= (uint32_t)tail[0] << 0;
            k1 *= c1;
            k1 = rotl32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
    };

    h1 ^= len;
    h2 ^= len;
    h3 ^= len;
    h4 ^= len;

    h1 += h2;
    h1 += h3;
    h1 += h4;
    h2 += h1;
    h3 += h1;
    h4 += h1;

    h1 = fmix32(h1);
    h2 = fmix32(h2);
    h3 = fmix32(h3);
    h4 = fmix32(h4);

    h1 += h2;
    h1 += h3;
    h1 += h4;
    h2 += h1;
    h3 += h1;
    h4 += h1;

    ((uint32_t*)out)[0] = h1;
    ((uint32_t*)out)[1] = h2;
    ((uint32_t*)out)[2] = h3;
    ((uint32_t*)out)[3] = h4;
}

uint32_t hash_xxh32(const unsigned char *str, uint32_t seed)
{
    return XXH32(str, strlen((const char*)str), seed);
}

uint32_t hash_xxh32_buf(const void *buffer, size_t len, uint32_t seed)
{
    return XXH32(buffer, len, seed);
}

uint64_t hash_xxh64(const unsigned char *str, const uint64_t seed)
{
    return XXH64(str, strlen((const char*)str), seed);
}

uint64_t hash_xxh64_buf(const void *buffer, size_t len, uint64_t seed)
{
    return XXH64(buffer, len, seed);
}

uint64_t hash_xxh3_64(const unsigned char *str, uint64_t seed)
{
    return XXH3_64bits_withSeed(str, strlen((const char*)str), seed);
}

uint64_t hash_xxh3_64_buf(const void *str, size_t len, uint64_t seed)
{
    return XXH3_64bits_withSeed(str, len, seed);
}

XXH128_hash_t hash_xxh3_128(const unsigned char *str, uint64_t seed)
{
    return XXH3_128bits_withSeed(str, strlen((const char*)str), seed);
}

XXH128_hash_t hash_xxh3_128_buf(const void *str, size_t len, uint64_t seed)
{
    return XXH3_128bits_withSeed(str, len, seed);
}

#ifdef __GNUC__
    #ifdef __x86_64__
        #include <cpuid.h>
        #include <x86intrin.h>
        
        static int has_sse42(void) {
            unsigned int eax, ebx, ecx, edx;
            if (__get_cpuid(1, &eax, &ebx, &ecx, &edx))
            {
                return (ecx & (1 << 20)) != 0;
            }
            return 0;
        }
        
        __attribute__((target("sse4.2")))
        static uint32_t hash_crc32_hw(const unsigned char *str)
        {
            uint32_t crc = HASH_CRC32_INIT;
            while (*str) {
                crc = _mm_crc32_u8(crc, *str++);
            }
            return ~crc;
        }
        
        __attribute__((target("sse4.2")))
        static uint32_t hash_crc32_buf_hw(const void *buffer, size_t len)
        {
            uint32_t crc = HASH_CRC32_INIT;
            const unsigned char *data = (const unsigned char*)buffer;
            for (size_t i = 0; i < len; i++)
            {
                crc = _mm_crc32_u8(crc, data[i]);
            }
            return ~crc;
        }
    #endif
#endif

static uint32_t hash_crc32_soft(const unsigned char *str) {
    uint32_t crc = HASH_CRC32_INIT;
    while (*str)
    {
        crc = crc32_table[(crc ^ *str++) & 0xFF] ^ (crc >> 8);
    }
    return ~crc;
}

static uint32_t hash_crc32_buf_soft(const void *buffer, size_t len) {
    uint32_t crc = HASH_CRC32_INIT;
    const unsigned char *data = (const unsigned char*)buffer;
    for (size_t i = 0; i < len; i++)
    {
        crc = crc32_table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    return ~crc;
}

uint32_t hash_crc32(const unsigned char *str)
{
#ifdef __x86_64__
    static int use_hw = -1;
    if (use_hw == -1) use_hw = has_sse42();
    
    if (use_hw)
    {
        return hash_crc32_hw(str);
    }
#endif
    return hash_crc32_soft(str);
}

uint32_t hash_crc32_buf(const void *buffer, size_t len)
{
#ifdef __x86_64__
    static int use_hw = -1;
    if (use_hw == -1) use_hw = has_sse42();
    
    if (use_hw)
    {
        return hash_crc32_buf_hw(buffer, len);
    }
#endif
    return hash_crc32_buf_soft(buffer, len);
}