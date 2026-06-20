#pragma once
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <random>
#include <ratio>
#include <iomanip>
#include <sstream>
#include <string>

#include <iostream>

#ifdef _WIN32
  #include <windows.h>
  #include <sddl.h>
  #pragma comment(lib, "Advapi32.lib")
#else
  #include <unistd.h>
  #include <sys/types.h>
#endif

/// dsl::random::randint(a, b) - generate a random integer in the range a ~ b
/// dsl::random::randfloat(a, b) - generate a random float number in the range a ~ b
/// dsl::random::vec_choise(list) - select a random element from a std::vector
/// dsl::random::arr_choise(arr, size) - select a random element from a C-style array of a given size
/// dsl::random::randbool() - select random true or false
/// dsl::random::randrgba() - return random R,G,B,A
/// dsl::random::randrgb() - return random R,G,B
/// dsl::random::randhsl() - return random H,S,L
/// dsl::random::randhsv() - return random H,S,V
/// dsl::uuid::v1() - generate uuid v1
/// dsl::uuid::v2() - generate uuid v2 
/// dsl::uuid::v3(name_string) - generate uuid v3
/// dsl::uuid::v4() - generate uuid v4
///            v5 - not implemented
/// dsl::uuid::v6() - generate uuid v6
/// dsl::uuid::v7() - generate uuid v7
/// dsl::uuid::v8() - generate uuid v8
/// dsl::dmath::pow(a, y) - return a^y (int, double, float)
/// dsl::dmath::abs(x) - return absolute value x (int, double, float)
/// dsl::dmath::trunc(x) - return whole part x (int, double, float)
/// dsl::dmath::lerp(a, b, t) - return linear Interpolation (int, double, float)
/// dsl::dmath::clamp(x, min, max) - limits a x to a range min/max (int, double, float)
/// dsl::dmath::sqrt(x) - return square root x (int, float, double)
/// dsl::dmath::gcd(a, b) - return greatest common divisor
/// dsl::dmath::lcm(a, b) - return least common multiple
/// dsl::hash::gencrc32(table[256]) - generate crc32 hash
/// dsl::hash::updcrc32(table[256], initial, buf, len) - Calculates the CRC32 from the memory block of length, using the passed table and the initial value . Returns a 32-bit hash.
/// dsl::hash::genSha256(data, len, hash) - generate sha-256 hash
/// dsl::hash::genFnv1a32(datam len, hash) - generate 32-bit Fvn1a hash
/// dsl::hash::genFnv1a64(datam len, hash) - generate 64-bit Fvn1a hash
/// dsl::crypto::ChaCha20(data, len, key, nonce, counter, out) - encrypts data using the method ChaCha20
/// dsl::encode::encbase64(data, len, out) - encode base64
/// dsl::encode::decbase64(data, len, out) - decode base64

