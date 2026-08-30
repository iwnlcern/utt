module checker::xxh3;

import std::core::bitorder;
import std::bits;

const ulong PRIME32_1 = 0x9e3779b1;
const ulong PRIME32_2 = 0x85ebca77;
const ulong PRIME32_3 = 0xc2b2ae3d;
const ulong PRIME64_1 = 0x9e3779b185ebca87;
const ulong PRIME64_2 = 0xc2b2ae3d27d4eb4f;
const ulong PRIME64_3 = 0x165667b19e3779f9;
const ulong PRIME64_4 = 0x85ebca77c2b2ae63;
const ulong PRIME64_5 = 0x27d4eb2f165667c5;
const ulong PRIME_MX1 = 0x165667919e3779f9;
const ulong PRIME_MX2 = 0x9fb21c651e98df25;

const char[192] SECRET = {
    0xb8, 0xfe, 0x6c, 0x39, 0x23, 0xa4, 0x4b, 0xbe, 0x7c, 0x01, 0x81, 0x2c, 0xf7, 0x21, 0xad, 0x1c,
    0xde, 0xd4, 0x6d, 0xe9, 0x83, 0x90, 0x97, 0xdb, 0x72, 0x40, 0xa4, 0xa4, 0xb7, 0xb3, 0x67, 0x1f,
    0xcb, 0x79, 0xe6, 0x4e, 0xcc, 0xc0, 0xe5, 0x78, 0x82, 0x5a, 0xd0, 0x7d, 0xcc, 0xff, 0x72, 0x21,
    0xb8, 0x08, 0x46, 0x74, 0xf7, 0x43, 0x24, 0x8e, 0xe0, 0x35, 0x90, 0xe6, 0x81, 0x3a, 0x26, 0x4c,
    0x3c, 0x28, 0x52, 0xbb, 0x91, 0xc3, 0x00, 0xcb, 0x88, 0xd0, 0x65, 0x8b, 0x1b, 0x53, 0x2e, 0xa3,
    0x71, 0x64, 0x48, 0x97, 0xa2, 0x0d, 0xf9, 0x4e, 0x38, 0x19, 0xef, 0x46, 0xa9, 0xde, 0xac, 0xd8,
    0xa8, 0xfa, 0x76, 0x3f, 0xe3, 0x9c, 0x34, 0x3f, 0xf9, 0xdc, 0xbb, 0xc7, 0xc7, 0x0b, 0x4f, 0x1d,
    0x8a, 0x51, 0xe0, 0x4b, 0xcd, 0xb4, 0x59, 0x31, 0xc8, 0x9f, 0x7e, 0xc9, 0xd9, 0x78, 0x73, 0x64,
    0xea, 0xc5, 0xac, 0x83, 0x34, 0xd3, 0xeb, 0xc3, 0xc5, 0x81, 0xa0, 0xff, 0xfa, 0x13, 0x63, 0xeb,
    0x17, 0x0d, 0xdd, 0x51, 0xb7, 0xf0, 0xda, 0x49, 0xd3, 0x16, 0x55, 0x26, 0x29, 0xd4, 0x68, 0x9e,
    0x2b, 0x16, 0xbe, 0x58, 0x7d, 0x47, 0xa1, 0xfc, 0x8f, 0xf8, 0xb8, 0xd1, 0x7a, 0xd0, 0x31, 0xce,
    0x45, 0xcb, 0x3a, 0x8f, 0x95, 0x16, 0x04, 0x28, 0xaf, 0xd7, 0xfb, 0xca, 0xbb, 0x4b, 0x40, 0x7e,
};

struct Hash128
{
    ulong low;
    ulong high;
}

fn ulong read64(char[] bytes, sz offset) @inline
{
    return bitorder::read(bytes[offset:8], ULongLE);
}

fn uint read32(char[] bytes, sz offset) @inline
{
    return bitorder::read(bytes[offset:4], UIntLE);
}

