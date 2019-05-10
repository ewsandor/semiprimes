#include "primes.hpp"
#include "uhuge.hpp"
#include <cstdint>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
  Uhuge tested;
  int testCount = 5000;
  bool testers[testCount];

//Uhuge TARGET("74037563479561712828046796097429573142593188889231289084936232638972765034028266276891996419625117843995894330502127585370118968098286733173273108930900552505116877063299072396380786710086096962537934650563796359");
//  Uhuge TARGET("15");
  Uhuge TARGET("1522605027922533360535618378132637429718068114961380688657908494580122963258952897654000350692006139");
  TARGET.print();
  Uhuge testVal(primes[NUM_PRIMES - 1]);
 
  for(;;){
    for(int i = 0; i < testCount; i++)
      testers[i] = false;
    
    for (int i = 0; i < NUM_PRIMES; i++){
      int added = 0;
	
      while (!testVal.add(added).mod(primes[i]).equals((uint64_t) 0))
	added++;

      while (added < testCount){
	testers[added] = true;
	  
	added += primes[i];
      }
    }
    for (int i = 0; i < testCount; i++){
      //cout << '.' << flush; //garbage 
      if (!testers[i]){
        Uhuge t = testVal.add(i);
    	if (TARGET.mod(&t).equals((uint64_t) 0))
    	  cout << "Found!!!";

    	//testVal.add(i).print(); //tested Value
        //cout << (double) i / testCount << endl; //percent complete
    	cout << '.' << flush; //garbage 
      }
    }      

    cout << endl;
    tested = tested.add(testCount);
    tested.print();
    cout << " Numbers Tested\n\n";
    testVal = testVal.add(testCount);
    }

  // for(int i = 0; i < 5000; i++){    
  //   Uhuge modded(TARGET.mod(&testVal));

  //   if(modded.equals(0)){
  //     testVal.print();
  //   }  
  //   testVal = testVal.add(1);
  //   testVal.print();
  // }  

  return 0;
}
