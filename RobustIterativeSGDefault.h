#include<iostream>
#include<istream>
#include<ostream>
#include<fstream>

#include<cmath>
#include<cstdlib>
#include<unistd.h>

#include<string>
#include<string.h>

#include<ctime>
#include <sys/time.h>
#include<vector>

using namespace std;

////// Grid division /////
int within_quad_rows0=2;
int within_quad_cols0=2;

const int num_quad_level1=6;
int tile_size=4;
///// Block size/////

ofstream error_reiter_gridvalues,error_reiter_log;
const int max_check_times=2;
const int dependency_length=1;
int num_rows=16+2*dependency_length;
int num_cols=16+2*dependency_length; 

 int num_times=1;
int max_iter_repeat=4;

int max_quad_check=2;
int max_quad_rollover=12;
const int max_tile_check=2;

double tile_error_cofactor=1.0;

double error_cofactor_quad=1.05;
double error_cofactor=1.05;

double error_cofactor_default=1.05;
double error_cofactor_quad_default=1.05;

/////////////////////////
const double pi=3.1415926535897932384626433832795;
const double xmin=-1;const double xmax=1;
const double ymin=xmin;const double ymax=xmax;
double c=1.0/(pi*pi);
double dt;
double dx;
double dy;

double aux11=0.0f,aux12=0.0f;
double aux1=0.0f,aux2=0.0f;


/////////////////////////
int verbose=0;// The command line option specifies which part of the computations are printed to standard io/file.

ofstream save_mat_file;

double scan_correct(double* heat_mat1,double* heat_mat2,double diff_curr_iter,int t);

//// Following variables are required to sync all the code from source file
double* diff_iter;
int num_times_roll_over; //=0;
double prev_iter_diff_sum;
double prev_error_diff;
double prev_error_diff2;
int grid_update_times;
char* filename;
int num_times_rollover;
int InitNotCalled=1;

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

double *a1,*a2;
double *b1,*b2;
double *c1,*c2;

int total_iters;		
void init(int num_rows, int num_cols, int dependency_length) 
{

	within_quad_rows1=num_quad_level1;
	within_quad_cols1=num_quad_level1;

	num_quad_rows=within_quad_rows1*within_quad_rows0;
	num_quad_cols=within_quad_cols1*within_quad_cols0;

	grid_num_quad=within_quad_rows1*within_quad_cols1;

	length_quad_rows=(int) ( (num_rows-2*dependency_length)/num_quad_rows );
	length_quad_cols=(int) ( (num_cols-2*dependency_length)/num_quad_cols );

	num_quad=num_quad_rows*num_quad_cols;

	num_tiles_within_row=( ( (length_quad_rows-1) / tile_size ) +1);
	num_tiles_within_col=( ( (length_quad_cols-1) / tile_size ) +1);

	num_req_tiles=num_quad*num_tiles_within_row*num_tiles_within_col;
	num_tiles_within_quad=num_tiles_within_row*num_tiles_within_col;

	 
	dt= ( (double) 1/total_iters );
	dx= ( (double) (xmax-xmin) /num_cols );
	dy= ( (double) (ymax-ymin) /num_rows );

	aux11=( (c*dt) ); aux12=( (c*dt) );
	aux1= ( dx * dx ); aux2= ( dy *dy );
	aux1=aux11/aux1; aux2= aux12/aux2;

	aux1=0.1; aux2=0.1;
	
	length_quad_rows=(int) ( (num_rows-2*dependency_length)/num_quad_rows );
	length_quad_cols=(int) ( (num_cols-2*dependency_length)/num_quad_cols );

	num_quad=num_quad_rows*num_quad_cols;

	num_tiles_within_row=( ( (length_quad_rows-1) / tile_size ) +1);
	num_tiles_within_col=( ( (length_quad_cols-1) / tile_size ) +1);

 	num_req_tiles=num_quad*num_tiles_within_row*num_tiles_within_col;
	num_tiles_within_quad=num_tiles_within_row*num_tiles_within_col;

	a1=new double[grid_num_quad]; a2=new double[grid_num_quad];
	b1= new double[num_quad];b2= new double[num_quad];
	c1=new double[num_req_tiles];c2=new double[num_req_tiles];


	prev_iter_diff_sum=0.95;

	for(int idx=0;idx<grid_num_quad;idx++)
	{
		a1[idx]=prev_iter_diff_sum;
		a2[idx]=prev_iter_diff_sum;
	}

	for(int idx=0;idx<num_quad;idx++)
	{
		b1[idx]=prev_iter_diff_sum/num_quad;
		b2[idx]=prev_iter_diff_sum/num_quad;
	}

	for(int idx=0;idx<num_req_tiles;idx++)
	{
		c1[idx]=0.034;
		c2[idx]=0.034;
	}

	int num_tiles_row = ( ( num_rows- 2*dependency_length -1 ) / tile_size ) + 1;
	int num_tiles_col = ( ( num_cols- 2*dependency_length -1 ) / tile_size ) + 1;
	int num_tiles= ( num_tiles_row * num_tiles_col );

}



