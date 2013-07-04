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

int tile_size=4;

const int max_check_times=2;
const int dependency_length=1;
int num_rows=32+2*dependency_length;
int num_cols=32+2*dependency_length; // For convenience of using same formula for each pixel.
int total_iters=1;
ofstream error_reiter_gridvalues,error_reiter_log;
////////


const double xmin=-1;
const double xmax=1;
const double ymin=xmin,ymax=xmax;
//const double ymin=-2,ymax=2;
double xmaxby2=xmax/2;
double ymaxby2=ymax/2;
double Len_x=2.0;
double Len_y=2.0;

// Compute useful constants
const double pi=3.1415926535897932384626433832795;

double dx=Len_x/(num_cols);
double dy=(Len_y)/(num_rows);
double dt;

double dxxinv=1/(dx*dx);
double dyyinv=1/(dy*dy);
double dcent=1/(2*(dxxinv+dyyinv)); 
const double c=1/pi;
double c1; //=0.05; //( c*( 0.01 )/(32) );
double c2; //=0.05;//( dt*c/dy ) ;//* ( dt*c/dy );

///////

//%%%%%%%%% Acoustics constants.
double acoustics_wave_speed=340;
double rho_zero=1;
int freq=1000;
double omega=(2*pi*freq);
double beta=0.5;

double aux1,aux2;
double aux11,aux12;
double aij_multiple;
///////////
double error_maxcofactor=1.028, error_mincofactor=0.975;
double error_maxcofactor_default=1.028, error_mincofactor_default=0.975;
double error_maxcofactor_quad=1.035, error_mincofactor_quad=0.97;
double error_maxcofactor_quad_default=1.035, error_mincofactor_quad_default=0.97;
////////////
int max_iter_repeat=4;

int max_quad_check=2;
int max_quad_rollover=1;
const int max_tile_check=2;

double tile_error_cofactor=1.0;

double error_cofactor_quad=1.05;
double error_cofactor=1.05;

double error_cofactor_default=1.05;
double error_cofactor_quad_default=1.05;


int verbose=0;// The command line option specifies which part of the computations are printed to standard io/file.

ofstream save_mat_file;

double f(int col,int row);
double g(int col,int row);
void initialize_matrix(double* mat,char rand_option)
{

	int index=0;

	for(int row_idx=dependency_length;row_idx<num_rows-dependency_length;row_idx++)
	{
		for(int col_idx=0;col_idx < dependency_length;col_idx++)
		{
			index=row_idx*num_cols+col_idx;
			mat[index]=0.0f;
		}
		for(int col_idx=num_cols-dependency_length;col_idx < num_cols;col_idx++)
		{
			index=row_idx*num_cols+col_idx;
			mat[index]=0.0f;
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
			mat[index]=0.0f;
		}
	}
 
	for(int row_idx=num_rows-dependency_length;row_idx<num_rows;row_idx++)
	{
 		for(int col_idx=0;col_idx < num_cols;col_idx++)
		{
			index=row_idx*num_cols+col_idx;
			mat[index]=0.0f;
		}
	}
 
	switch(rand_option)
	{
	

		case 'r':
			for(int row_idx=dependency_length;row_idx<(num_rows-dependency_length);row_idx++)
			{
				int row_offset=row_idx*num_cols;
				for(int col_idx=dependency_length;col_idx<(num_cols-dependency_length);col_idx++)
				{
					index=row_offset+col_idx;
					mat[index]= f(col_idx,row_idx);//( 0.6 * ( (double) rand() / RAND_MAX ) );
 
				} 
			}
			break;

		case 'z':
			for(int row_idx=dependency_length;row_idx<=(num_rows-dependency_length);row_idx++)
			{
				int row_offset=row_idx*num_cols;
 				for(int col_idx=dependency_length;col_idx<=(num_cols-dependency_length);col_idx++)
				{
					index=row_offset+col_idx;
					mat[index]=0.0f;
				} 
			}
		break;

	}

	if(verbose==-2)
	{
		cout<<endl;
		for(int row_idx=0;row_idx<num_rows;row_idx++)
		{

			for(int col_idx=0;col_idx<num_cols;col_idx++)
			{
				index=row_idx*num_cols+col_idx;
				cout<<" "<<mat[index];
			}
			cout<<endl;
		}
	}
}

double f(int col,int row)
{
	double x=xmin+row*dx;
	double y=ymin+col*dy;

	return ( (x*(x-1) ) * (y* (y-1) ) );
}

