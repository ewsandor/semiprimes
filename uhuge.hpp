#ifndef _UHUGE_H_
#define _UHUGE_H_

#include <cstdint>
#include <string>

class Uhuge{

  private:
    #define CHUNKCOUNT 12
    uint64_t chunks[CHUNKCOUNT];
    void     setZero();

  public:
    Uhuge();
    Uhuge(uint64_t value);
    Uhuge(Uhuge * value);
    Uhuge(std::string str); 

    std::string toString();
    void        print();

    Uhuge       add(Uhuge * rhs);
    Uhuge       add(uint64_t rhs);
    Uhuge       subtract(Uhuge * rhs);
    Uhuge       subtract(uint64_t rhs);
    Uhuge       multiply(Uhuge * rhs);
    Uhuge       multiply(uint64_t rhs);
    Uhuge       divide(Uhuge * rhs);
    Uhuge       divide(uint64_t rhs);

    Uhuge       mod(Uhuge * rhs);
    Uhuge       mod(uint64_t rhs);

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