namespace dsl {

namespace random {

inline int randint(int a, int b) {
  static std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<int> distribution(a, b);
  return distribution(generator);
}

inline float randfloat(float a, float b) {
  static std::mt19937 generator(std::random_device{}());
  std::uniform_real_distribution<float> distribution(a, b);
  return distribution(generator);
}

inline int vec_choise(const std::vector<int>& list) {
  if (list.empty()) return 0;
  int size = list.size();
  int number = randint(0, size - 1);
  return list[number];
}

inline int arr_choise(const int* arr, int size) {
  if (size <= 0) return 0;
  int number = randint(0, size - 1);
  return arr[number];
}

inline int randbool() {
  return randint(0, 1) == 1; 
}

struct rgba { int r, g, b, a; };
inline rgba randrgba() {
  int r = randint(0, 255);
  int g = randint(0, 255);
  int b = randint(0, 255);
  int a = randint(0, 255);
  return {r, g, b, a};
}

struct rgb { int r, g, b; };
inline rgb randrgb() {
  int r = randint(0, 255);
  int g = randint(0, 255);
  int b = randint(0, 255);
  return { r, g, b };
}

struct hsl { int h, s, l; };
inline hsl randhsl() {
  int h = randint(0, 360);
  int s = randint(0, 100);
  int l = randint(0, 100);
  return { h, s, l };
}

struct hsv { int h, s, v; };
inline hsv randhsv() {
  int h = randint(0, 360);
  int s = randint(0, 100);
  int v = randint(0, 100);
  return { h, s, v };
}

} // random

namespace uuid {

inline std::string v1() {
  using namespace std::chrono;
  using namespace std::chrono_literals;
  auto n = system_clock::now();
  auto e = sys_days{1582y/October/15d};
  auto d = n - e;
  uint64_t ticks = duration_cast<std::chrono::duration<uint64_t, std::ratio<1, 10'000'000>>>(d).count();
  uint16_t cs = std::random_device{}() & 0x3FFF;
  std::stringstream ss;
  ss << std::hex << std::setfill('0')
    << std::setw(8) << (ticks & 0xFFFFFFFF) << "-"
    << std::setw(4) << ((ticks >> 32) & 0xFFFF) << "-"
    << std::setw(4) << (((ticks >> 48) & 0x0FFF) | 0x1000) << "-"
    << std::setw(4) << (cs | 0x8000) << "-"
    << "001a2b3c4d5e";
  return ss.str();
}

inline std::string v2() {
  using namespace std::chrono;
  using namespace std::chrono_literals;
  auto n = system_clock::now();
  auto e = sys_days{1582y/October/15d};
  auto d = n - e;
  uint64_t ticks = duration_cast<std::chrono::duration<uint64_t, std::ratio<1, 10'000'000>>>(d).count();
  uint16_t cs = std::random_device{}() & 0x3FFF;
  int uid = 0;
#ifdef _WIN32
  uid = static_cast<int>(GetCurrentProcessId());
#else
  uid = static_cast<int>(getuid());
#endif
  std::stringstream ss;
  ss << std::hex << std::setfill('0')
    << std::setw(8) << (uid & 0xFFFFFFFF) << '-'
    << std::setw(4) << ((ticks >> 32) & 0xFFFF) <<  '-'
    << std::setw(4) << (((ticks >> 48) & 0x0FFF) | 0x2000) << '-'
    << std::setw(2) << (int)(((cs >> 8) & 0x3F) | 0x80)
    << std::setw(2) << (int)(cs & 0xFF) << '-'
    << "002ab6f1a3c5";
  return ss.str();
}

inline std::string v3(const char* name_string) {
  const char* ns = "6ba7b810-9dad-11d1-80b4-00c04fd430c8"; /* DNS */
  uint8_t hash[16]; 
  std::string combined = std::string(ns) + std::string(name_string);
  const uint8_t* data = (const uint8_t*)combined.c_str();
  size_t len = combined.length();
  
  static const uint32_t S[64] = {
    7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
    5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,  
    4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
    6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
  };
  static const uint32_t K[64] = { /* floor(2³² × |sin(i+1)|) */
    0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
    0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
    0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
    0x6b901122,0xfd987193,0xa679438e,0x49b40821,
    0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
    0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
    0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
    0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
    0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
    0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
    0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
    0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
    0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
    0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
    0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
    0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
  };
  
  uint32_t h[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476}; /* A, B, C, D */
  uint64_t bits = (uint64_t)len * 8;
  uint8_t buf[64];
  uint32_t M[16];
  size_t pos = 0;
  
  while (pos + 64 <= len) {
    for (int i = 0; i < 16; i++)
      M[i] = (uint32_t)data[pos+i*4] | ((uint32_t)data[pos+i*4+1]<<8) | ((uint32_t)data[pos+i*4+2]<<16) | ((uint32_t)data[pos+i*4+3]<<24);
    uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
    for (int i = 0; i < 64; i++) {
      uint32_t f, g;
      if (i < 16) { f = (b & c) | (~b & d); g = i; }
      else if (i < 32) { f = (d & b) | (~d & c); g = (5*i + 1) % 16; }
      else if (i < 48) { f = b ^ c ^ d; g = (3*i + 5) % 16; }
      else { f = c ^ (b | ~d); g = (7*i) % 16; }
      uint32_t t = d; d = c; c = b;
      b += ((a + f + K[i] + M[g]) << S[i]) | ((a + f + K[i] + M[g]) >> (32 - S[i]));
      a = t;
    }
    h[0] += a; h[1] += b; h[2] += c; h[3] += d;
    pos += 64;
  }
  
  size_t rem = len - pos;
  memset(buf, 0, 64);
  memcpy(buf, data + pos, rem);
  buf[rem] = 0x80;
  
  if (rem >= 56) {
    for (int i = 0; i < 16; i++)
      M[i] = (uint32_t)buf[i*4] | ((uint32_t)buf[i*4+1]<<8) | ((uint32_t)buf[i*4+2]<<16) | ((uint32_t)buf[i*4+3]<<24);
    uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
    for (int i = 0; i < 64; i++) {
      uint32_t f, g;
      if (i < 16) { f = (b & c) | (~b & d); g = i; }
      else if (i < 32) { f = (d & b) | (~d & c); g = (5*i + 1) % 16; }
      else if (i < 48) { f = b ^ c ^ d; g = (3*i + 5) % 16; }
      else { f = c ^ (b | ~d); g = (7*i) % 16; }
      uint32_t t = d; d = c; c = b;
      b += ((a + f + K[i] + M[g]) << S[i]) | ((a + f + K[i] + M[g]) >> (32 - S[i]));
      a = t;
    }
    h[0] += a; h[1] += b; h[2] += c; h[3] += d;
    memset(buf, 0, 64);
  }
  
  memcpy(buf + 56, &bits, 8);
  for (int i = 0; i < 16; i++)
    M[i] = (uint32_t)buf[i*4] | ((uint32_t)buf[i*4+1]<<8) | ((uint32_t)buf[i*4+2]<<16) | ((uint32_t)buf[i*4+3]<<24);
  
  uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
  for (int i = 0; i < 64; i++) {
    uint32_t f, g;
    if (i < 16) { f = (b & c) | (~b & d); g = i; }
    else if (i < 32) { f = (d & b) | (~d & c); g = (5*i + 1) % 16; }
    else if (i < 48) { f = b ^ c ^ d; g = (3*i + 5) % 16; }
    else { f = c ^ (b | ~d); g = (7*i) % 16; }
    uint32_t t = d; d = c; c = b;
    b += ((a + f + K[i] + M[g]) << S[i]) | ((a + f + K[i] + M[g]) >> (32 - S[i]));
    a = t;
  }
  h[0] += a; h[1] += b; h[2] += c; h[3] += d;
  
  for (int i = 0; i < 16; i++)
    hash[i] = (uint8_t)(h[i/4] >> ((i%4) * 8));
  hash[6] = (hash[6] & 0x0F) | 0x30;
  hash[8] = (hash[8] & 0x3F) | 0x80;
  
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (int i = 0; i < 16; i++) {
    if (i == 4 || i == 6 || i == 8 || i == 10)
      ss << '-';
    ss << std::setw(2) << (int)hash[i];
  }
  return ss.str();
}

inline std::string v4() {
  static uint64_t s[2] = {
    (uint64_t)std::random_device{}() << 32 | std::random_device{}(),
    (uint64_t)std::random_device{}() << 32 | std::random_device{}()
  };
  auto xorshift128plus = [&]() -> uint64_t {
    uint64_t s1 = s[0];
    uint64_t s0 = s[1];
    s[0] = s0;
    s1 ^= s1 << 23;
    s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return s[1] + s0;
  };
  union { uint8_t b[16]; uint64_t w[2]; } u;
  u.w[0] = xorshift128plus();
  u.w[1] = xorshift128plus();
  u.b[6] = (u.b[6] & 0x0F) | 0x40;
  u.b[8] = (u.b[8] & 0x3F) | 0x80;
  static const char h[] = "0123456789abcdef";
  std::string out(36, '\0');
  int idx = 0;
  for (int i = 0; i < 16; i++) {
    out[idx++] = h[u.b[i] >> 4];
    out[idx++] = h[u.b[i] & 0xF];
    if (i == 3 || i == 5 || i == 7 || i == 9) out[idx++] = '-';
  }
  return out;
}

inline std::string v6() {
  using namespace std::chrono;
  using namespace std::chrono_literals;
  auto n = system_clock::now();
  auto e = sys_days{1582y/October/15d};
  auto d = n - e;
  uint64_t ticks = duration_cast<std::chrono::duration<uint64_t, std::ratio<1, 10'000'000>>>(d).count();
  uint16_t cs = std::random_device{}() & 0x3FFF;
  
  std::stringstream ss;
  ss << std::hex << std::setfill('0')
    << std::setw(8) << (ticks >> 28) << "-"
    << std::setw(4) << ((ticks >> 12) & 0xFFFF) << "-"
    << std::setw(4) << ((ticks & 0x0FFF) | 0x6000) << "-"
    << std::setw(4) << (cs | 0x8000) << "-"
    << "001a2b3c4d5e";
  return ss.str();
}

inline std::string v7() {
  using namespace std::chrono;
  auto now = system_clock::now();
  auto epoch = now.time_since_epoch();
  uint64_t time_ms = duration_cast<milliseconds>(epoch).count();
  
  uint32_t time_high = (time_ms >> 16) & 0xFFFFFFFF;
  uint16_t time_mid  = time_ms & 0xFFFF;
  uint16_t time_low_ver = (random::randint(0, 0xFFF) & 0x0FFF) | 0x7000;
  uint16_t clk_seq_var  = (random::randint(0, 0x3FFF) & 0x3FFF) | 0x8000;
  uint64_t node = (uint64_t)random::randint(0, 0xFFFF) << 32 | random::randint(0, 0xFFFFFFFF);
  node &= 0xFFFFFFFFFFFF;
  
  std::stringstream ss;
  ss << std::hex << std::setfill('0')
     << std::setw(8) << time_high << '-'
     << std::setw(4) << time_mid << '-'
     << std::setw(4) << time_low_ver << '-'
     << std::setw(4) << clk_seq_var << '-'
     << std::setw(12) << node;
  return ss.str();
}

inline std::string v8() {
  uint32_t time_low = random::randint(0, 0xFFFFFFFF);
  uint32_t time_mid = random::randint(0, 0xFFFF);
  uint32_t time_hi_and_version = (random::randint(0, 0xFFF) & 0x0FFF) | 0x8000;
  uint32_t clk_seq_and_variant = (random::randint(0, 0x3FFF) & 0x3FFF) | 0x8000;
  uint64_t node = (uint64_t)random::randint(0, 0xFFFF) << 32 | random::randint(0, 0xFFFFFFFF);
  node &= 0xFFFFFFFFFFFF;
  
  std::stringstream ss;
  ss << std::hex << std::setfill('0')
     << std::setw(8) << time_low << '-'
     << std::setw(4) << time_mid << '-'
     << std::setw(4) << time_hi_and_version << '-'
     << std::setw(4) << clk_seq_and_variant << '-'
     << std::setw(12) << node;
  return ss.str();
}

} // uuid

namespace dmath {

inline int pow(int a, int y) {
  int result = 1;
  int abs_y = (y < 0) ? -y : y; 
  for (int i = 0; i < abs_y; i++) {
    result *= a;
  }
  if (y < 0) {
    return 1 / result;
  }
  return result;
}

inline double pow(double a, double y) {
  double result = 1.0;
  double abs_y = (y < 0) ? -y : y; 
  for (int i = 0; i < abs_y; i++) {
    result *= a;
  }
  if (y < 0) {
    return 1.0 / result;
  }
  return result;
}

inline float pow(float a, float y) {
  float result = 1.0f;
  float abs_y = (y < 0) ? -y : y; 
  for (int i = 0; i < abs_y; i++) {
    result *= a;
  }
  if (y < 0) {
    return 1.0f / result;
  }
  return result;
}

inline int abs(int x) {
  if (x < 0 ) {
    return -x;
  }
  return x;
}

inline double abs(double x) {
  if (x < 0 ) {
    return -x;
  }
  return x;
}

inline float abs(float x) {
  if (x < 0 ) {
    return -x;
  }
  return x;
}

inline double trunc(double x) {
  int n = 0;
  if (x >= 0) {
    while (x >= 1.0) {
      x -= 1.0;
      n++;
    }
  } else {
    while (x <= -1.0) {
      x += 1.0;
      n--;
    }
  }
  return n;
}

inline int trunc(int x) {
  int n = 0;
  if (x >= 0) {
    while (x >= 1) {
      x -= 1;
      n++;
    }
  } else {
    while (x <= -1) {
      x += 1;
      n--;
    }
  }
  return n;
}

inline float trunc(float x) {
  float n = 0.0f;
  if (x >= 0.0f) {
    while (x >= 1.0f) {
      x -= 1.0f;
      n++;
    }
  } else {
    while (x <= -1.0f) {
      x += 1.0f;
      n--;
    }
  }
  return n;
}

inline double lerp(double a, double b, double t) {
  return a + t * (b - a);
}

inline float lerp(float a, float b, float t) {
  return a + t * (b - a);
}

inline int lerp(int a, int b, int t) {
  return a + t * (b - a);
}

inline double clamp(double x, double min, double max) {
  if (x < min) { return min; };
  if (x > max) { return max; };
  return x;
}

inline float clamp(float x, float min, float max) {
  if (x < min) { return min; };
  if (x > max) { return max; };
  return x;
}

inline int clamp(int x, int min, int max) {
  if (x < min) { return min; };
  if (x > max) { return max; };
  return x;
}

inline double sqrt(double x) {
  if (x < 0) { return 0; };
  double out = x / 2.0;
  while (true) {
    double next = 0.5 * (out + x / out);
    if (abs(out - next) < 0.00001) { return next; };
    out = next;
  }
}

inline float sqrt(float x) {
  if (x < 0.0f) { return 0; };
  float out = x / 2.0f;
  while (true) {
    float next = 0.5f * (out + x / out);
    if (abs(out - next) < 0.00001f) { return next; };
    out = next;
  }
}

inline int sqrt(int x) {
  if (x <= 0) return 0;
  if (x == 1) return 1;
  int out = x >> 1;
  while (true) {
    int next = (out + x / out) >> 1;
    if (next >= out) { 
      return out;
    }
    out = next;
  }
}

inline int gcd(int a, int b) {
  while (b != 0) {
    int tp = b;
    b = a % b;
    a = tp;
  }
  return a;
}

inline int lcm(int a, int b) {
  return a / gcd(a, b) * b;
}


} // dmath

namespace hash {

inline void gencrc32(uint32_t(&table)[256]) {
  uint32_t a = 0xEDB88320;
  for (uint32_t i = 0; i < 256; i++) {
    uint32_t c = i;
    for (size_t j = 0; j < 8; j++) {
      if (c & 1) {
        c = a ^ (c >> 1);
      } else {
        c >>=1;
      }
    }
    table[i] = c;
  }
}

inline uint32_t updcrc32(uint32_t (&table)[256], uint32_t initial, const void* buf, size_t len) {
  uint32_t c = initial ^ 0xFFFFFFFF;
  const uint8_t* u = static_cast<const uint8_t*>(buf);
  for (size_t i = 0; i < len; ++i) {
    c = table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
  }
  return c ^ 0xFFFFFFFF;
}

inline void genSha256(const void* data, size_t len, uint8_t* hash) {
  static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
  };
  uint32_t s[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
  };
  uint8_t b[64];
  int bl = 0;
  uint64_t bits = 0;
  const uint8_t* p = (const uint8_t*)data;
  for (size_t i = 0; i < len; ++i) {
    b[bl++] = p[i];
    if (bl == 64) {
      uint32_t m[64];
      for (int t = 0; t < 16; ++t)
        m[t] = (b[t*4] << 24) | (b[t*4+1] << 16) | (b[t*4+2] << 8) | b[t*4+3];
      for (int t = 16; t < 64; ++t) {
        uint32_t s0 = ((m[t-15] >> 7) | (m[t-15] << 25)) ^ ((m[t-15] >> 18) | (m[t-15] << 14)) ^ (m[t-15] >> 3);
        uint32_t s1 = ((m[t-2] >> 17) | (m[t-2] << 15)) ^ ((m[t-2] >> 19) | (m[t-2] << 13)) ^ (m[t-2] >> 10);
        m[t] = m[t-16] + s0 + m[t-7] + s1;
      }
      uint32_t a = s[0], c = s[1], d = s[2], e = s[3];
      uint32_t f = s[4], g = s[5], h = s[6], i = s[7];
      for (int t = 0; t < 64; ++t) {
        uint32_t s1 = ((f >> 6) | (f << 26)) ^ ((f >> 11) | (f << 21)) ^ ((f >> 25) | (f << 7));
        uint32_t ch = (f & g) ^ (~f & h);
        uint32_t t1 = i + s1 + ch + k[t] + m[t];
        uint32_t s0 = ((a >> 2) | (a << 30)) ^ ((a >> 13) | (a << 19)) ^ ((a >> 22) | (a << 10));
        uint32_t maj = (a & c) ^ (a & d) ^ (c & d);
        uint32_t t2 = s0 + maj;
        i = h; h = g; g = f; f = e + t1;
        e = d; d = c; c = a; a = t1 + t2;
      }
      s[0] += a; s[1] += c; s[2] += d; s[3] += e;
      s[4] += f; s[5] += g; s[6] += h; s[7] += i;
      bits += 512;
      bl = 0;
    }
  }
  bits += bl * 8;
  b[bl++] = 0x80;
  if (bl > 56) {
    while (bl < 64) b[bl++] = 0;
    uint32_t m[64];
    for (int t = 0; t < 16; ++t)
      m[t] = (b[t*4] << 24) | (b[t*4+1] << 16) | (b[t*4+2] << 8) | b[t*4+3];
    for (int t = 16; t < 64; ++t) {
      uint32_t s0 = ((m[t-15] >> 7) | (m[t-15] << 25)) ^ ((m[t-15] >> 18) | (m[t-15] << 14)) ^ (m[t-15] >> 3);
      uint32_t s1 = ((m[t-2] >> 17) | (m[t-2] << 15)) ^ ((m[t-2] >> 19) | (m[t-2] << 13)) ^ (m[t-2] >> 10);
      m[t] = m[t-16] + s0 + m[t-7] + s1;
    }
    uint32_t a = s[0], c = s[1], d = s[2], e = s[3];
    uint32_t f = s[4], g = s[5], h = s[6], i = s[7];
    for (int t = 0; t < 64; ++t) {
      uint32_t s1 = ((f >> 6) | (f << 26)) ^ ((f >> 11) | (f << 21)) ^ ((f >> 25) | (f << 7));
      uint32_t ch = (f & g) ^ (~f & h);
      uint32_t t1 = i + s1 + ch + k[t] + m[t];
      uint32_t s0 = ((a >> 2) | (a << 30)) ^ ((a >> 13) | (a << 19)) ^ ((a >> 22) | (a << 10));
      uint32_t maj = (a & c) ^ (a & d) ^ (c & d);
      uint32_t t2 = s0 + maj;
      i = h; h = g; g = f; f = e + t1;
      e = d; d = c; c = a; a = t1 + t2;
    }
    s[0] += a; s[1] += c; s[2] += d; s[3] += e;
    s[4] += f; s[5] += g; s[6] += h; s[7] += i;
    memset(b, 0, 56);
  }
  while (bl < 56) b[bl++] = 0;
  b[56] = bits >> 56; b[57] = bits >> 48; b[58] = bits >> 40; b[59] = bits >> 32;
  b[60] = bits >> 24; b[61] = bits >> 16; b[62] = bits >> 8;  b[63] = bits;
  {
    uint32_t m[64];
    for (int t = 0; t < 16; ++t)
      m[t] = (b[t*4] << 24) | (b[t*4+1] << 16) | (b[t*4+2] << 8) | b[t*4+3];
    for (int t = 16; t < 64; ++t) {
      uint32_t s0 = ((m[t-15] >> 7) | (m[t-15] << 25)) ^ ((m[t-15] >> 18) | (m[t-15] << 14)) ^ (m[t-15] >> 3);
      uint32_t s1 = ((m[t-2] >> 17) | (m[t-2] << 15)) ^ ((m[t-2] >> 19) | (m[t-2] << 13)) ^ (m[t-2] >> 10);
      m[t] = m[t-16] + s0 + m[t-7] + s1;
    }
    uint32_t a = s[0], c = s[1], d = s[2], e = s[3];
    uint32_t f = s[4], g = s[5], h = s[6], i = s[7];
    for (int t = 0; t < 64; ++t) {
      uint32_t s1 = ((f >> 6) | (f << 26)) ^ ((f >> 11) | (f << 21)) ^ ((f >> 25) | (f << 7));
      uint32_t ch = (f & g) ^ (~f & h);
      uint32_t t1 = i + s1 + ch + k[t] + m[t];
      uint32_t s0 = ((a >> 2) | (a << 30)) ^ ((a >> 13) | (a << 19)) ^ ((a >> 22) | (a << 10));
      uint32_t maj = (a & c) ^ (a & d) ^ (c & d);
      uint32_t t2 = s0 + maj;
      i = h; h = g; g = f; f = e + t1;
      e = d; d = c; c = a; a = t1 + t2;
    }
    s[0] += a; s[1] += c; s[2] += d; s[3] += e;
    s[4] += f; s[5] += g; s[6] += h; s[7] += i;
  }
  for (int j = 0; j < 4; ++j) {
    hash[j]      = s[0] >> (24 - j * 8);
    hash[j + 4]  = s[1] >> (24 - j * 8);
    hash[j + 8]  = s[2] >> (24 - j * 8);
    hash[j + 12] = s[3] >> (24 - j * 8);
    hash[j + 16] = s[4] >> (24 - j * 8);
    hash[j + 20] = s[5] >> (24 - j * 8);
    hash[j + 24] = s[6] >> (24 - j * 8);
    hash[j + 28] = s[7] >> (24 - j * 8);
  }
}

inline void genFnv1a32(const void* data, size_t len, uint8_t* hash) {
  const uint8_t* p = (const uint8_t*)data;
  uint32_t h = 0x811c9dc5;
  for (size_t i = 0; i < len; ++i) {
    h ^= p[i];
    h *= 0x1000193;
  }
  hash[0] = h >> 24;
  hash[1] = h >> 16;
  hash[2] = h >> 8;
  hash[3] = h;
}

inline void genFnv1a64(const void* data, size_t len, uint8_t* hash) {
  const uint8_t* p = (const uint8_t*)data;
  uint64_t h = 0xcbf29ce484222325;
  for (size_t i = 0; i < len; ++i) {
    h ^= p[i];
    h *= 0x100000001b3;
  }
  hash[0] = h >> 56;
  hash[1] = h >> 48;
  hash[2] = h >> 40;
  hash[3] = h >> 32;
  hash[4] = h >> 24;
  hash[5] = h >> 16;
  hash[6] = h >> 8;
  hash[7] = h;
}

} // hash

