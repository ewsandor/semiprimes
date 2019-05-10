#include "uhuge.hpp"

#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>

void Uhuge::setZero(){
  
  memset(&chunks,0,sizeof(chunks));
}

Uhuge::Uhuge(){

  setZero();
}

Uhuge::Uhuge(uint64_t value){
  
  setZero();

  chunks[0] = value;
}

Uhuge::Uhuge(Uhuge * value){

  memcpy(&chunks, &value->chunks, sizeof(chunks));
}

Uhuge::Uhuge(std::string str){

  int i = 0;
  setZero();

  while(str[i] >= '0' && str[i] <= '9')
  {
    Uhuge(multiply((uint64_t) 10));
    Uhuge(add((uint64_t) (str[i] - '0')));
  }
}

Uhuge Uhuge::add(Uhuge * rhs){

  int i;
  bool carried = false;

  Uhuge sum(this);

  sum.chunks[0] += rhs->chunks[0];

  for(i = 1; i < CHUNKCOUNT; i++)
  {
    if( (sum.chunks[i-1] < rhs->chunks[i-1]) ||
        (carried && sum.chunks[i-1] == rhs->chunks[i-1]) )
    {
      carried = true;
      sum.chunks[i]++;
    }
    else
    {
      carried = false;
    }

    sum.chunks[i] += rhs->chunks[i];
  }

  return sum;
}

Uhuge Uhuge::add(uint64_t rhs){

  int i;
  uint64_t prev;

  Uhuge sum(this);

  prev = sum.chunks[0];
  sum.chunks[0] += rhs;

  for(i = 1; i < CHUNKCOUNT; i++)
  {
    if(sum.chunks[i-1] < prev)
    {
      prev = sum.chunks[i];
      sum.chunks[i]++;
    }
    else
    {
      break;
    }
  }

  return sum;
}

Uhuge Uhuge::subtract(Uhuge * rhs){

  int i;
  bool borrowed = false;

  Uhuge difference(this);

  difference.chunks[0] -= rhs->chunks[0];

  for(i = 1; i < CHUNKCOUNT; i++)
  {
    if( (difference.chunks[i-1] > rhs->chunks[i-1]) ||
        (borrowed && difference.chunks[i-1] == rhs->chunks[i-1]) )
    {
      borrowed = true;
      difference.chunks[i]--;
    }
    else
    {
      borrowed = false;
    }

    difference.chunks[i] -= rhs->chunks[i];
  }

  return difference;
} 

Uhuge Uhuge::subtract(uint64_t rhs){

  int i;
  uint64_t prev;

  Uhuge difference(this);

  prev = difference.chunks[0];
  difference.chunks[0] -= rhs;

  for(i = 1; i < CHUNKCOUNT; i++)
  {
    if(difference.chunks[i-1] > prev)
    {
      prev = difference.chunks[i];
      difference.chunks[i]--;
    }
    else
    {
      break;
    }
  }

  return difference;
}

Uhuge Uhuge::multiply(Uhuge * rhs){
  Uhuge product;

  return product;
}

Uhuge Uhuge::multiply(uint64_t rhs)
{
  Uhuge product;
  
  return product;
}

Uhuge Uhuge::divide(Uhuge * rhs){

  Uhuge  quotient;

  return quotient;
}
Uhuge Uhuge::divide(uint64_t rhs){

  Uhuge  quotient;

  return quotient;
}

Uhuge Uhuge::mod(Uhuge * rhs){

  Uhuge tmp = divide(rhs).multiply(rhs);

  return subtract(&tmp);
}

Uhuge Uhuge::mod(uint64_t rhs){

  Uhuge tmp = divide(rhs).multiply(rhs);

  return subtract(&tmp);
}

bool Uhuge::equals(Uhuge * obj){

  int i;
  bool ret_val = true;

  for(i = 0; i < CHUNKCOUNT; i++)
  {
    if(chunks[i] != obj->chunks[i])
    {
      ret_val = false;
      break;
    }
  }
  
  return ret_val;
}

