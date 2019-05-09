#include "uhuge.hpp"

#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

 void Uhuge::setZero(){
    
    memset(&chunks,0,sizeof(chunks));
    RGROUP = pow(10.0, CHUNKSIZE);
  }

 Uhuge::Uhuge(){

    setZero();
  }
Uhuge::Uhuge(uint64_t value){
    
    setZero();

    chunks[CHUNKCOUNT - 1] = value % RGROUP;
    chunks[CHUNKCOUNT - 2] = value / RGROUP;
  }
Uhuge::Uhuge(Uhuge * value){
    
    RGROUP = value->getRGROUP();
    
    for (uint64_t i = 0; i < CHUNKCOUNT; i++){
      chunks[i] = value->getChunk(i);
    }
  }
Uhuge::Uhuge(string str){

    setZero();
    string value = "";
    
    for(uint64_t i = 0; i < str.length(); i++)
      if(str.at(i) >= 48 && str.at(i) <= 57)
  value += str.at(i);

    uint64_t i = 1;
    
    while(value.length() > CHUNKSIZE && i <= CHUNKCOUNT){
      chunks[CHUNKCOUNT - i] = parseInt(value.substr(value.length() - CHUNKSIZE));
      value = value.substr(0, value.length() - CHUNKSIZE);
      i ++;
    }
    chunks[CHUNKCOUNT - i] = parseInt(value);
  }
