#ifndef ENDIAN_HPP
#define ENDIAN_HPP

#include <cstdint>

class Endian
{
public:
    /*
     * Bit endian
     */
    static inline uint8_t *UInt16ToBytes(uint16_t i)
    {
        static uint8_t bytes[2];
        bytes[0] = static_cast<uint8_t>((i >> 8) & 0xff);
        bytes[1] = static_cast<uint8_t>(i & 0xff);
        return bytes;
    }

    static inline uint16_t BytesToUInt16(uint8_t *bytes)
    {
        return (static_cast<uint16_t>(bytes[0]) << 8)
               + (static_cast<uint16_t>(bytes[1]));
    }

    static inline uint8_t *Int32ToBytes(int32_t i)
    {
        static uint8_t bytes[4];
        bytes[0] = static_cast<uint8_t>((i >> 24) & 0xff);
        bytes[1] = static_cast<uint8_t>((i >> 16) & 0xff);
        bytes[2] = static_cast<uint8_t>((i >> 8) & 0xff);
        bytes[3] = static_cast<uint8_t>(i & 0xff);
        return bytes;
    }

    static inline int32_t BytesToInt32(uint8_t *bytes)
    {
        return (static_cast<int32_t>(bytes[0]) << 24)
               + (static_cast<int32_t>(bytes[1]) << 16)
               + (static_cast<int32_t>(bytes[2]) << 8)
               + static_cast<int32_t>(bytes[3]);
    }

    static inline uint8_t *Int64ToBytes(int64_t i)
    {
        static uint8_t bytes[8];
        bytes[0] = static_cast<uint8_t>((i >> 56) & 0xff);
        bytes[1] = static_cast<uint8_t>((i >> 48) & 0xff);
        bytes[2] = static_cast<uint8_t>((i >> 40) & 0xff);
        bytes[3] = static_cast<uint8_t>((i >> 32) & 0xff);
        bytes[4] = static_cast<uint8_t>((i >> 24) & 0xff);
        bytes[5] = static_cast<uint8_t>((i >> 16) & 0xff);
        bytes[6] = static_cast<uint8_t>((i >> 8) & 0xff);
        bytes[7] = static_cast<uint8_t>(i & 0xff);
        return bytes;
    }

    static inline int64_t BytesToInt64(uint8_t *bytes)
    {
        return (static_cast<int64_t>(bytes[0]) << 56)
               + (static_cast<int64_t>(bytes[1]) << 48)
               + (static_cast<int64_t>(bytes[2]) << 40)
               + (static_cast<int64_t>(bytes[3]) << 32)
               + (static_cast<int64_t>(bytes[4]) << 24)
               + (static_cast<int64_t>(bytes[5]) << 16)
               + (static_cast<int64_t>(bytes[6]) << 8)
               + static_cast<int64_t>(bytes[7]);
    }

    static inline uint8_t *Float32ToBytes(float_t f)
    {
        static uint8_t bytes[4];
        const uint8_t *floatBytes = reinterpret_cast<uint8_t *>(&f);
        memcpy(bytes, floatBytes, 4);
        return bytes;
    }

    static inline float_t BytesToFloat32(uint8_t *bytes)
    {
        float_t f;
        memcpy(&f, bytes, 4);
        return f;
    }

    static inline uint8_t *Float64ToBytes(double_t d)
    {
        static uint8_t bytes[8];
        const uint8_t *floatBytes = reinterpret_cast<uint8_t *>(&d);
        memcpy(bytes, floatBytes, 8);
        return bytes;
    }

    static inline double_t BytesToFloat64(uint8_t *bytes)
    {
        double_t d;
        memcpy(&d, bytes, 8);
        return d;
    }
};

#endif //ENDIAN_HPP
