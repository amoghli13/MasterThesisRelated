#include "KnapsackTMR.h"
using namespace std;

template <class M>
double error_inject_operators<M>::error_percent=0.001;

int main()
{

	error_inject_operators<int>::error_percent=0.001;

	Knapsack my_knapsack(50,50,10);
	my_knapsack.fill_matrix();
	//my_knapsack.print_info();
	
return 0;
}
