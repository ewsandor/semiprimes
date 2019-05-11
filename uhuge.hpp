#ifndef _UHUGE_H_
#define _UHUGE_H_

#include <cstdint>
#include <string>

class Uhuge{

  private:
    #define CHUNKBITS  64
    #define CHUNKCOUNT 12
    #define CHUNK_L(chunk) (chunk & 0xffffFFFF)
    #define CHUNK_H(chunk) (chunk >> (CHUNKBITS/2))
    #define CHUNK_L_TO_H(chunk) (chunk << (CHUNKBITS/2))
    
    uint64_t chunks[CHUNKCOUNT];
    void     setZero();
    uint64_t numberOfBits();

  public:
    Uhuge();
    Uhuge(uint64_t value);
    Uhuge(Uhuge * value);
    Uhuge(std::string str); 

    std::string toString();
    std::string toHexString();
    void        print();
    void        printHex();

    Uhuge       add(Uhuge * rhs);
    Uhuge       add(uint64_t rhs);
    Uhuge       subtract(Uhuge * rhs);
    Uhuge       subtract(uint64_t rhs);
    Uhuge       multiply(Uhuge * rhs);
    Uhuge       multiply(uint64_t rhs);
    Uhuge       divide(Uhuge * rhs);
    Uhuge       divide(uint64_t rhs);
    Uhuge       divide2();

    Uhuge       mod(Uhuge * rhs);
    Uhuge       mod(uint64_t rhs);

    Uhuge       leftShift(uint64_t positions);
    Uhuge       rightShift(uint64_t positions);

    bool        equals(Uhuge * obj);
    bool        equals(uint64_t obj);
    bool        greaterThan(Uhuge * obj);
    bool        greaterThan(uint64_t obj);
    bool        greaterThanEqual(Uhuge * obj);
    bool        greaterThanEqual(uint64_t obj);
    bool        lessThan(Uhuge * obj);
    bool        lessThan(uint64_t obj);
    bool        lessThanEqual(Uhuge * obj);
    bool        lessThanEqual(uint64_t obj);

    Uhuge       divide10();
    bool        divisible2();
};
#endif