namespace crypto {

inline void ChaCha20(const void* data, size_t len, const uint8_t key[32], const uint8_t nonce[8], uint64_t counter, uint8_t* out) {
  const uint8_t magic[16] = {'e','x','p','a','n','d',' ','3','2','-','b','y','t','e',' ','k'};
  const uint8_t* in = (const uint8_t*)data;
  uint32_t state[16];

  auto pack4 = [](const uint8_t* a) -> uint32_t {
    return (uint32_t)a[0] | ((uint32_t)a[1] << 8) | ((uint32_t)a[2] << 16) | ((uint32_t)a[3] << 24);
  };

  state[0] = pack4(magic);
  state[1] = pack4(magic + 4);
  state[2] = pack4(magic + 8);
  state[3] = pack4(magic + 12);
  for (int i = 0; i < 8; ++i) state[4+i] = pack4(key + i*4);
  state[12] = (uint32_t)counter;
  state[13] = (uint32_t)(counter >> 32);
  state[14] = pack4(nonce);
  state[15] = pack4(nonce + 4);

  uint8_t ks[64];
  size_t pos = 64;

  for (size_t i = 0; i < len; ++i) {
    if (pos >= 64) {
      uint32_t x[16];
      for (int j = 0; j < 16; ++j) x[j] = state[j];

      for (int r = 0; r < 10; ++r) {
        auto qr = [](uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d) {
          a += b; d ^= a; d = (d << 16) | (d >> 16);
          c += d; b ^= c; b = (b << 12) | (b >> 20);
          a += b; d ^= a; d = (d << 8)  | (d >> 24);
          c += d; b ^= c; b = (b << 7)  | (b >> 25);
        };
        qr(x[0], x[4], x[8],  x[12]); qr(x[1], x[5], x[9],  x[13]);
        qr(x[2], x[6], x[10], x[14]); qr(x[3], x[7], x[11], x[15]);
        qr(x[0], x[5], x[10], x[15]); qr(x[1], x[6], x[11], x[12]);
        qr(x[2], x[7], x[8],  x[13]); qr(x[3], x[4], x[9],  x[14]);
      }

      for (int j = 0; j < 16; ++j) {
        x[j] += state[j];
        ks[j*4]   = (uint8_t)x[j];
        ks[j*4+1] = (uint8_t)(x[j] >> 8);
        ks[j*4+2] = (uint8_t)(x[j] >> 16);
        ks[j*4+3] = (uint8_t)(x[j] >> 24);
      }

      if (!++state[12]) ++state[13];
      pos = 0;
    }
    out[i] = in[i] ^ ks[pos++];
  }
}

} // crypto

