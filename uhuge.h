#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

using namespace std;

class Uhuge{

 public:
  static const int CHUNKCOUNT = 27;
  static const int CHUNKSIZE = 8;


		
 private:
  int chunks[CHUNKCOUNT];
  int RGROUP;


  void setZero(){

    //RGROUP = pow(10.0, CHUNKSIZE);
    RGROUP = 1;
    
    for (int i = 0; i < CHUNKSIZE; i++){
      RGROUP *= 10;
      chunks[i] = 0;
    }

    for (int i = CHUNKSIZE; i < CHUNKCOUNT; i++){
      chunks[i] = 0;
    }
  }




 public:
  Uhuge(){

    setZero();
  }
  Uhuge(int value){
    
    setZero();

    chunks[CHUNKCOUNT - 1] = value % RGROUP;
    chunks[CHUNKCOUNT - 2] = value / RGROUP;
  }
  Uhuge(Uhuge * value){
    
    RGROUP = value->getRGROUP();
    
    for (int i = 0; i < CHUNKCOUNT; i++){
      chunks[i] = value->getChunk(i);
    }
  }
  Uhuge(string str){

    setZero();
    string value = "";
    
    for(int i = 0; i < str.length(); i++)
      if(str.at(i) >= 48 && str.at(i) <= 57)
	value += str.at(i);

    int i = 1;
    
    while(value.length() > CHUNKSIZE && i <= CHUNKCOUNT){
      chunks[CHUNKCOUNT - i] = parseInt(value.substr(value.length() - CHUNKSIZE));
      value = value.substr(0, value.length() - CHUNKSIZE);
      i ++;
    }
    chunks[CHUNKCOUNT - i] = parseInt(value);
  }
  ~Uhuge(){}
  Uhuge add(Uhuge * rhs){

    Uhuge sum;
		
    for(int i = CHUNKCOUNT - 1 ; i >= 0; i--) {
      sum.setChunk(i, chunks[i] + rhs->getChunk(i) + sum.getChunk(i));
	
      if(sum.getChunk(i) >= RGROUP){
    	sum.setChunk(i, sum.getChunk(i) - RGROUP);
    	if(i > 0)
    	  sum.setChunk(i - 1, sum.getChunk(i - 1) + 1);
      }
    }
    return sum;
  }
  Uhuge add(int rhs){

    Uhuge sum(this);
		
    sum.setChunk(CHUNKCOUNT - 1, chunks[CHUNKCOUNT - 1] + rhs);
    
    for(int i = CHUNKCOUNT - 1; i >= 0;i--){
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
  Uhuge  subtract(Uhuge * rhs){
    Uhuge difference;
    Uhuge lhs(this);

    for(int i = CHUNKCOUNT - 1 ; i >= 0; i--) {
      if(lhs.getChunk(i) < rhs->getChunk(i))
      {
	for(int j = i - 1; j >= 0; j--){
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
  Uhuge  subtract(int rhs){
    Uhuge difference;
    Uhuge lhs(this);

    for(int i = CHUNKCOUNT - 1 ; i >= CHUNKCOUNT - 2; i--) {
      
      int val = 0;
      if(CHUNKCOUNT - 1 == i)
	val = rhs % RGROUP;
      else
	val = rhs / RGROUP;

      if(val <= lhs.getChunk(i))
	difference.setChunk(i, lhs.getChunk(i) - val);
      else{
	difference.setChunk(i, (lhs.getChunk(i)+RGROUP) - val);

	for(int j = i - 1; j >=0; j--){
	  if(lhs.getChunk(j) == 0)
	    lhs.setChunk(j, RGROUP - 1);
	  else{
	    lhs.setChunk(j, lhs.getChunk(j)-1);
	    break;
	  }
	}
      }
    }
    for(int i = CHUNKCOUNT - 3; i >= 0; i--){
      difference.setChunk(i, lhs.getChunk(i));
    }

    return difference;
  }
  Uhuge  multiply(Uhuge * rhs){
    Uhuge product;
    Uhuge tmp;

    long long carry = 0;

    for(int i = CHUNKCOUNT - 1; i >= 0; i--){
      for(int j = CHUNKCOUNT - 1; j >= 0; j--){
	long long holder =  ((long long) chunks[i] * (long long) rhs->getChunk(j)) + carry;

	carry = holder / RGROUP;
	holder = holder - (carry * RGROUP);

	tmp.setChunk(j, (int) holder);
      }      
      
      for (int k = 0; k < CHUNKCOUNT - 1 - i; k++)
	tmp = tmp.shiftLeft();
      

      product = product.add(&tmp);
      tmp = Uhuge();
      
    }

    return product;
  }
  Uhuge  multiply(int rhs)
  {
    Uhuge product;
    
    long carry = 0;
    
    for (int i = CHUNKCOUNT - 1; i >= 0; i--){
	long holder = (long)chunks[i] * (long)rhs + carry;
	
	carry = holder / RGROUP;
	holder = holder - (carry * RGROUP);
        
	product.setChunk(i, (int) holder);
    }
    
    return product;
  }
  Uhuge divide(Uhuge * rhs){

    if (rhs->greaterThan(this) || rhs->equals(0))
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

    while(tester.greaterThan(0)){
    
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
  Uhuge  divide(int rhs){
    Uhuge  quotient;
    
    quotient.setChunk(0, chunks[0] / rhs);
    
    long holder = chunks[0] - quotient.getChunk(0) * rhs;

    for(int i = 1; i < CHUNKCOUNT; i++){
      
      holder = holder * RGROUP + chunks[i];

      quotient.setChunk(i, holder / rhs);

      holder = holder - quotient.getChunk(i) * rhs; 
    }

    return quotient;
  }
  Uhuge  mod(Uhuge * rhs){
    Uhuge tmp = divide(rhs).multiply(rhs);
    return subtract(&tmp);
  }
  Uhuge  mod(int rhs){
    Uhuge tmp = divide(rhs).multiply(rhs);
    return subtract(&tmp);
  }





  bool equals(Uhuge * obj){
    //if(!(obj is Uhuge))
    //return false;

    for(int i = 0; i < CHUNKCOUNT; i++){
      if(chunks[i] != obj->getChunk(i))
  	return false;
    }
    
    return true;
  }
  bool equals(int obj){
    //if(!(obj is Uhuge))
    //return false;

    for(int i = 0; i < CHUNKCOUNT - 1; i++){
      if(chunks[i] != 0)
  	return false;
    }
    
    return chunks[CHUNKCOUNT - 1] == obj;
  }
  bool greaterThan(Uhuge * obj){
    
    for(int i = 0; i < CHUNKCOUNT; i++){
      
      if(chunks[i] > obj->getChunk(i))
	return true;

      if(chunks[i] < obj->getChunk(i))
	return false;
    }

    return false;
  }
  bool greaterThan(int obj){
    
    for(int i = 0; i < CHUNKCOUNT - 1; i++){
      
      if(chunks[i] > 0)
	return true;
    }
    if(chunks[CHUNKCOUNT - 1] > obj)
      return true;

    return false;
  }
  bool greaterThanEqual(Uhuge * obj){
    
    for(int i = 0; i < CHUNKCOUNT; i++){
      
      if(chunks[i] > obj->getChunk(i))
	return true;

      if(chunks[i] < obj->getChunk(i))
	return false;
    }

    return true;
  }
  bool greaterThanEqual(int obj){
    
    for(int i = 0; i < CHUNKCOUNT - 1; i++){
      
      if(chunks[i] > 0)
	return true;
    }
    if(chunks[CHUNKCOUNT - 1] >= obj)
      return true;

    return false;
  }
bool lessThan(Uhuge * obj){
    
    for(int i = 0; i < CHUNKCOUNT; i++){
      
      if(chunks[i] < obj->getChunk(i))
	return true;

      if(chunks[i] > obj->getChunk(i))
	return false;
    }

    return false;
  }
  bool lessThan(int obj){
    
    for(int i = 0; i < CHUNKCOUNT - 1; i++){
      
      if(chunks[i] > 0)
	return false;
    }
    if(chunks[CHUNKCOUNT - 1] < obj)
      return true;

    return false;
  }
  bool lessThanEqual(Uhuge * obj){
    
    for(int i = 0; i < CHUNKCOUNT; i++){
      
      if(chunks[i] < obj->getChunk(i))
	return true;

      if(chunks[i] > obj->getChunk(i))
	return false;
    }

    return true;
  }
  bool lessThanEqual(int obj){
    
    for(int i = 0; i < CHUNKCOUNT - 1; i++){
      
      if(chunks[i] > 0)
	return false;
    }
    if(chunks[CHUNKCOUNT - 1] <= obj)
      return true;

    return false;
  }
  Uhuge  shiftLeft(){
    Uhuge shifted;

    shifted.setChunk(0, CHUNKCOUNT - 1);

    for (int i = CHUNKCOUNT - 1; i > 0; i--){
      shifted.setChunk(i-1, chunks[i]);
    }

    return shifted;
  }
  Uhuge divide10()
  {
    Uhuge quotient(this);

    for (int i = CHUNKCOUNT-1; i > 0; i--)
      {
	quotient.setChunk(i, (quotient.getChunk(i) / 10) + (quotient.getChunk(i - 1) % 10 * RGROUP / 10));
      }
    quotient.setChunk(0, quotient.getChunk(0) / 10);
            
    return quotient;
  }
  int getChunk(int i) {
    return chunks[i];
  }
  void setChunk(int i, int value) {
    chunks[i] = value;
  }
  int getRGROUP() {
    return RGROUP;
  }
  void print() {

    for (int i = 0; i < CHUNKCOUNT; i++){
      cout << setfill('0') << setw(CHUNKSIZE) << chunks[i] << " ";
    }		
    
    cout << "\n";
  }
  static int parseInt(string val){
    int intVal = 0;

    for(int i = 0; i < val.length(); i++)
      intVal = intVal * 10 + val.at(i) - 48;

    return intVal;
  }
  bool divisible2(){
    return chunks[CHUNKCOUNT - 1] % 2 == 0;
  }
};
