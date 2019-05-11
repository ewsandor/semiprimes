#include "primes.hpp"
#include "uhuge.hpp"
#include <cstdint>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
  int i;
  Uhuge tested;
  int testCount = 10000;
  Uhuge testVal((uint64_t)2);

//Uhuge TARGET("74037563479561712828046796097429573142593188889231289084936232638972765034028266276891996419625117843995894330502127585370118968098286733173273108930900552505116877063299072396380786710086096962537934650563796359");
//Uhuge TARGET("361");
Uhuge TARGET("1522605027922533360535618378132637429718068114961380688657908494580122963258952897654000350692006139");
  cout << "TARGET:\n";
  TARGET.printHex();
  cout << "Starting at:\n";
  testVal.printHex();

  for(;;)
  {
    for (i = 0; i < testCount; i++)
    {
        Uhuge t = testVal.add(i);
        if (TARGET.mod(&t).equals((uint64_t) 0))
        {
          cout << "Found!!!";
          testVal.printHex();
          return 0;
        }

        cout << '.' << flush; //garbage 
    }      
    cout << endl;
    tested = tested.add(testCount);
    tested.printHex();
    cout << " Numbers Tested\n\n";
    testVal = testVal.add(testCount);
  }

  return 0;
}
