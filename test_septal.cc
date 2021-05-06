/**
 * @file
 *
 * Tests for the septal utility.
 */
#include <cstdint>

#include "gtest/gtest.h"

#include "septal.h"


namespace {

using std::uint8_t;


/**
 * @brief Gets sign bit from buffer.
 *
 * The sign bit is expected to be the first (Most significant)
 * bit after the continuation bit of the first byte of an encoded value.
 */
bool sign_bit(const uint8_t *buf) {
    return *buf & 0x40;
}

/**
 * @brief Gets continuation bit from buffer.
 *
 * The continuation bit is the first bit of an encoded byte.
 */
bool continuation_bit(uint8_t byte) {
    return byte & 0x80;
}

/**
 * @brief Gets value encoded into a septal encoded byte.
 *
 * The continuation bit is the first bit of an encoded byte.
 */
uint8_t byte_value(uint8_t byte, bool ignore_sign_position = false) {
    return byte & (ignore_sign_position ? 0x3F : 0x7F);
}


TEST(Septal, EncodeZero) {
    uint8_t buf[10] = {};
    septal_encode(0, buf);
    EXPECT_EQ(buf[0], 0);
}

TEST(Septal, EncodeSmall) {
    const int value = 7;
    uint8_t buf[10] = {};
    septal_encode(value, buf);

    EXPECT_FALSE(sign_bit(buf));
    EXPECT_FALSE(continuation_bit(buf[0]));
    EXPECT_EQ(byte_value(buf[0]), 7);
    EXPECT_EQ(buf[1], 0);
}

TEST(Septal, EncodeSmallNegative) {
    const int value = -12;
    uint8_t buf[10] = {};
    septal_encode(value, buf);

    EXPECT_TRUE(sign_bit(buf));
    EXPECT_FALSE(continuation_bit(buf[0]));
    EXPECT_EQ(byte_value(buf[0], true), 12);
    EXPECT_EQ(buf[1], 0);
}

TEST(Septal, EncodeShort) {
    const int value = 1000;
    uint8_t buf[10] = {};
    septal_encode(value, buf);

    // 1000 in binary is 0b1111101000.
    // The least significant 6 bits (0b101000) should be encoded first,
    // with a hex representation of 0x28.
    // The next 7 bits (0b1111) should be encoded into the second byte,
    // with a hex representation of 0xF.

    EXPECT_FALSE(sign_bit(buf));
    EXPECT_TRUE(continuation_bit(buf[0]));
    EXPECT_FALSE(continuation_bit(buf[1]));
    EXPECT_EQ(byte_value(buf[0]), 0x28);
    EXPECT_EQ(byte_value(buf[1]), 0xF);
    EXPECT_EQ(buf[2], 0);
}

TEST(Septal, DecodeZero) {
    uint8_t buf[] = {0x0};
    EXPECT_EQ(septal_decode(buf), 0);
}

TEST(Septal, DecodeSmall) {
    uint8_t buf[] = {0x07};
    EXPECT_EQ(septal_decode(buf), 7);
}

TEST(Septal, DecodeSmallNegative) {
    uint8_t buf[] = {0x4C};
    EXPECT_EQ(septal_decode(buf), -12);
}

TEST(Septal, DecodeShort) {
    uint8_t buf[] = {0xA8, 0x0F};
    EXPECT_EQ(septal_decode(buf), 1000);
}


}  // namespace