Uhuge::~Uhuge(){}

  Uhuge Uhuge::add(Uhuge * rhs){

    Uhuge sum;
    
    for(uint64_t i = CHUNKCOUNT - 1 ; i >= 0; i--) {
      sum.setChunk(i, chunks[i] + rhs->getChunk(i) + sum.getChunk(i));
  
      if(sum.getChunk(i) >= RGROUP){
      sum.setChunk(i, sum.getChunk(i) - RGROUP);
      if(i > 0)
        sum.setChunk(i - 1, sum.getChunk(i - 1) + 1);
      }
    }
    return sum;
  }
  Uhuge Uhuge::add(uint64_t rhs){

    Uhuge sum(this);
    
    sum.setChunk(CHUNKCOUNT - 1, chunks[CHUNKCOUNT - 1] + rhs);
    
    for(uint64_t i = CHUNKCOUNT - 1; i >= 0;i--){
      if (sum.getChunk(i) >= RGROUP){
  sum.setChunk(i, (sum.getChunk(i) - RGROUP));
  if(i > 0)
    sum.setChunk(i - 1, sum.getChunk(i - 1) + 1);
      
  while (sum.getChunk(i) >= RGROUP){
    sum.setChunk(i, (sum.getChunk(i) - RGROUP));
    if(i > 0)
      sum.setChunk(i - 1, sum.getChunk(i - 1) + 1);
  }
      }
      else
  break;
    }
    return sum;
  }
  Uhuge  Uhuge::subtract(Uhuge * rhs){
    Uhuge difference;
    Uhuge lhs(this);

    for(uint64_t i = CHUNKCOUNT - 1 ; i >= 0; i--) {
      if(lhs.getChunk(i) < rhs->getChunk(i))
      {
  for(uint64_t j = i - 1; j >= 0; j--){
    if(lhs.getChunk(j) > 0){
      lhs.setChunk(j, lhs.getChunk(j)-1);
      break;
    }
    else
      lhs.setChunk(j, RGROUP - 1);
  }  
  lhs.setChunk(i, chunks[i] + RGROUP);
      }     
      
      difference.setChunk(i, lhs.getChunk(i) - rhs->getChunk(i));
    }

    return difference;
  } 
  Uhuge  Uhuge::subtract(uint64_t rhs){
    Uhuge difference;
    Uhuge lhs(this);

    for(uint64_t i = CHUNKCOUNT - 1 ; i >= CHUNKCOUNT - 2; i--) {
      
      uint64_t val = 0;
      if(CHUNKCOUNT - 1 == i)
  val = rhs % RGROUP;
      else
  val = rhs / RGROUP;

      if(val <= lhs.getChunk(i))
  difference.setChunk(i, lhs.getChunk(i) - val);
      else{
  difference.setChunk(i, (lhs.getChunk(i)+RGROUP) - val);

  for(uint64_t j = i - 1; j >=0; j--){
    if(lhs.getChunk(j) == 0)
      lhs.setChunk(j, RGROUP - 1);
    else{
      lhs.setChunk(j, lhs.getChunk(j)-1);
      break;
    }
  }
      }
    }
    for(uint64_t i = CHUNKCOUNT - 3; i >= 0; i--){
      difference.setChunk(i, lhs.getChunk(i));
    }

    return difference;
  }
  Uhuge  Uhuge::multiply(Uhuge * rhs){
    Uhuge product;
    Uhuge tmp;

    long long carry = 0;

    for(uint64_t i = CHUNKCOUNT - 1; i >= 0; i--){
      for(uint64_t j = CHUNKCOUNT - 1; j >= 0; j--){
  long long holder =  ((long long) chunks[i] * (long long) rhs->getChunk(j)) + carry;

  carry = holder / RGROUP;
  holder = holder - (carry * RGROUP);

  tmp.setChunk(j, (int) holder);
      }      
      
      for (uint64_t k = 0; k < CHUNKCOUNT - 1 - i; k++)
  tmp = tmp.shiftLeft();
      

      product = product.add(&tmp);
      tmp = Uhuge();
      
    }

    return product;
  }
  Uhuge  Uhuge::multiply(uint64_t rhs)
  {
    Uhuge product;
    
    long carry = 0;
    
    for (uint64_t i = CHUNKCOUNT - 1; i >= 0; i--){
  long holder = (long)chunks[i] * (long)rhs + carry;
  
  carry = holder / RGROUP;
  holder = holder - (carry * RGROUP);
        
  product.setChunk(i, (int) holder);
    }
    
    return product;
  }
  Uhuge Uhuge::divide(Uhuge * rhs){

    if (rhs->greaterThan(this) || rhs->equals((uint64_t)0))
      return Uhuge();

    Uhuge  quotient;
    Uhuge  tester;
    Uhuge  goal(this);
    Uhuge  divisor(rhs);

    while(divisor.getChunk(CHUNKCOUNT - 1) % 10 == 0){
      goal = goal.divide10();
      divisor = divisor.divide10();
    }
    
    tester.setChunk(0, RGROUP / 10);

    while(tester.greaterThan((uint64_t) 0)){
    
      quotient = quotient.add(&tester);
      Uhuge  multTest = quotient.multiply(&divisor);
      
      while(multTest.lessThan(&goal)){
  quotient = quotient.add(&tester);
  multTest = quotient.multiply(&divisor);
      }

      if(multTest.equals(&goal))
  return quotient;

      quotient = quotient.subtract(&tester);
      tester = tester.divide10();
    }  

    return quotient;
  }
  Uhuge  Uhuge::divide(uint64_t rhs){
    Uhuge  quotient;
    
    quotient.setChunk(0, chunks[0] / rhs);
    
    long holder = chunks[0] - quotient.getChunk(0) * rhs;

    for(uint64_t i = 1; i < CHUNKCOUNT; i++){
      
      holder = holder * RGROUP + chunks[i];

      quotient.setChunk(i, holder / rhs);

      holder = holder - quotient.getChunk(i) * rhs; 
    }

    return quotient;
  }
  Uhuge  Uhuge::mod(Uhuge * rhs){
    Uhuge tmp = divide(rhs).multiply(rhs);
    return subtract(&tmp);
  }
  Uhuge  Uhuge::mod(uint64_t rhs){
    Uhuge tmp = divide(rhs).multiply(rhs);
    return subtract(&tmp);
  }





  bool Uhuge::equals(Uhuge * obj){
    //if(!(obj is Uhuge))
    //return false;

    for(uint64_t i = 0; i < CHUNKCOUNT; i++){
      if(chunks[i] != obj->getChunk(i))
    return false;
    }
    
    return true;
  }
  bool Uhuge::equals(uint64_t obj){
    //if(!(obj is Uhuge))
    //return false;

    for(uint64_t i = 0; i < CHUNKCOUNT - 1; i++){
      if(chunks[i] != 0)
    return false;
    }
    
    return chunks[CHUNKCOUNT - 1] == obj;
  }
  bool Uhuge::greaterThan(Uhuge * obj){
    
    for(uint64_t i = 0; i < CHUNKCOUNT; i++){
      
      if(chunks[i] > obj->getChunk(i))
  return true;

      if(chunks[i] < obj->getChunk(i))
  return false;
    }

    return false;
  }
  bool Uhuge::greaterThan(uint64_t obj){
    
    for(uint64_t i = 0; i < CHUNKCOUNT - 1; i++){
      
      if(chunks[i] > 0)
  return true;
    }
    if(chunks[CHUNKCOUNT - 1] > obj)
      return true;

    return false;
  }
  bool Uhuge::greaterThanEqual(Uhuge * obj){
    
    for(uint64_t i = 0; i < CHUNKCOUNT; i++){
      
      if(chunks[i] > obj->getChunk(i))
  return true;

      if(chunks[i] < obj->getChunk(i))
  return false;
    }

    return true;
  }
  bool Uhuge::greaterThanEqual(uint64_t obj){
    
    for(uint64_t i = 0; i < CHUNKCOUNT - 1; i++){
      
      if(chunks[i] > 0)
  return true;
    }
    if(chunks[CHUNKCOUNT - 1] >= obj)
      return true;

    return false;
  }
