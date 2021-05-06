#ifndef SEPTAL_H_
#define SEPTAL_H_

#include <stdint.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus


inline static void septal_encode(int64_t value, void *out);
inline static int64_t septal_decode(const void *data);


void septal_encode(int64_t value, void *out) {
    uint8_t *ptr = (uint8_t *)out;
    uint64_t x = (value == INT64_MIN) ? 0x8000000000000000 : llabs(value);

    // Encode first byte with sign.
    *ptr = x & 0x3F;
    if (value < 0) {
        *ptr |= 0x40;
    }
    x >>= 6;

    // Encode remaining bytes.
    while (x) {
        *ptr |= 0x80;
        ++ptr;
        *ptr = x & 0x7F;
        x >>= 7;
    }
}

int64_t septal_decode(const void *data) {
    const uint8_t *ptr = (uint8_t *)data;

    // Decode first byte.
    uint64_t x = *ptr & 0x3F;
    uint_fast8_t shift = 6;
    while (*ptr & 0x80) {
        ++ptr;
        const uint64_t chunk = *ptr & 0x7F;
        x |= chunk << shift;
        shift += 7;
    }
    return (int64_t)(*(uint8_t *)data & 0x40 ? -x : x);
}


#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif // SEPTAL_H_
