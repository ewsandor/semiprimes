#ifndef _UHUGE_H_
#define _UHUGE_H_

#include <cstdint>
#include <string>

using namespace std;

class Uhuge{

 private:
  static const uint64_t CHUNKCOUNT = 27;
  static const uint64_t CHUNKSIZE = 8;

  uint64_t chunks[CHUNKCOUNT];
  uint64_t RGROUP;


  void     setZero();
  uint64_t getRGROUP();

 public:
  Uhuge();
  Uhuge(uint64_t value);
  Uhuge(Uhuge * value);
  Uhuge(string str); 

  ~Uhuge();

  Uhuge shiftLeft();
  Uhuge divide10();
  Uhuge add(Uhuge * rhs);
  Uhuge add(uint64_t rhs);
  Uhuge subtract(Uhuge * rhs);
  Uhuge subtract(uint64_t rhs);
  Uhuge multiply(Uhuge * rhs);
  Uhuge multiply(uint64_t rhs);
  Uhuge divide(Uhuge * rhs);
  Uhuge divide(uint64_t rhs);
  Uhuge mod(Uhuge * rhs);
  Uhuge mod(uint64_t rhs);
  bool  equals(Uhuge * obj);
  bool  equals(uint64_t obj);
  bool  greaterThan(Uhuge * obj);
  bool  greaterThan(uint64_t obj);
  bool  greaterThanEqual(Uhuge * obj);
  bool  greaterThanEqual(uint64_t obj);
  bool  lessThan(Uhuge * obj);
  bool  lessThan(uint64_t obj);
  bool  lessThanEqual(Uhuge * obj);
  bool  lessThanEqual(uint64_t obj);
  bool  divisible2();

  void print();
  static uint64_t parseInt(string val);

  uint64_t getChunk(uint64_t i);
  void     setChunk(uint64_t i, uint64_t value);
};

#endif