bool Uhuge::lessThan(Uhuge * obj){
    
    for(uint64_t i = 0; i < CHUNKCOUNT; i++){
      
      if(chunks[i] < obj->getChunk(i))
  return true;

      if(chunks[i] > obj->getChunk(i))
  return false;
    }

    return false;
  }
  bool Uhuge::lessThan(uint64_t obj){
    
    for(uint64_t i = 0; i < CHUNKCOUNT - 1; i++){
      
      if(chunks[i] > 0)
  return false;
    }
    if(chunks[CHUNKCOUNT - 1] < obj)
      return true;

    return false;
  }
  bool Uhuge::lessThanEqual(Uhuge * obj){
    
    for(uint64_t i = 0; i < CHUNKCOUNT; i++){
      
      if(chunks[i] < obj->getChunk(i))
  return true;

      if(chunks[i] > obj->getChunk(i))
  return false;
    }

    return true;
  }
  bool Uhuge::lessThanEqual(uint64_t obj){
    
    for(uint64_t i = 0; i < CHUNKCOUNT - 1; i++){
      
      if(chunks[i] > 0)
  return false;
    }
    if(chunks[CHUNKCOUNT - 1] <= obj)
      return true;

    return false;
  }
  Uhuge  Uhuge::shiftLeft(){
    Uhuge shifted;

    shifted.setChunk(0, CHUNKCOUNT - 1);

    for (uint64_t i = CHUNKCOUNT - 1; i > 0; i--){
      shifted.setChunk(i-1, chunks[i]);
    }

    return shifted;
  }
  Uhuge Uhuge::divide10()
  {
    Uhuge quotient(this);

    for (uint64_t i = CHUNKCOUNT-1; i > 0; i--)
      {
  quotient.setChunk(i, (quotient.getChunk(i) / 10) + (quotient.getChunk(i - 1) % 10 * RGROUP / 10));
      }
    quotient.setChunk(0, quotient.getChunk(0) / 10);
            
    return quotient;
  }
  uint64_t Uhuge::getChunk(uint64_t i) {
    return chunks[i];
  }
  void Uhuge::setChunk(uint64_t i, uint64_t value) {
    chunks[i] = value;
  }
  uint64_t Uhuge::getRGROUP() {
    return RGROUP;
  }
  void Uhuge::print() {

    for (uint64_t i = 0; i < CHUNKCOUNT; i++){
      cout << setfill('0') << setw(CHUNKSIZE) << chunks[i] << " ";
    }    
    
    cout << "\n";
  }
  uint64_t Uhuge::parseInt(string val){
    uint64_t intVal = 0;

    for(uint64_t i = 0; i < val.length(); i++)
      intVal = intVal * 10 + val.at(i) - 48;

    return intVal;
  }
  bool Uhuge::divisible2(){
    return chunks[CHUNKCOUNT - 1] % 2 == 0;
  }