bool Uhuge::equals(uint64_t obj){

  int i;
  bool ret_val = (chunks[0] == obj);

  if(ret_val)
  {
    for(i = 1; i < CHUNKCOUNT; i++)
    {
      if(0 != chunks[i])
      {
        ret_val = false;
        break;
      }
    }
  }
  
  return ret_val;
}

bool Uhuge::greaterThan(Uhuge * obj){
  
  int i;
  bool ret_val = true;

  for(i = (CHUNKCOUNT - 1); i >= 0 ; i--)
  {
    if(chunks[i] != obj->chunks[i])
    {
      ret_val = chunks[i] > obj->chunks[i];
      break;
    }
  }
  
  return ret_val;
}

bool Uhuge::greaterThan(uint64_t obj){
  
  int i;
  bool ret_val = (chunks[0] > obj);

  if(ret_val)
  {
    for(i = 1; i < CHUNKCOUNT; i++)
    {
      if(0 != chunks[i])
      {
        ret_val = false;
        break;
      }
    }
  }
  
  return ret_val;
}

bool Uhuge::greaterThanEqual(Uhuge * obj){
  
  int i;
  bool ret_val = true;

  for(i = (CHUNKCOUNT - 1); i >= 0 ; i--)
  {
    if(chunks[i] != obj->chunks[i])
    {
      ret_val = chunks[i] >= obj->chunks[i];
      break;
    }
  }
  
  return ret_val;
}

bool Uhuge::greaterThanEqual(uint64_t obj){
  
  int i;
  bool ret_val = (chunks[0] >= obj);

  if(ret_val)
  {
    for(i = 1; i < CHUNKCOUNT; i++)
    {
      if(0 != chunks[i])
      {
        ret_val = false;
        break;
      }
    }
  }
  
  return ret_val;
}

bool Uhuge::lessThan(Uhuge * obj){
  
  int i;
  bool ret_val = true;

  for(i = (CHUNKCOUNT - 1); i >= 0 ; i--)
  {
    if(chunks[i] != obj->chunks[i])
    {
      ret_val = chunks[i] < obj->chunks[i];
      break;
    }
  }
  
  return ret_val;
}

bool Uhuge::lessThan(uint64_t obj){
  
  int i;
  bool ret_val = (chunks[0] < obj);

  if(ret_val)
  {
    for(i = 1; i < CHUNKCOUNT; i++)
    {
      if(0 != chunks[i])
      {
        ret_val = false;
        break;
      }
    }
  }
  
  return ret_val;
}

bool Uhuge::lessThanEqual(Uhuge * obj){
  
  int i;
  bool ret_val = true;

  for(i = (CHUNKCOUNT - 1); i >= 0 ; i--)
  {
    if(chunks[i] != obj->chunks[i])
    {
      ret_val = chunks[i] <= obj->chunks[i];
      break;
    }
  }
  
  return ret_val;
}

bool Uhuge::lessThanEqual(uint64_t obj){
  
  int i;
  bool ret_val = (chunks[0] <= obj);

  if(ret_val)
  {
    for(i = 1; i < CHUNKCOUNT; i++)
    {
      if(0 != chunks[i])
      {
        ret_val = false;
        break;
      }
    }
  }
  
  return ret_val;
}

Uhuge Uhuge::divide10()
{
  return divide((uint64_t) 10);
}

bool Uhuge::divisible2(){
  return (chunks[0] % 2) == 0;
}

std::string Uhuge::toString() {
  
  std::string out = "";
  Uhuge temp = Uhuge(this);

  while(!temp.equals((uint64_t) 0))
  {
    out.insert(0, 1, (char)(temp.mod((uint64_t) 10).chunks[0] - '0'));
    temp = temp.divide10();
  }
  
  return out;
}

void Uhuge::print() {
  std::cout << toString() << "\n";
}