namespace encode {

inline size_t encbase64(const uint8_t* data, size_t len, char* out) {
  static const char* map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  size_t j = 0;

  for (size_t i = 0; i < len; i += 3) {
    uint8_t b0 = data[i];
    uint8_t b1 = (i + 1 < len) ? data[i + 1] : 0;
    uint8_t b2 = (i + 2 < len) ? data[i + 2] : 0;

    uint32_t triple = (b0 << 16) | (b1 << 8) | b2;

    out[j++] = map[(triple >> 18) & 0x3F];
    out[j++] = map[(triple >> 12) & 0x3F];
    out[j++] = (i + 1 < len) ? map[(triple >> 6) & 0x3F] : '=';
    out[j++] = (i + 2 < len) ? map[triple & 0x3F] : '=';
  }

  out[j] = '\0';
  return j;
}

inline size_t decbase64(const char* data, size_t len, uint8_t* out) {
  static const uint8_t rev[128] = {
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
  };

  size_t j = 0;

  for (size_t i = 0; i < len; i += 4) {
    uint8_t c0 = rev[(int)data[i]];
    uint8_t c1 = rev[(int)data[i + 1]];
    uint8_t c2 = (data[i + 2] == '=') ? 64 : rev[(int)data[i + 2]];
    uint8_t c3 = (data[i + 3] == '=') ? 64 : rev[(int)data[i + 3]];

    uint32_t triple = (c0 << 18) | (c1 << 12) | (c2 << 6) | c3;

    out[j++] = triple >> 16;

    if (data[i + 2] != '=')
      out[j++] = triple >> 8;

    if (data[i + 3] != '=')
      out[j++] = triple;
  }

  return j;
}

} // encode

} // dsl

//---------------------------------------------------------------------------------\
//                                                                                 |
//                                  MIT License                                    |
//                                                                                 |
//                          Copyright (c) 2026 retti                               |
//                                                                                 |
//  Permission is hereby granted, free of charge, to any person obtaining a copy   |
//  of this software and associated documentation files (the "Software"), to deal  |
//  in the Software without restriction, including without limitation the rights   |
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      |
//  copies of the Software, and to permit persons to whom the Software is          |
//  furnished to do so, subject to the following conditions:                       |
//                                                                                 |
//  The above copyright notice and this permission notice shall be included in all |
//  copies or substantial portions of the Software.                                |
//                                                                                 |
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     |
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       |
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    |
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         |
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  |
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  |
//  SOFTWARE.                                                                      |
// --------------------------------------------------------------------------------/
