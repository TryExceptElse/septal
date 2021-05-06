# Septal

A simple integer encoding scheme for encoding integers into the
smallest number of bytes possible.

Inspired by the integer encoding used by Protobuf, generalized for use
in C or C++.

The encoded format is optimized to consume the smallest number of bytes
in the common case; when encoding small numbers between -63 and +63,
only a single byte is used. While at the extremes of the representable
range this encoding scheme will be less bit-efficient than simple
fixed-width encoding (using 10 bytes to encode INT64_MAX or INT64_MIN),
the vast majority of real-world integer values fall relatively close to
0, and will benefit from an encoding scheme that omits the 
most-significant bytes when possible.
