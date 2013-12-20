
	 #include RobustIterativeSG.h 
#include "dissippation.h"

int main(int argc, char* argv[])
{
	char* filename;//[20];
	if(argc<3)
	{
		cout<<"\n\t ERROR: "<<" Minimum required parameters: "<<"\n\t\t 1. Output filename "<<"\n\t\t 2. Number of iterations "<<cout<<endl;
		exit(-1);
	}
	filename=argv[1];
	num_times=atoi(argv[2]);
	double A[num_rows*num_cols];
	double B[num_rows*num_cols];

	double* heat_mat1;
	double* heat_mat2;

	initialize_matrix(A,'r');
	initialize_matrix(B,'r');

	cout<<"\n\t Going to update grid for "<<num_times<<" number of times \n";

	for (int t = 0; t < num_times; t++)
	{

		if(t%2)
		{
			heat_mat1=A;
			heat_mat2=B;
		}
		else
		{
			heat_mat1=B;
			heat_mat2=A;
		}
		     
		//#pragma struct_grid start
		//#pragma struct_grid data_layout 1. 1 dimensional, 2. 2 dimensional 3. 3-dimensional
		
		for (int row_idx = dependency_length ; row_idx < num_rows - dependency_length ; row_idx++) 
		{
			for (int col_idx = dependency_length ; col_idx < num_cols-dependency_length ; col_idx++) 
			{
		  		 int index= row_idx*num_cols + col_idx;
				heat_mat1[index] =   0.125 * ( heat_mat2[index+num_cols]  - 2.0 * heat_mat2[index] + heat_mat2[index-num_cols] )+ 0.125 * (heat_mat2[index+1] - 2.0 * heat_mat2[index] + heat_mat2[index-1]) + heat_mat2[index];			}
		}
				grid_analyze_per_quadrant(heat_mat1,heat_mat2,100,150,1,t);		//#pragma struct_grid end		
	}

	save_matrix(heat_mat1,num_rows,num_cols,"matA",filename);
return 0;
}
