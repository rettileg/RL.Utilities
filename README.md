# RL.Utilities
A collection of lightweight single‑header C++20 libraries designed for fast integration, zero dependencies, and clean architecture. Each module is a standalone .hpp file you can drop directly into any project.

Modules
rl_image.hpp
Single‑header image decoder supporting:
- PNG, TGA, HDR, BMP, DDS, QOI
- doc in file

dls.hpp
Single-header library:
- dmath - pow, abs, trunc, lerp, clamp, sqrt, gcd, lcm
- random - randint, randfloat, randbool, randrgba, randrgb, randhsl, randhsv, rand choise arr
- uuid - generate uuid v1/2/3/4/6/7/8 (5 not implemented)
- hash - crc32, sha-256, fnv1a 32/64
- encode - base64
- crypto - ChaCha20
