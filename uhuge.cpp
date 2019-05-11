#include "uhuge.hpp"

#include <assert.h>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>

void Uhuge::setZero(){
  
  memset(&chunks,0,sizeof(chunks));
}

uint64_t Uhuge::numberOfBits()
{
  int i, j;
  uint64_t bits = CHUNKBITS * CHUNKCOUNT;

  for(i = (CHUNKCOUNT-1); i >= 0; i--)
  {
    if(0 == this->chunks[i])
    {
      bits -= CHUNKBITS;
    }
    else
    {
      break;
    }
  }
  for(j = (CHUNKBITS - 1); j >= 0; j--)
  {
    if( 0 == (this->chunks[i] & (1LL << j)) )
    {
      bits--;
    }
    else
    {
      break;
    }
  }

  return bits;
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
  Uhuge temp;
  setZero();

  while(str[i] >= '0' && str[i] <= '9')
  {
    temp = multiply((uint64_t) 10);
    temp = temp.add((uint64_t) (str[i] - '0'));
    memcpy(&chunks, &temp.chunks, sizeof(chunks));
    i++;
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

  int i, j;
  uint64_t hold;
  Uhuge temp;
  Uhuge product;

  for(i = 0; i < CHUNKCOUNT; i++)
  {
    if(0 == rhs->chunks[i])
    {
      continue;
    }

    for(j = 0; j < CHUNKCOUNT; j++)
    {
      if(0 == chunks[j])
      {
        continue;
      }

      hold = CHUNK_L(rhs->chunks[i]) * CHUNK_L(chunks[j]);
      if(hold != 0)
      {
        temp = Uhuge();
        temp.chunks[i+j] = hold;
        product = product.add(&temp);
      }

      hold = CHUNK_L(rhs->chunks[i]) * CHUNK_H(chunks[j]);
      if(hold != 0)
      {
        temp = Uhuge();
        temp.chunks[i+j] = CHUNK_L_TO_H(hold);
        if(i+j+1 < CHUNKCOUNT)
        {
          temp.chunks[i+j+1] = CHUNK_H(hold);
        }
        product = product.add(&temp);
      }

      hold = CHUNK_H(rhs->chunks[i]) * CHUNK_L(chunks[j]);
      if(hold != 0)
      {
        temp = Uhuge();
        temp.chunks[i+j] = CHUNK_L_TO_H(hold);
        if(i+j+1 < CHUNKCOUNT)
        {
          temp.chunks[i+j+1] = CHUNK_H(hold);
        }
        product = product.add(&temp);
      }

      hold = CHUNK_H(rhs->chunks[i]) * CHUNK_H(chunks[j]);
      if(hold != 0)
      {
        temp = Uhuge();
        if(i+j+1 < CHUNKCOUNT)
        {
          temp.chunks[i+j+1] = hold;
          product = product.add(&temp);
        }
      }
    }
  }
 
  return product;
}

Uhuge Uhuge::multiply(uint64_t rhs)
{
  int i;
  uint64_t hold;
  Uhuge product;

  for(i = 0; i < CHUNKCOUNT; i++)
  {
    if(0 == chunks[i])
    {
      continue;
    }

    Uhuge temp;

    hold = CHUNK_L(chunks[i]) * CHUNK_L(rhs);
    if(hold != 0)
    {
      temp = Uhuge();
      temp.chunks[i] = hold;
      product = product.add(&temp);
    }

    hold = CHUNK_H(chunks[i]) * CHUNK_L(rhs);
    if(hold != 0)
    {
      temp = Uhuge();
      temp.chunks[i] = CHUNK_L_TO_H(hold);
      if(i+1 < CHUNKCOUNT)
      {
        temp.chunks[i+1] = CHUNK_H(hold);
      }
      product = product.add(&temp);
    }

    hold = CHUNK_L(chunks[i]) * CHUNK_H(rhs);
    if(hold != 0)
    {
      temp = Uhuge();
      temp.chunks[i] = CHUNK_L_TO_H(hold);
      if(i+1 < CHUNKCOUNT)
      {
        temp.chunks[i+1] = CHUNK_H(hold);
      }
      product = product.add(&temp);
    }

    hold = CHUNK_H(chunks[i]) * CHUNK_H(rhs);
    if(hold != 0)
    {
      temp = Uhuge();
      if(i+1 < CHUNKCOUNT)
      {
        temp.chunks[i+1] = hold;
        product = product.add(&temp);
      }
    }
  }
  
  return product;
}

Uhuge Uhuge::divide(Uhuge * rhs)
{
  assert(!rhs->equals((uint64_t) 0));

  bool found = false;
  int i, j;
  const uint64_t this_bits = numberOfBits();
  const uint64_t rhs_bits  = rhs->numberOfBits();
  Uhuge quotient;
  Uhuge product;

  for(i = (CHUNKCOUNT-1); i >= 0; i--)
  {
    for(j = (CHUNKBITS-1); j >= 0; j--)
    {
      quotient.chunks[i] |= (1LL << j);

      if((quotient.numberOfBits() + rhs_bits) > this_bits)
      {
        quotient.chunks[i] &= ~(1LL << j);
        continue;
      }
      
      product = rhs->multiply(&quotient);
      if(this->equals(&product))
      {
        found = true;
        break;
      }
      else if(product.greaterThan(this))
      {
        quotient.chunks[i] &= ~(1LL << j);
      }
    }
    if(found)
    {
      break;
    }
  }

  return quotient;
}

Uhuge Uhuge::divide(uint64_t rhs)
{
  assert(rhs != 0);

  bool found = false;
  int i, j;
  const uint64_t this_bits = numberOfBits();
  uint64_t       rhs_bits = 0;
  uint64_t       temp = rhs;
  Uhuge quotient;
  Uhuge product;

  while(temp)
  {
    rhs_bits++;
    temp >>= 1;
  }

  for(i = (CHUNKCOUNT-1); i >= 0; i--)
  {
    for(j = (CHUNKBITS-1); j >= 0; j--)
    {
      quotient.chunks[i] |= (1LL << j);

      if((quotient.numberOfBits() + rhs_bits) > this_bits)
      {
        quotient.chunks[i] &= ~(1LL << j);
        continue;
      }
      
      product = quotient.multiply(rhs);
      if(this->equals(&product))
      {
        found = true;
        break;
      }
      else if(product.greaterThan(this))
      {
        quotient.chunks[i] &= ~(1LL << j);
      }
    }
    if(found)
    {
      break;
    }
  }

  return quotient;
}

Uhuge Uhuge::divide2()
{
  return rightShift((uint64_t) 1);
}


Uhuge Uhuge::mod(Uhuge * rhs)
{

  Uhuge tmp = divide(rhs).multiply(rhs);

  return subtract(&tmp);
}

Uhuge Uhuge::mod(uint64_t rhs)
{

  Uhuge tmp = divide(rhs).multiply(rhs);

  return subtract(&tmp);
}

Uhuge Uhuge::leftShift(uint64_t positions)
{
  /* Currently only shifting upto 64 bits is supported */
  assert(positions <= CHUNKBITS);

  int i;
  Uhuge shifted = Uhuge(this);

  for(i = (CHUNKCOUNT - 1); i > 0; i--)
  {
    shifted.chunks[i] = ( shifted.chunks[i]   << positions ) |
                        ( shifted.chunks[i-1] >> (CHUNKBITS-positions) ) ;
  }

  shifted.chunks[0] = ( shifted.chunks[0] << positions );

  return shifted;
}

Uhuge Uhuge::rightShift(uint64_t positions)
{
  /* Currently only shifting upto 64 bits is supported */
  assert(positions <= CHUNKBITS);

  int i;
  Uhuge shifted = Uhuge(this);

  for(i = 0; i < (CHUNKCOUNT - 1); i++)
  {
    shifted.chunks[i] = ( shifted.chunks[i]   >> positions ) |
                        ( shifted.chunks[i+1] << (CHUNKBITS-positions) ) ;
  }

  shifted.chunks[CHUNKCOUNT - 1] = ( shifted.chunks[CHUNKCOUNT - 1] >> positions );

  return shifted;
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

bool Uhuge::divisible2()
{
  return (chunks[0] % 2) == 0;
}

std::string Uhuge::toString() 
{
  std::string out = "";
  Uhuge temp = Uhuge(this);

  while(!temp.equals((uint64_t) 0))
  {
    out.insert(0, 1, (char)(temp.mod((uint64_t) 10).chunks[0] + '0'));
    temp = temp.divide10();
  }
  
  return out;
}

std::string Uhuge::toHexString() 
{
  int digit;
  std::string out = "";
  Uhuge temp = Uhuge(this);

  while(!temp.equals((uint64_t) 0))
  {
    digit = (temp.chunks[0] & 0xF);

    if(digit < 0xA)
    {
      out.insert(0, 1, (char)(digit + '0'));
    }
    else
    {
      out.insert(0, 1, (char)((digit - 0xA) + 'A'));
    }

    temp = temp.rightShift(4);
  }
  
  return out;
}

void Uhuge::print()
{
  if(equals((uint64_t) 0))
  {
    std::cout << "0\n";
  }
  else
  {
    std::cout << toString() << "\n";
  }
}

void Uhuge::printHex()
{
  if(equals((uint64_t) 0))
  {
    std::cout << "0x0\n";
  }
  else
  {
    std::cout << "0x" << toHexString() << "\n";
  }
}