int grid_analyze_per_quadrant(double* heat_mat1,double* heat_mat2,int num_rows,int num_cols,int dependency_length,int datalayout,int iter)
{
 
 
if(InitNotCalled)
{
	init(num_rows,num_cols,dependency_length);//,total_iters);
	InitNotCalled=0;
}


	if(iter%2==0)
	{
		curr_grid_quad_error=a2;
		curr_quad_error=b2;
		curr_tile_error=c2;

		prev_grid_quad_error=a1;
		prev_quad_error=b1;
		prev_tile_error=c1;

	}
	else
	{
		curr_grid_quad_error=a1;
		curr_quad_error=b1;
		curr_tile_error=c1;

		prev_grid_quad_error=a2;
		prev_quad_error=b2;
		prev_tile_error=c2;
 
	}

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
		cout<<"\n\t Duh!! \n";
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

						double temp=heat_mat2[index];
					   	insert_equation
					   	//heat_mat1[index] =   aux1 * ( heat_mat2[index+num_cols]  - 2.0 * heat_mat2[index] + heat_mat2[index-num_cols] ) + aux2 * (heat_mat2[index+1] - 2.0 * heat_mat2[index] + heat_mat2[index-1] ) + heat_mat2[index];
						
						 error_tile+=( ( temp-heat_mat1[index] ) * ( temp-heat_mat1[index] )  ); //- heat_mat1[index-1]  );
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
 	}
 
  	double diff_curr_iter=error; //_dx_dy;
 	double error_dx_dy1;
	int t=iter;
	
	while(true)
	{
		error_dx_dy1=diff_curr_iter;
		diff_curr_iter=scan_correct(heat_mat1,heat_mat2,error_dx_dy1,iter);	 
		
	 	int t_div= t/100;
		switch(t_div)
		{
			case 1: error_cofactor=1.05;
				  break;
			case 2: error_cofactor=1.05;
				  break;
			default: error_cofactor=1.05;
		}
			diff_iter[t]=diff_curr_iter;	
		 if(num_times_rollover>2 ) error_cofactor*=num_times_rollover;
 		 if( ( abs(diff_curr_iter) <= abs(error_cofactor * prev_iter_diff_sum) )  && ( diff_curr_iter > 0 ) )
		{
			diff_iter[t]=diff_curr_iter;	
			char mat_name[20];
		
		 	//error_reiter_gridvalues<<"\n\t At iter: "<<t<<" diff_curr_iter "<<diff_curr_iter<<" is less than "<<prev_iter_diff_sum<<" !! ";
	 		//sprintf(mat_name,"%s_%d","diff_in_tile_curr",t);//save_matrix(diff_in_tile_curr,num_tiles_row*num_tiles_col,1,mat_name,filename);
			prev_error_diff=prev_iter_diff_sum-diff_curr_iter;
			prev_error_diff2=prev_error_diff;
			prev_iter_diff_sum=diff_curr_iter;
			//normal_iter=1;
			grid_update_times=0;
			num_times_rollover=0;
	 		return diff_curr_iter;
		}
		else
		{
 	 
			grid_update_times++;
			if(grid_update_times > max_iter_repeat)
			{
				grid_update_times=0;
				//normal_iter=0;
				prev_iter_diff_sum+=prev_error_diff;
				prev_error_diff=0.0;
				num_times_rollover++;
				if(num_times_rollover>4)
				{
					cout<<"\n\t At iter: "<<t<<" diff_curr_iter "<<diff_curr_iter<<" is not_less than "<<prev_iter_diff_sum<<" reiterating for grid-update: "<<grid_update_times<<" rollover_times: "<<num_times_rollover<<"! ";
					save_matrix(heat_mat1,num_rows,num_cols,"matA",filename);
					save_matrix(heat_mat2,num_rows,num_cols,"matB",filename);
					exit(-1);
			
				}	
 
				cout<<"\n\t At iter: "<<t<<" diff_curr_iter "<<diff_curr_iter<<" is not_less than "<<prev_iter_diff_sum<<" reiterating for grid-update: "<<grid_update_times<<" rollover_times: "<<num_times_rollover<<"! ";
			}
 
		}
 	
 	}
 	

}


