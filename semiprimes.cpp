#include "uhuge.hpp"
#include <cstdint>
#include <iostream>
#include <fstream>


using namespace std;

int * populatePrimes(int count){
  //int primes [count];
  int * primes = new int[count];

  ifstream primeFile("primes1.txt");
  string primeString = "";

  if(primeFile.is_open()){
    while(primeFile.good()){
      string t;
      getline(primeFile,t);
      primeString = primeString + " " + t;
    }

    primeFile.close();
  }

  for(int i = 0; i < count; i++){
    primes[i] = 0;

    while(primeString.at(0) == ' '){
      primeString = primeString.substr(1);
    }
    int loc = primeString.find(' ');
    primes[i] = Uhuge::parseInt(primeString.substr(0, loc));
    primeString = primeString.substr(loc);
    //cout << i + 1 << ": " << primes[i] << endl; //value
    cout << '#' << flush;
  }
  
  //cout << endl;

  return primes;
}

int main()
{
  Uhuge tested;
  int primeCount = 50;
  int testCount = 5000;
  int * primes= populatePrimes(primeCount);
  bool testers[testCount];

//Uhuge TARGET("74037563479561712828046796097429573142593188889231289084936232638972765034028266276891996419625117843995894330502127585370118968098286733173273108930900552505116877063299072396380786710086096962537934650563796359");
//  Uhuge TARGET("15");
  Uhuge TARGET("1522605027922533360535618378132637429718068114961380688657908494580122963258952897654000350692006139");
  Uhuge testVal(primes[primeCount - 1]);
  



  
 
  for(;;){
    for(int i = 0; i < testCount; i++)
      testers[i] = false;
    
    for (int i = 0; i < primeCount; i++){
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