double g(int col,int row)
{
	//double x=xmin+row*dx;
	//double y=ymin+col*dy;

	return ( 0.0f );
}


// initialize_mat_wave initializes two matrices!! 
void initialize_mat_wave( double* mat,double* mat1)
{


	char file1[50];
	char file2[50];
	cout<<"\n\t Num-rows: "<<num_rows<<" num-cols: "<<num_cols;
	sprintf(file1,"%s_%dX%d.mat","mat0",num_rows,num_cols);
	sprintf(file2,"%s_%dX%d.mat","mat1",num_rows,num_cols);

	ofstream file1_handle,file2_handle;
	file1_handle.open(file1,ios::out);
	file2_handle.open(file2,ios::out);


	for(int idx=0;idx<(num_rows*num_cols);idx++)
	{
		mat[idx]=0.0f;
		mat1[idx]=0.0f;	
	}

	for(int idx=0;idx<num_cols;idx++)
	{
		file1_handle<<"\t"<<mat[idx]<<endl;
		file2_handle<<"\t"<<mat1[idx]<<endl;	
	}
	
	for(int row_idx=dependency_length;row_idx<(num_rows-dependency_length);row_idx++)
	{
		int row_offset=row_idx*num_cols;
		int index=row_offset;

		file1_handle<<"\t"<<mat[index]<<endl;
		file2_handle<<"\t"<<mat1[index]<<endl;	
	
		for(int col_idx=dependency_length;col_idx<(num_cols-dependency_length);col_idx++)
		{
			index=row_offset+col_idx;
			mat[index]=f(col_idx,row_idx) - (dt*g(col_idx,row_idx)  );
			file1_handle<<"\t "<<mat[index]<<endl;		
			mat1[index]=f(col_idx,row_idx) ;
			file2_handle<<"\t "<<mat1[index]<<endl;
		}

		index=row_offset+num_cols;
		file1_handle<<"\t"<<mat[index]<<endl;
		file2_handle<<"\t"<<mat1[index]<<endl;	

	}
	
	for(int idx=0;idx<num_cols;idx++)
	{
		int index=(num_rows-dependency_length)*num_cols + idx;
		file1_handle<<"\t"<<mat[index]<<endl;
		file2_handle<<"\t"<<mat1[index]<<endl;	
	}

	file2_handle.close();
	file1_handle.close();
	
	cout<<"\n\t File1: "<<file1<<" File2: "<<file2<<" have been written! \n";
	//exit(-1);
}

void read_matrix(double* image_pixels,char* read_image)
{
ifstream mat_file;
cout<<"\n\t Reading from file "<<read_image<<endl;
//if(mat_file.is_open() )
//{
 	mat_file.open( read_image, ios::in );
	int mat_size=num_rows*num_cols;

	for(int idx=0;idx<mat_size;idx++)
	{
		mat_file>>image_pixels[idx];
		//cout<<"\n\t Index: "<<idx<<" image_pixels[index] "<<image_pixels[idx];
	}

//}
mat_file.close();
//exit(-1);
}

void save_col(double *mat,int rows_mat,int cols_mat,string mat_name,char* filename,int curr_iter,int min_idx) 
{
	save_mat_file.open (filename,ios::out | ios::app);

	//save_mat_file<<"\n\n\t ------------------------------------------------------------------------------------------------------------ \n";
	//save_mat_file<<"\n\t Matrix: "<<mat_name<<" iters: "<<curr_iter<<"\n\n";
	int index=0;
 	cout<<"\n\t About to write matrix "<<mat_name<<" with rows "<<rows_mat<<" with columns "<<cols_mat<<" to file "<<filename<<" with min-idx: "<<min_idx<<" \n";
	//save_mat_file<<"\n\t This is a row-matrix";
	for(int row_idx=0;row_idx<rows_mat;row_idx++)
	{
		// save_mat_file<<"\n\t col: "<<(row_idx+1)<<" , "<<mat[row_idx];
		if(row_idx>min_idx) 
		save_mat_file<<"\n\t "<<mat[row_idx];
	}	
	save_mat_file.close();

}