fn Hash128 multiply128(ulong left, ulong right) @inline
{
    uint128 product = (uint128)left * (uint128)right;
    return { (ulong)product, (ulong)(product >> 64) };
}

fn ulong fold128(ulong left, ulong right) @inline
{
    Hash128 product = multiply128(left, right);
    return product.low ^ product.high;
}

fn ulong avalanche(ulong value) @inline
{
    value ^= value >> 37;
    value *= PRIME_MX1;
    value ^= value >> 32;
    return value;
}

fn ulong avalanche_xxh64(ulong value) @inline
{
    value ^= value >> 33;
    value *= PRIME64_2;
    value ^= value >> 29;
    value *= PRIME64_3;
    value ^= value >> 32;
    return value;
}

fn Hash128 hash_0to16(char[] data)
{
    sz len = data.len;
    if (len == 0)
    {
        return {
            avalanche_xxh64(read64(SECRET[..], 64) ^ read64(SECRET[..], 72)),
            avalanche_xxh64(read64(SECRET[..], 80) ^ read64(SECRET[..], 88)),
        };
    }
    if (len <= 3)
    {
        uint combined = (uint)data[len - 1]
            | ((uint)len << 8)
            | ((uint)data[0] << 16)
            | ((uint)data[len >> 1] << 24);
        ulong low = (ulong)(read32(SECRET[..], 0) ^ read32(SECRET[..], 4)) ^ (ulong)combined;
        uint swapped = bswap(combined).rotl(13);
        ulong high = (ulong)(read32(SECRET[..], 8) ^ read32(SECRET[..], 12)) ^ (ulong)swapped;
        return { avalanche_xxh64(low), avalanche_xxh64(high) };
    }
    if (len <= 8)
    {
        uint first = read32(data, 0);
        uint last = read32(data, len - 4);
        ulong combined = (ulong)first | ((ulong)last << 32);
        ulong value = (read64(SECRET[..], 16) ^ read64(SECRET[..], 24)) ^ combined;
        Hash128 product = multiply128(value, PRIME64_1 + ((ulong)len << 2));
        product.high += product.low << 1;
        product.low ^= product.high >> 3;
        product.low ^= product.low >> 35;
        product.low *= PRIME_MX2;
        product.low ^= product.low >> 28;
        product.high = avalanche(product.high);
        return product;
    }

    ulong first = read64(data, 0);
    ulong last = read64(data, len - 8);
    ulong val1 = (read64(SECRET[..], 32) ^ read64(SECRET[..], 40)) ^ first ^ last;
    ulong val2 = (read64(SECRET[..], 48) ^ read64(SECRET[..], 56)) ^ last;
    Hash128 product = multiply128(val1, PRIME64_1);
    ulong low = product.low + ((ulong)(len - 1) << 54);
    ulong high = product.high + val2 + (ulong)(uint)val2 * (PRIME32_2 - 1);
    low ^= bswap(high);
    product = multiply128(low, PRIME64_2);
    low = product.low;
    high = product.high + high * PRIME64_2;
    return { avalanche(low), avalanche(high) };
}

fn ulong mix_step(char[] data, sz data_offset, sz secret_offset) @inline
{
    ulong left = read64(data, data_offset) ^ read64(SECRET[..], secret_offset);
    ulong right = read64(data, data_offset + 8) ^ read64(SECRET[..], secret_offset + 8);
    return fold128(left, right);
}

fn void mix_two(ulong[2]* acc, char[] data, sz first, sz second, sz secret_offset)
{
    ulong first_sum = read64(data, first) + read64(data, first + 8);
    ulong second_sum = read64(data, second) + read64(data, second + 8);
    (*acc)[0] += mix_step(data, first, secret_offset);
    (*acc)[1] += mix_step(data, second, secret_offset + 16);
    (*acc)[0] ^= second_sum;
    (*acc)[1] ^= first_sum;
}

