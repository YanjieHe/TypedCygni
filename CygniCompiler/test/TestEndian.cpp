#include <iostream>
#include <cmath>
#include "TestEndian.hpp"
#include "../Endian.hpp"

void TestEndian::TestAll()
{
    TestUInt16ToBytes();
    TestIntToBytes();
    TestLongToBytes();
    TestFloatToBytes();
    TestDoubleToBytes();
}

void TestEndian::TestUInt16ToBytes()
{
    uint16_t number = 1234;
    uint8_t *bytes = Endian::UInt16ToBytes(number);
    bool pass = Endian::BytesToUInt16(bytes) == number;
    std::cout << __FUNCTION__ << "  " << (pass ? "pass" : "fail") << std::endl;
}

void TestEndian::TestIntToBytes()
{
    int32_t number = 123456789;
    uint8_t *bytes = Endian::Int32ToBytes(number);
    bool pass = Endian::BytesToInt32(bytes) == number;
    std::cout << __FUNCTION__ << "  " << (pass ? "pass" : "fail") << std::endl;
}

void TestEndian::TestLongToBytes()
{
    int64_t number = 123456789123456789;
    uint8_t *bytes = Endian::Int64ToBytes(number);
    bool pass = Endian::BytesToInt64(bytes) == number;
    std::cout << __FUNCTION__ << "  " << (pass ? "pass" : "fail") << std::endl;
}

void TestEndian::TestFloatToBytes()
{
    float_t number = 12345.6789;
    uint8_t *bytes = Endian::Float32ToBytes(number);
    bool pass = std::abs(Endian::BytesToFloat32(bytes) - number) < 0.000001;
    std::cout << __FUNCTION__ << "  " << (pass ? "pass" : "fail") << std::endl;
}

void TestEndian::TestDoubleToBytes()
{
    double_t number = 12345.6789;
    uint8_t *bytes = Endian::Float64ToBytes(number);
    bool pass = std::abs(Endian::BytesToFloat64(bytes) - number) < 0.000001;
    std::cout << __FUNCTION__ << "  " << (pass ? "pass" : "fail") << std::endl;
}