void save_initial_matrix(double *mat,int rows_mat,int cols_mat,string mat_name,char* filename) 
{
	save_mat_file.open (filename,ios::out | ios::app);

	//save_mat_file<<"\n\n\t ------------------------------------------------------------------------------------------------------------ \n";
	//save_mat_file<<"\n\t Matrix: "<<mat_name<<"\n\n";
	cout<<"\n\t Matrix: "<<mat_name<<"\n\n";	
	int index=0;
	if(cols_mat > 1)	
	{
	cout<<"\n\t About to write matrix "<<mat_name<<" with rows "<<rows_mat<<" with columns "<<cols_mat<<" to file "<<filename<<" \n";
		for(int row_idx=0;row_idx<rows_mat;row_idx++)
		{
			int row_offset=row_idx*cols_mat;
			//save_mat_file<<"\n\t Row: "<<row_idx<<endl;
//			cout<<"\n\t Row: "<<row_idx<<endl;

			for(int col_idx=0;col_idx<cols_mat;col_idx++)
			{
 
				index=row_offset+col_idx;
				save_mat_file<<"\n\t "<<mat[index];
				//save_mat_file<<"\t col: "<<col_idx<<" , "<<mat[index];
			}

		}
	}
	else if(cols_mat==1)
	{
		cout<<"\n\t About to write matrix "<<mat_name<<" with rows "<<rows_mat<<" with columns "<<cols_mat<<" to file "<<filename<<" \n";
		save_mat_file<<"\n\t This is a row-matrix";
		for(int row_idx=0;row_idx<rows_mat;row_idx++)
		{
			//save_mat_file<<"\n\t col: "<<(row_idx+1)<<" , "<<mat[row_idx];
			save_mat_file<<"\n\t  "<<mat[row_idx];
			//cout<<"\n\t col: "<<(row_idx+1)<<" , "<<mat[row_idx];
		}	
	}
	save_mat_file.close();

}