fn Hash128 hash_17to240(char[] data)
{
    sz len = data.len;
    ulong[2] acc = { (ulong)len * PRIME64_1, 0 };
    if (len <= 128)
    {
        sz rounds = ((len - 1) >> 5) + 1;
        for (sz round = rounds; round > 0; round--)
        {
            sz i = round - 1;
            mix_two(&acc, data, i * 16, len - i * 16 - 16, i * 32);
        }
    }
    else
    {
        for (sz i = 0; i < 4; i++)
        {
            mix_two(&acc, data, i * 32, i * 32 + 16, i * 32);
        }
        acc[0] = avalanche(acc[0]);
        acc[1] = avalanche(acc[1]);
        sz chunks = len >> 5;
        for (sz i = 4; i < chunks; i++)
        {
            mix_two(&acc, data, i * 32, i * 32 + 16, (i - 4) * 32 + 3);
        }
        mix_two(&acc, data, len - 16, len - 32, 103);
    }
    ulong low = avalanche(acc[0] + acc[1]);
    ulong high = 0 - avalanche(acc[0] * PRIME64_1 + acc[1] * PRIME64_4 + (ulong)len * PRIME64_2);
    return { low, high };
}

fn void accumulate(ulong[8]* acc, char[] data, sz data_offset, sz secret_offset)
{
    for (sz i = 0; i < 8; i++)
    {
        ulong lane = read64(data, data_offset + i * 8);
        ulong keyed = lane ^ read64(SECRET[..], secret_offset + i * 8);
        (*acc)[i ^ 1] += lane;
        (*acc)[i] += (ulong)(uint)keyed * (keyed >> 32);
    }
}

fn void scramble(ulong[8]* acc)
{
    for (sz i = 0; i < 8; i++)
    {
        ulong value = (*acc)[i];
        value ^= value >> 47;
        value ^= read64(SECRET[..], 128 + i * 8);
        (*acc)[i] = value * PRIME32_1;
    }
}

fn ulong merge(ulong[8]* acc, ulong initial, sz secret_offset)
{
    ulong result = initial;
    for (sz i = 0; i < 4; i++)
    {
        result += fold128(
            (*acc)[i * 2] ^ read64(SECRET[..], secret_offset + i * 16),
            (*acc)[i * 2 + 1] ^ read64(SECRET[..], secret_offset + i * 16 + 8),
        );
    }
    return avalanche(result);
}

fn Hash128 hash_long(char[] data)
{
    ulong[8] acc = {
        PRIME32_3, PRIME64_1, PRIME64_2, PRIME64_3,
        PRIME64_4, PRIME32_2, PRIME64_5, PRIME32_1,
    };
    sz full_blocks = (data.len - 1) / 1024;
    for (sz block = 0; block < full_blocks; block++)
    {
        sz block_offset = block * 1024;
        for (sz stripe = 0; stripe < 16; stripe++)
        {
            accumulate(&acc, data, block_offset + stripe * 64, stripe * 8);
        }
        scramble(&acc);
    }

    sz last_offset = full_blocks * 1024;
    sz last_len = data.len - last_offset;
    sz full_stripes = (last_len - 1) / 64;
    for (sz stripe = 0; stripe < full_stripes; stripe++)
    {
        accumulate(&acc, data, last_offset + stripe * 64, stripe * 8);
    }
    accumulate(&acc, data, data.len - 64, 121);
    return {
        merge(&acc, (ulong)data.len * PRIME64_1, 11),
        merge(&acc, ~((ulong)data.len * PRIME64_2), 117),
    };
}

fn char[16] hash128(char[] data)
{
    Hash128 hash;
    if (data.len <= 16) { hash = hash_0to16(data); }
    else if (data.len <= 240) { hash = hash_17to240(data); }
    else { hash = hash_long(data); }

    char[16] canonical;
    bitorder::write(hash.high, canonical[0:8], ULongBE);
    bitorder::write(hash.low, canonical[8:8], ULongBE);
    return canonical;
}
