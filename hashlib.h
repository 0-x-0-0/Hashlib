#ifndef HASHLIB_H
#define HASHLIB_H

#include "xxhash.h"
#include <stdint.h>
#include <stddef.h>

//constants
#define HASH_DJB2_SEED               5381UL

#define HASH_FNV1_OFFSET_32          2166136261U
#define HASH_FNV1_PRIME_32           16777619U
#define HASH_FNV1_OFFSET_64          14695981039346656037ULL
#define HASH_FNV1_PRIME_64           1099511628211ULL

#define HASH_FNV1A_OFFSET_32         2166136261U
#define HASH_FNV1A_PRIME_32          16777619U
#define HASH_FNV1A_OFFSET_64         14695981039346656037ULL
#define HASH_FNV1A_PRIME_64          1099511628211ULL

#define HASH_MURMUR3_C1_X86_32       0xcc9e2d51U
#define HASH_MURMUR3_C2_X86_32       0x1b873593U
#define HASH_MURMUR3_MIX_X86_32      0xe6546b64U
#define HASH_MURMUR3_FINAL1_X86_32   0x85ebca6bU
#define HASH_MURMUR3_FINAL2_X86_32   0xc2b2ae35U

#define HASH_MURMUR3_C1_X86_128      0x239b961bU
#define HASH_MURMUR3_C2_X86_128      0xab0e9789U
#define HASH_MURMUR3_C3_X86_128      0x38b34ae5U
#define HASH_MURMUR3_C4_X86_128      0xa1e38b93U
#define HASH_MURMUR3_MIX_X86_128     0x85ebca6bU
#define HASH_MURMUR3_FINAL_X86_128   0xc2b2ae35U

#define HASH_MURMUR3_C1_X64_128      0x87c37b91114253d5ULL
#define HASH_MURMUR3_C2_X64_128      0x4cf5ad432745937fULL
#define HASH_MURMUR3_MIX_1_X64_128   0x52dce729ULL
#define HASH_MURMUR3_MIX_2_X64_128   0x38495ab5ULL
#define HASH_MURMUR3_FINAL1_X64_128  0xff51afd7ed558ccdULL
#define HASH_MURMUR3_FINAL2_X64_128  0xc4ceb9fe1a85ec53ULL

#define HASH_MURMUR3_SEED            0x12345678U

#define HASH_CRC32_INIT              0xFFFFFFFFU

#ifdef __cplusplus
extern "C" {
#endif

//functions
uint64_t hash_djb2(const unsigned char *str);
uint64_t hash_djb2_xor(const unsigned char *str);

uint64_t hash_sdbm(const unsigned char *str);

uint32_t hash_fnv1_32(const unsigned char *str);
uint64_t hash_fnv1_64(const unsigned char *str);

uint32_t hash_fnv1a_32(const unsigned char *str);
uint64_t hash_fnv1a_64(const unsigned char *str);

void hash_murmur3_x86_32(const void *key, const int len, const uint32_t seed, void *out);
void hash_murmur3_x86_128(const void *key, const int len, const uint32_t seed, void *out);
void hash_murmur3_x64_128(const void *key, const int len, const uint32_t seed, void *out);

uint32_t hash_xxh32(const unsigned char *str, uint32_t seed);
uint32_t hash_xxh32_buf(const void *buffer, size_t len, uint32_t seed);
uint64_t hash_xxh64(const unsigned char *str, const uint64_t seed);
uint64_t hash_xxh64_buf(const void *buffer, size_t len, uint64_t seed);
uint64_t hash_xxh3_64(const unsigned char *str, uint64_t seed);
uint64_t hash_xxh3_64_buf(const void *str, size_t len, uint64_t seed);
XXH128_hash_t hash_xxh3_128(const unsigned char *str, uint64_t seed);
XXH128_hash_t hash_xxh3_128_buf(const void *str, size_t len, uint64_t seed);

uint32_t hash_crc32(const unsigned char *str);
uint32_t hash_crc32_buf(const void *buffer, size_t len);


#ifdef __cplusplus
}
#endif


static inline uint64_t rotl64(uint64_t x, int8_t r)
{
    return (x << r) | (x >> (64 - r));
}

static inline uint32_t rotl32(uint32_t x, int8_t r)
{
    return (x << r) | (x >> (32 - r));
}

static inline uint32_t fmix32(uint32_t h)
{
    h ^= h >> 16;
    h *= HASH_MURMUR3_FINAL1_X86_32;
    h ^= h >> 13;
    h *= HASH_MURMUR3_FINAL2_X86_32;
    h ^= h >> 16;

    return h;
}

static inline uint64_t fmix64(uint64_t k)
{
    k ^= k >> 33;
    k *= HASH_MURMUR3_FINAL1_X64_128;
    k ^= k >> 33;
    k *= HASH_MURMUR3_FINAL2_X64_128;
    k ^= k >> 33;
    
    return k;
}

#endif