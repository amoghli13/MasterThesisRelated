#include "error_inject_operator.h"

using namespace std;

template <class M>
double error_inject_operators<M>::error_percent=0.01;
int main()
{

error_inject_operators<int> var1,var2;



var1=100;
var2=200;
var1=var1+100;
var1=var1+var2;
var2=var1+100;


return 0;
}