void save_matrix(double *mat,int rows_mat,int cols_mat,string mat_name,char* filename,int curr_iter) 
{
	save_mat_file.open (filename,ios::out | ios::app);

	save_mat_file<<"\n\n\t ------------------------------------------------------------------------------------------------------------ \n";
	save_mat_file<<"\n\t Matrix: "<<mat_name<<" iters: "<<curr_iter<<"\n\n";
	int index=0;
	if(cols_mat > 1)	
	{
	cout<<"\n\t About to write matrix "<<mat_name<<" with rows "<<rows_mat<<" with columns "<<cols_mat<<" to file "<<filename<<" \n";
		for(int row_idx=0;row_idx<rows_mat;row_idx++)
		{
			int row_offset=row_idx*cols_mat;
			save_mat_file<<"\n\t Row: "<<row_idx<<endl;
//			cout<<"\n\t Row: "<<row_idx<<endl;

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
		cout<<"\n\t About to write matrix "<<mat_name<<" with rows "<<rows_mat<<" with columns "<<cols_mat<<" to file "<<filename;//<<" with min-idx: "<<min_idx<<" \n";
		//save_mat_file<<"\n\t This is a row-matrix";
		for(int row_idx=0;row_idx<rows_mat;row_idx++)
		{
			 save_mat_file<<"\n\t col: "<<(row_idx+1)<<" , "<<mat[row_idx];
 		}	
	}
	save_mat_file.close();

}

	int within_quad_rows0=2;
	int within_quad_cols0=2;

	const int num_quad_level1=3;

	int within_quad_rows1=num_quad_level1;// *within_quad_rows0;
	int within_quad_cols1=num_quad_level1; //*within_quad_cols0;
//int quad_within_quad=

	double* curr_grid_quad_error;
	double* prev_grid_quad_error;

	int num_quad_rows=within_quad_rows1*within_quad_rows0;
	int num_quad_cols=within_quad_cols1*within_quad_cols0;

	int grid_num_quad=within_quad_rows1*within_quad_cols1; //*within_quad_cols0;

	int length_quad_rows=(int) ( (num_rows-2*dependency_length)/num_quad_rows );
	int length_quad_cols=(int) ( (num_cols-2*dependency_length)/num_quad_cols );

	int num_quad=num_quad_rows*num_quad_cols;

	int num_tiles_within_row=( ( (length_quad_rows-1) / tile_size ) +1);
	int num_tiles_within_col=( ( (length_quad_cols-1) / tile_size ) +1);

	double* curr_quad_error;
	double* prev_quad_error;
	//vector<double> prev_quad_error(num_quad);

	int num_req_tiles=num_quad*num_tiles_within_row*num_tiles_within_col;
	int num_tiles_within_quad=num_tiles_within_row*num_tiles_within_col;

	double* curr_tile_error;//(num_req_tiles,0.0f);
	double* prev_tile_error;//vector<double> prev_tile_error(num_req_tiles,0.0f);


double grid_analyze_per_quadrant(double* wave_mat1,double* wave_mat2,double* wave_mat3,int t)
{
 
	for(int idx=0;idx<grid_num_quad;idx++)
	{
	 	curr_grid_quad_error[idx]=0.0f;
	}

 	int quad=0;

	int quad_idx_rows=-1;
	int quad_idx_cols=0;

	int within_quad_idx_rows=0;
	int within_quad_idx_cols=0;

	int row_offset=0;int col_offset=0;
	double error=0.0f;

	int align_quad_rows=-1;
	int align_quad_cols=-1;
	int quad_alias=-1;
	int num_tiles=0;
  
 	for(int quad_idx=0;quad_idx<num_quad;quad_idx++)
	{

		if(quad_idx%num_quad_cols)
		{
				quad_idx_cols++;
			if(quad_idx%within_quad_cols0)
			{
				within_quad_idx_cols++;

			}
			else
				within_quad_idx_cols=0;
		 
		}
		else
		{

			quad_idx_rows++;
			if(quad_idx_rows%within_quad_rows0)
				within_quad_idx_rows++;
			else
				within_quad_idx_rows=0;
			quad_idx_cols=0;
			within_quad_idx_cols=0;
		}


		row_offset=quad_idx_rows*length_quad_rows+dependency_length;
		col_offset=quad_idx_cols*length_quad_cols+dependency_length;

		double error_quad=0.0f;
		double z=0.0f;
		double error_before_tile;

		for(int row_idx=row_offset;(row_idx<(row_offset+length_quad_rows));row_idx+=tile_size)
		{

			for(int col_idx=col_offset;col_idx<(col_offset+length_quad_cols);col_idx+=tile_size)
			{
				int index;
				error_before_tile=error_quad;
				double error_tile=0.0f;
				for(int row_within_tile=row_idx; (row_within_tile<(row_idx+tile_size)) && ( row_within_tile<= (num_rows-dependency_length) ) && (row_within_tile<(row_offset+length_quad_rows))  ;row_within_tile++ )
				{
					int row_offset=row_within_tile*num_cols ;
					for(int col_within_tile=col_idx; (col_within_tile<(col_idx+tile_size)) && ( col_within_tile<= (num_cols-dependency_length) ) && (col_within_tile<(col_offset+length_quad_cols))  ;col_within_tile++ )
					{
						index=row_offset+col_within_tile;
						double temp=wave_mat2[index];
						wave_mat1[index] =   ( (  aij_multiple * wave_mat2[index]  ) + aux1 * (  wave_mat2[index-1] + wave_mat2[index+1] ) + aux2 * ( wave_mat2[index-num_cols] + wave_mat2[index+num_cols]  ) - wave_mat3[index]  );					   
						error_tile+=abs( abs( temp)-abs(wave_mat1[index] )) ; 
						//error_tile+=abs( abs(wave_mat1[index] )) ; 						
	 					 if( (verbose==1))
							cout<<"\n\t\t Index: "<<index<<" Row: "<<row_within_tile<<" Col: "<<col_within_tile<<" wave_mat1[index] "<< wave_mat1[index]<<" error_tile: "<<error_tile<<" error: "<<error<<" diff: "<<(abs( wave_mat1[index] )-abs(temp) ) <<" wave_mat2[index] "<< wave_mat2[index]<<" wave_mat3[index] "<< wave_mat3[index]<<" temp "<<temp;
					}

				}
				error_quad+=error_tile;
				
				
				curr_tile_error[num_tiles]=error_tile;
				if(verbose==2)
					cout<<"\n --- Tile-num: "<<num_tiles<<" Col-start: "<<col_idx<<" Row-start: "<<row_idx<<" curr_tile_error[num_tiles]: "<<curr_tile_error[num_tiles]<<"\t Quad_row: "<<quad_idx_rows<<"\t Quad_col: "<<quad_idx_cols;
				num_tiles++;		

			}

		}
		 	 error+=abs(error_quad);

		curr_quad_error[quad_idx]=error_quad;

  		if(quad_idx%within_quad_cols0==0)
		{

			 align_quad_cols++;
			if( (align_quad_cols%within_quad_cols1==0) && (align_quad_cols!=0) )
			{
				align_quad_cols-=within_quad_cols1;
			}

			if(quad_idx%(within_quad_cols0*num_quad_cols)==0) // Not too sure of 2nd operator!! 
			{
				align_quad_rows++;
				align_quad_cols=0;
			}
		
		}
		quad_alias=align_quad_rows*within_quad_cols1+ align_quad_cols;
		curr_grid_quad_error[quad_alias]+=abs(error_quad);

		// cout<<"\n\t Quad-idx: "<<quad_idx<<" quad_idx_rows "<<quad_idx_rows<<" quad_idx_cols "<<quad_idx_cols; //<<" within_quad_idx_rows "<<within_quad_idx_rows<<" within_quad_idx_cols "<<within_quad_idx_cols;
		// cout<<" Quad_alias: "<<quad_alias<<" Align_quad_cols: "<<align_quad_cols<<" Align_quad_rows: "<<align_quad_rows<<" curr_grid_quad_error[quad_alias] "<<curr_grid_quad_error[quad_alias];

	}
 
	//cout<<"\n\t ---  Num_quad_rows: "<<num_quad_rows<<"\t Num_quad_cols: "<<num_quad_cols; cout<<"\n\t ---  Length_quad_rows: "<<length_quad_rows<<"\t Length_quad_cols: "<<length_quad_cols;
 

// cout<<"\n\t within_quad_rows0: "<<within_quad_rows0<<" within_quad_cols0: "<<within_quad_cols0; cout<<"\n\t grid_num_quad: "<<grid_num_quad<<" length_quad_rows: "<<length_quad_rows<<" length_quad_cols: "<<length_quad_cols;
// cout<<"\n\t num_tiles_within_row: "<<num_tiles_within_row<<" num_tiles_within_col: "<<num_tiles_within_col; cout<<"\n\t num_req_tiles: "<<num_req_tiles<<" num_tiles_within_quad: "<<num_tiles_within_quad;cout<<endl;

// for(int idx=0;idx<grid_num_quad;idx++)	cout<<"\n\t --- Quad-idx: "<<idx<<" curr_grid_quad_error[quad_idx]: "<<curr_grid_quad_error[idx];


	double error_dx_dy=((error)/(num_rows*num_cols)); //*dx*dy);
	cout<<"\n\t Iter: "<<t<<" Error-after adding each quad: "<<(error_dx_dy)<<"\t num_tiles: "<<num_tiles<<" num_req_tiles: "<<num_req_tiles<<endl;

 	//exit(-1);
	return error_dx_dy;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double scan_correct(double* wave_mat1,double* wave_mat2,double* wave_mat3,double diff_curr_iter,int t)
{

int count_quad_repeat=0;
int quad_rollover_count=0;
int quad_checked=0;
cout<<"\n\t Iter: "<<t<<" Grid_num_quad "<<grid_num_quad<<endl;
double b4_check;
for(int quad_idx=0,num_times_quad_checked=0;quad_idx<grid_num_quad;quad_idx++)
{
//verbose=5;

 if(verbose>=3)
 	 error_reiter_gridvalues<<"\n\t ** Iter: "<<t<<" Quad-idx: "<<quad_idx<<"\t Error current quad: "<<curr_grid_quad_error[quad_idx]<<"\t prev-quad: "<<prev_grid_quad_error[quad_idx]<<" num_times_quad_checked: "<<num_times_quad_checked;;
	//cout<<"\n\t ** Iter: "<<t<<" Quad-idx: "<<quad_idx<<"\t Error current quad: "<<curr_grid_quad_error[quad_idx]<<"\t prev-quad: "<<prev_grid_quad_error[quad_idx]<<" num_times_quad_checked: "<<num_times_quad_checked;; 	 
	// if ( (  ( abs(curr_grid_quad_error[quad_idx]) < abs(prev_grid_quad_error[quad_idx] ) ) || ( curr_grid_quad_error[quad_idx] < 0 ) || 1) && ( num_times_quad_checked < max_quad_check ) )

	double ratio_grid_quad= (  abs(curr_grid_quad_error[quad_idx])  / abs(prev_grid_quad_error[quad_idx] ) ) ;
//	if( ( ( abs(curr_grid_quad_error[quad_idx]) > (error_cofactor_quad*abs(prev_grid_quad_error[quad_idx] ) ) )  || (!quad_checked)) && (quad_rollover_count < max_quad_rollover) )

if(!quad_checked) b4_check=abs(curr_grid_quad_error[quad_idx]) ;
	double ratio_quad_during_check= ( abs(curr_grid_quad_error[quad_idx])/b4_check);
	if( ( (  ( (abs(ratio_grid_quad) > error_mincofactor_quad)  && (abs(ratio_grid_quad) < error_maxcofactor_quad) ) &&  ( (abs(ratio_quad_during_check < 0.999 ) ) || ( ratio_quad_during_check > 1.001) )   ) || (!quad_checked) ) && (quad_rollover_count < max_quad_rollover) )
	{

		count_quad_repeat++;
		if(  count_quad_repeat == max_iter_repeat )
		{
			error_mincofactor_quad-=0.01;
			error_maxcofactor_quad+=0.01;			
			count_quad_repeat=0;
	 		quad_rollover_count++;
 		}						 


		 		int quad_row= (quad_idx/within_quad_rows1);
		int quad_col=(quad_idx%within_quad_cols1);//quad_idx%within_quad_cols0;

		int quad_error_idx=grid_num_quad*quad_idx;

		int within_quad_row=quad_row*within_quad_rows0;
		int within_quad_col=quad_col*within_quad_cols0;

		curr_grid_quad_error[quad_idx]=0;
		int subquad_checked=0;
		for(int quad_row_idx=within_quad_row; quad_row_idx<(within_quad_row+within_quad_rows0); quad_row_idx++)
		{
			int quad_error_idx_offset=quad_row_idx*num_quad_cols;
			int quad_error_idx;
			int start_check_tile=0;
			for(int quad_col_idx=within_quad_col; quad_col_idx<(within_quad_col+within_quad_cols0); quad_col_idx++)
			{
				quad_error_idx= quad_error_idx_offset+ quad_col_idx;
	 
 				if(verbose>=4)
 				error_reiter_gridvalues<<"\n\t\t Iter: "<<t<<" curr_quad0: "<<curr_quad_error[quad_error_idx]<<" prev_quad0: "<<prev_quad_error[quad_error_idx]<<" subquad_checked "<<subquad_checked;
	 				double ratio_quad= abs(curr_quad_error[quad_error_idx]) / abs(prev_quad_error[quad_error_idx] );
	 				//if( ( ( ) || (!subquad_checked) )  || (curr_quad_error[quad_error_idx] < 0) )
	 				if (  (  ( (abs(ratio_grid_quad) > error_mincofactor_quad)  && (abs(ratio_grid_quad) < error_maxcofactor_quad) ) || (!subquad_checked) ) || (curr_quad_error[quad_error_idx] < 0 ) )
					{
	 					start_check_tile= quad_error_idx * num_tiles_within_quad;
						int tile_num_times_checked=0;
					 			// cout<<"\n\t\t *********** Quad_error_idx "<<quad_error_idx<<" Quad-idx-row: "<<within_quad_row<<" Quad-idx-col: "<<within_quad_col;
						for(int check_tile_idx=start_check_tile;check_tile_idx<(start_check_tile+num_tiles_within_quad);check_tile_idx++)
						{
 
						if(verbose>=5)
							 error_reiter_gridvalues<<"\n\t\t\t Iter: "<<t<<"\t tile-idx: "<<check_tile_idx<<" curr_tile_error[check_tile_idx]: "<<curr_tile_error[check_tile_idx]<<" prev_tile_error[check_tile_idx]: "<<prev_tile_error[check_tile_idx];
 	
									double tile_ratio=abs(curr_tile_error[check_tile_idx]) / abs(prev_tile_error[check_tile_idx] );
									//tile_diff =  abs(curr_tile_error[check_tile_idx]) - abs(prev_tile_error[check_tile_idx] ) ; 
									//if( ( ( tile_diff > tile_error_cofactor*abs(prev_tile_error[check_tile_idx] ) ) || ( curr_tile_error[check_tile_idx] < 0 ) )&& (tile_num_times_checked< max_tile_check) )
									if( (  (  ( tile_ratio > error_mincofactor_quad) && ( tile_ratio < error_maxcofactor_quad) ) || ( curr_tile_error[check_tile_idx] < 0 ) )&& (tile_num_times_checked< max_tile_check) )									
									// if(tile_num_times_checked< max_tile_check) 
	 								{
	  									int check_tile_idx_start_row=(quad_row_idx*num_tiles_within_row*tile_size)+( ( (check_tile_idx-start_check_tile)/num_tiles_within_row) *tile_size)+dependency_length;
										int check_tile_idx_start_col=(quad_col_idx*num_tiles_within_col*tile_size)+( ( (check_tile_idx-start_check_tile)%num_tiles_within_col) *tile_size)+dependency_length;
										int num_updates=0;
										// cout<<"\n\t\t\t Check-tile idx: "<<check_tile_idx<<" Row: "<<check_tile_idx_start_row<<" Col: "<<check_tile_idx_start_col;
										double error_quad=0.0f;
	 									for(int row_idx=check_tile_idx_start_row; ( row_idx< ( check_tile_idx_start_row+tile_size ) && (row_idx<=(num_rows-dependency_length)) );row_idx++ )
										{
											int row_offset=row_idx*num_cols;
											int index=0;
											for(int col_idx=check_tile_idx_start_col; ( col_idx< ( check_tile_idx_start_col+tile_size ) && (col_idx<=(num_cols-dependency_length)) );col_idx++ )
											{
	 											index=row_offset+col_idx;
												double temp=wave_mat2[index]; // Should make this wave_mat1[index]!!
												wave_mat1[index] =   ( (  aij_multiple * wave_mat2[index]  ) - wave_mat3[index] + aux1 * (  wave_mat2[index-1] + wave_mat2[index+1] ) + aux2 * ( wave_mat2[index-num_cols] + wave_mat2[index+num_cols]  ) );					   
												double result=abs( abs(temp)-abs(wave_mat1[index] ) );
												 error_quad+=result; //- wave_mat1[index-1]  );
 
													if(verbose>=6)
														 error_reiter_gridvalues<<"\n\t\t\t Updating index: "<<index<<" row: "<<row_idx<<" col: "<<col_idx<<"\t wave_mat1[index]: "<<wave_mat1[index]<<" wave_mat2[index] "<<wave_mat2[index]<<" temp "<<temp<<" result "<<result;
 												

											}

										}
 
										curr_tile_error[check_tile_idx] = abs(error_quad); 
	 									tile_num_times_checked++;
 										if(verbose>=5)
 											 error_reiter_gridvalues<<"\n\t\t\t --- Iter: "<<t<<"\t tile-idx: "<<check_tile_idx<<" curr_tile_error[check_tile_idx]: "<<curr_tile_error[check_tile_idx]<<" prev_tile_error[check_tile_idx]: "<<prev_tile_error[check_tile_idx]<<std::flush;
										check_tile_idx--;
	 		 							}
									else
									{
 	 									tile_num_times_checked=0;
										if(verbose>=5) 
											 error_reiter_gridvalues<<"\n\t\t\t +++ Iter: "<<t<<"\t tile-idx: "<<check_tile_idx<<" curr_tile_error[check_tile_idx]: "<<curr_tile_error[check_tile_idx]<<" prev_tile_error[check_tile_idx]: "<<prev_tile_error[check_tile_idx]<<std::flush;
 									}
								}
 
	 							//curr_grid_quad_error[quad_idx]+=curr_quad_error[quad_error_idx] ;
								curr_quad_error[quad_error_idx]=0;
								double diff_tile=0.0f;
								for(int check_tile_idx=start_check_tile;check_tile_idx<(start_check_tile+num_tiles_within_quad);check_tile_idx++)
								{
									curr_quad_error[quad_error_idx]+=abs(curr_tile_error[check_tile_idx]);
								}
								
								for(int check_tile_idx=start_check_tile;check_tile_idx<(start_check_tile+num_tiles_within_quad);check_tile_idx++)
								{	
									diff_tile= curr_tile_error[check_tile_idx]  - prev_tile_error[check_tile_idx];
									if( ( abs(diff_tile) > abs(10*prev_tile_error[check_tile_idx] ) ) || ( curr_tile_error[check_tile_idx] < 0 ) ) 
									{
										curr_quad_error[quad_error_idx]+=abs(curr_tile_error[check_tile_idx]);
										error_reiter_gridvalues<<"\n\t Iter: "<<t<<" quad-idx: "<<quad_idx<<" quad_error_idx "<<quad_error_idx<<" tile-idx: "<<check_tile_idx<<" curr: "<<curr_tile_error[check_tile_idx]<<" prev "<<prev_tile_error[check_tile_idx]<<" diff_tile "<<diff_tile;
										error_reiter_gridvalues<<" curr_quad_error[quad_error_idx] "<<curr_quad_error[quad_error_idx] <<" prev_quad_error[quad_error_idx] "<<prev_quad_error[quad_error_idx] ;											
										//cout<<" curr_quad_error[quad_error_idx] "<<curr_quad_error[quad_error_idx] <<" prev_quad_error[quad_error_idx] "<<prev_quad_error[quad_error_idx] ;
										//cout<<"\n\t Iter: "<<t<<" quad-idx: "<<quad_idx<<" quad_error_idx "<<quad_error_idx<<" tile-idx: "<<check_tile_idx<<" curr: "<<curr_tile_error[check_tile_idx]<<" prev "<<prev_tile_error[check_tile_idx]<<" diff_tile "<<diff_tile;											
										quad_col_idx--;
										subquad_checked=1;
										check_tile_idx+=num_tiles_within_quad;
									}
									
								}
								 if(subquad_checked)
 									subquad_checked=0;
 								else
 								{
 									subquad_checked=1;
 									quad_col_idx--;	
 								}
 								if(verbose>=4)
 									error_reiter_gridvalues<<"\n\t\t -- Iter: "<<t<<" curr_quad0: "<<curr_quad_error[quad_error_idx]<<" prev_quad0: "<<prev_quad_error[quad_error_idx]<<" subquad_checked "<<subquad_checked;
 							}
							else
							{
								subquad_checked=0;
								if(verbose>=4)
									 error_reiter_gridvalues<<"\n\t\t ++ Iter: "<<t<<" curr_quad0: "<<curr_quad_error[quad_error_idx]<<" prev_quad0: "<<prev_quad_error[quad_error_idx]<<" subquad_checked "<<subquad_checked;
 	 						}


	 			}	
							 
			}
			curr_grid_quad_error[quad_idx]=0.0f;
	 		for(int quad_row_idx=within_quad_row; quad_row_idx<(within_quad_row+within_quad_rows0); quad_row_idx++)
			{
				int quad_error_idx_offset=quad_row_idx*num_quad_cols;
				int quad_error_idx;
				int start_check_tile=0;
				for(int quad_col_idx=within_quad_col; quad_col_idx<(within_quad_col+within_quad_cols0); quad_col_idx++)
				{
					quad_error_idx= quad_error_idx_offset+ quad_col_idx;
					curr_grid_quad_error[quad_idx]+=abs(curr_quad_error[quad_error_idx] );
				}
			}
			quad_checked=1;
			 num_times_quad_checked++;				
			if(verbose>=3)
				error_reiter_gridvalues<<"\n\t -- Iter: "<<t<<" Quad-idx: "<<quad_idx<<"\t Error current quad: "<<curr_grid_quad_error[quad_idx]<<"\t  prev-quad: "<<prev_grid_quad_error[quad_idx]<<" num_times_quad_checked: "<<num_times_quad_checked<<" Quad-checked "<<quad_checked<<std::flush;
 			quad_idx--;
  		}
		else
		{
			quad_checked=0;
			num_times_quad_checked=0;
			quad_rollover_count=0;
			error_mincofactor_quad=error_mincofactor_quad_default;
			error_maxcofactor_quad=error_maxcofactor_quad_default;			
			count_quad_repeat=0;
			if(verbose>=3)
				 error_reiter_gridvalues<<"\n\t ++ Iter: "<<t<<" Quad-idx: "<<quad_idx<<"\t Error current quad: "<<curr_grid_quad_error[quad_idx]<<"\t prev-quad: "<<prev_grid_quad_error[quad_idx]<<" num_times_quad_checked: "<<num_times_quad_checked<<std::flush;
 		}

}

double error_dx_dy3=diff_curr_iter;
diff_curr_iter=0.0f;		
for(int quad_idx=0,num_times_quad_checked=0;quad_idx<grid_num_quad;quad_idx++)
{
	diff_curr_iter+=curr_grid_quad_error[quad_idx];
	error_reiter_gridvalues<<"\n\t Quad-idx: "<<quad_idx<<" curr_grid_quad_error[quad_idx] "<<curr_grid_quad_error[quad_idx]<<" prev_grid_quad_error[quad_idx] "<<prev_grid_quad_error[quad_idx];
}	
diff_curr_iter/=(num_rows*num_cols);
//cout<<endl;exit(-1);
return diff_curr_iter;
}

///////////////////////////////////////