double scan_correct(double* heat_mat1,double* heat_mat2,double diff_curr_iter,int t)
{

int count_quad_repeat=0;
int quad_rollover_count=0;
int quad_checked=0;
	
for(int quad_idx=0,num_times_quad_checked=0;quad_idx<grid_num_quad;quad_idx++)
{

 if(verbose>=3)
 	 error_reiter_gridvalues<<"\n\t ** Iter: "<<t<<" Quad-idx: "<<quad_idx<<"\t Error current quad: "<<curr_grid_quad_error[quad_idx]<<"\t prev-quad: "<<prev_grid_quad_error[quad_idx]<<" num_times_quad_checked: "<<num_times_quad_checked;;
 
	if( ( ( abs(curr_grid_quad_error[quad_idx]) > (error_cofactor_quad*abs(prev_grid_quad_error[quad_idx] ) ) )  || (!quad_checked)) && (quad_rollover_count < max_quad_rollover) )
	{

		count_quad_repeat++;
		if(  count_quad_repeat == max_iter_repeat )
		{
			error_cofactor_quad+=0.01;
			count_quad_repeat=0;
	 		quad_rollover_count++;
 		}						 


		 		int quad_row= (quad_idx/within_quad_rows1);
		int quad_col=(quad_idx%within_quad_cols1);

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
	 				if( ( (abs(curr_quad_error[quad_error_idx]) > abs(prev_quad_error[quad_error_idx] ) ) || (!subquad_checked) )  || (curr_quad_error[quad_error_idx] < 0) )
					{
	 					start_check_tile= quad_error_idx * num_tiles_within_quad;
						int tile_num_times_checked=0;
						for(int check_tile_idx=start_check_tile;check_tile_idx<(start_check_tile+num_tiles_within_quad);check_tile_idx++)
						{
 
						if(verbose>=5)
							 error_reiter_gridvalues<<"\n\t\t\t Iter: "<<t<<"\t tile-idx: "<<check_tile_idx<<" curr_tile_error[check_tile_idx]: "<<curr_tile_error[check_tile_idx]<<" prev_tile_error[check_tile_idx]: "<<prev_tile_error[check_tile_idx];
 	
							double tile_diff;
									tile_diff =  abs(curr_tile_error[check_tile_idx]) - abs(prev_tile_error[check_tile_idx] ) ; 
									if( ( ( tile_diff > tile_error_cofactor*abs(prev_tile_error[check_tile_idx] ) ) || ( curr_tile_error[check_tile_idx] < 0 ) )&& (tile_num_times_checked< max_tile_check) )
 	 								{
	  									int check_tile_idx_start_row=(quad_row_idx*num_tiles_within_row*tile_size)+( ( (check_tile_idx-start_check_tile)/num_tiles_within_row) *tile_size)+dependency_length;
										int check_tile_idx_start_col=(quad_col_idx*num_tiles_within_col*tile_size)+( ( (check_tile_idx-start_check_tile)%num_tiles_within_col) *tile_size)+dependency_length;
										int num_updates=0;
 
 										double error_quad=0.0f;
	 									for(int row_idx=check_tile_idx_start_row; ( row_idx< ( check_tile_idx_start_row+tile_size ) && (row_idx<=(num_rows-dependency_length)) );row_idx++ )
										{
											int row_offset=row_idx*num_cols;
											int index=0;
											for(int col_idx=check_tile_idx_start_col; ( col_idx< ( check_tile_idx_start_col+tile_size ) && (col_idx<=(num_cols-dependency_length)) );col_idx++ )
											{
	 											index=row_offset+col_idx;
												double temp=heat_mat2[index];
												insert_equation
												//heat_mat1[index] =   aux1 * ( heat_mat2[index+num_cols]  - 2.0 * heat_mat2[index] + heat_mat2[index-num_cols] ) + aux2 * (heat_mat2[index+1] - 2.0 * heat_mat2[index] + heat_mat2[index-1] ) + heat_mat2[index];					   
						
												 error_quad+=( ( temp-heat_mat1[index] ) * ( temp-heat_mat1[index] )  ); //- heat_mat1[index-1]  );
 
													if(verbose>=6)
														 error_reiter_gridvalues<<"\n\t\t\t Updating index: "<<index<<" row: "<<row_idx<<" col: "<<col_idx<<"\t heat_mat1[index]: "<<heat_mat1[index]<<" heat_mat2[index] "<<heat_mat2[index];
 												

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
 
 								curr_quad_error[quad_error_idx]=0;
								double diff_tile=0.0f;
								for(int check_tile_idx=start_check_tile;check_tile_idx<(start_check_tile+num_tiles_within_quad);check_tile_idx++)
								{
									curr_quad_error[quad_error_idx]+=abs(curr_tile_error[check_tile_idx]);
								}
								
								for(int check_tile_idx=start_check_tile;check_tile_idx<(start_check_tile+num_tiles_within_quad);check_tile_idx++)
								{	
									diff_tile= curr_tile_error[check_tile_idx]  - prev_tile_error[check_tile_idx];
									if( ( abs(diff_tile) > abs(200*prev_tile_error[check_tile_idx] ) ) || ( curr_tile_error[check_tile_idx] < 0 ) ) 
									{
										curr_quad_error[quad_error_idx]+=abs(curr_tile_error[check_tile_idx]);
										error_reiter_gridvalues<<"\n\t Iter: "<<t<<" quad-idx: "<<quad_idx<<" quad_error_idx "<<quad_error_idx<<" tile-idx: "<<check_tile_idx<<" curr: "<<curr_tile_error[check_tile_idx]<<" prev "<<prev_tile_error[check_tile_idx]<<" diff_tile "<<diff_tile;
										error_reiter_gridvalues<<" curr_quad_error[quad_error_idx] "<<curr_quad_error[quad_error_idx] <<" prev_quad_error[quad_error_idx] "<<prev_quad_error[quad_error_idx] ;											
 											
										quad_col_idx--;
										subquad_checked=1;
										check_tile_idx+=num_tiles_within_quad;
										// error_reiter_gridvalues;sleep(15);exit(-1);
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
			error_cofactor_quad=error_cofactor_quad_default;
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
	if(verbose>=3)
		error_reiter_gridvalues<<"\n\t Quad-idx: "<<quad_idx<<" curr_grid_quad_error[quad_idx] "<<curr_grid_quad_error[quad_idx]<<" prev_grid_quad_error[quad_idx] "<<prev_grid_quad_error[quad_idx];
}	
return diff_curr_iter;
}

///////////////////////////////////////







