#include<iostream>
#include<istream>
#include<ostream>
#include<fstream>

#include<cmath>
#include<cstdlib>

#include<string>
#include<string.h>

#include<ctime>
#include <sys/time.h>
#include<vector>

using namespace std;

const int dependency_length=1;
const int num_rows=32+2*dependency_length;
const int num_cols=32+2*dependency_length; // For convenience of using same formula for each pixel.

int num_times=4;
int verbose=0;// The command line option specifies which part of the computations are printed to standard io/file.

ofstream save_mat_file;


void initialize_matrix(double* image_pixels,char rand_option)
{

	int index=0;

	for(int row_idx=dependency_length;row_idx<num_rows-dependency_length;row_idx++)
	{
		for(int col_idx=0;col_idx < dependency_length;col_idx++)
		{
			index=row_idx*num_cols+col_idx;
			image_pixels[index]=0.0f;
		}
	}

	//timeval t1;
	//gettimeofday(&t1, NULL); 
	//srand(t1.tv_usec * t1.tv_sec);

	for(int row_idx=0;row_idx<dependency_length;row_idx++)
	{
		for(int col_idx=0;col_idx < num_cols;col_idx++)
		{
			index=row_idx*num_cols+col_idx;
			image_pixels[index]=0.0f;
		}
	}

	for(int row_idx=num_rows-dependency_length;row_idx<num_rows;row_idx++)
	{
		for(int col_idx=0;col_idx < num_cols;col_idx++)
		{
			index=row_idx*num_cols+col_idx;
			image_pixels[index]=0.0f;
		}
	}

	switch(rand_option)
	{

		case 'r':
			for(int row_idx=dependency_length;row_idx<num_rows-dependency_length;row_idx++)
			{
				int row_offset=row_idx*num_cols;
				for(int col_idx=dependency_length;col_idx<num_cols-dependency_length;col_idx++)
				{
					index=row_offset+col_idx;
					image_pixels[index]= ( 0.6 * ( (double) rand() / RAND_MAX ) );
				} 
			}
			break;

		case 'z':
			for(int row_idx=dependency_length;row_idx<num_rows-dependency_length;row_idx++)
			{
				int row_offset=row_idx*num_cols;
				for(int col_idx=dependency_length;col_idx<num_cols-dependency_length;col_idx++)
				{
					index=row_offset+col_idx;
					image_pixels[index]=0.0f;
				} 
			}
			break;

	}

	if(verbose==2)
	{
		cout<<endl;
		for(int row_idx=0;row_idx<num_rows;row_idx++)
		{

			for(int col_idx=0;col_idx<num_cols;col_idx++)
			{
				index=row_idx*num_cols+col_idx;
				cout<<" "<<image_pixels[index];
			}
			cout<<endl;
		}
	}
}


void save_matrix(double *mat,int rows_mat,int cols_mat,char* mat_name,char* filename)
{
	save_mat_file.open (filename,ios::out | ios::app);
	cout<<"\n\t About to write matrix "<<mat_name<<" with rows "<<rows_mat<<" with columns "<<cols_mat<<" to file "<<filename<<" \n";
	save_mat_file<<"\n\n\t ------------------------------------------------------------------------------------------------------------ \n";
	save_mat_file<<"\n\t Matrix: "<<mat_name<<"\n\n";
	int index=0;
	if(cols_mat > 1)	
	{
		for(int row_idx=0;row_idx<rows_mat;row_idx++)
		{
		int row_offset=row_idx*cols_mat;
		save_mat_file<<"\n\t Row: "<<row_idx<<endl;
		//	 cout<<"\n\t Row: "<<row_idx<<endl;

			for(int col_idx=0;col_idx<cols_mat;col_idx++)
			{
				if(col_idx%20==0)	
				save_mat_file<<endl;
				index=row_offset+col_idx;
				//save_mat_file<<"\n\t "<<mat[index];
				save_mat_file<<"\t col: "<<col_idx<<" , "<<mat[index];
			}

		}
	}
	else if(cols_mat==1)
	{
		save_mat_file<<"\n\t This is a row-matrix";
		for(int row_idx=0;row_idx<rows_mat;row_idx++)
		{
			save_mat_file<<"\n\t col: "<<row_idx<<" , "<<mat[row_idx];
		}	
	}
	save_mat_file.close();

}

