#include "dissippation.h"

int main(int argc, char* argv[])
{
int total_iters;
char* filename;//[20];
char* read_filename;
 
	if(argc<5)
	{
		cout<<"\n\t ERROR: "<<" Minimum required parameters: "<<"\n\t\t 1. Output filename "<<"\n\t\t 2.  Verbose "<<"\n\t\t 3. File to read input from "<<"\n\t\t 4. Number of iterations  "<<endl;
		exit(-1);
	}
	else
	{
		filename=(argv[1]);
		verbose=atoi(argv[2]);
		read_filename=(argv[3]);
		total_iters=atoi(argv[4]);
		if(argc==6)
		{
			num_rows=atoi(argv[5]);
		}
		if(argc>=7)
		{
			num_rows=atoi(argv[5]);
			num_cols=atoi(argv[6]);
		}
		if(argc>=8)
		{
			tile_size=atoi(argv[7]);
		}
	}
// filename=argv[1]; num_times=atoi(argv[2]);  if(argc==4) verbose=atoi(argv[3]);

dt= ( (double) 1/total_iters );
dx= ( (double) (xmax-xmin) /num_cols );
dy= ( (double) (ymax-ymin) /num_rows );

aux11=( (c*dt) ); aux12=( (c*dt) );
aux1= ( dx * dx ); aux2= ( dy *dy );
aux1=aux11/aux1; aux2= aux12/aux2;

aux1=0.1; aux2=0.1;
cout<<"\n\t Dt: "<<dt<<" dx: "<<dx<<" dy: "<<dy;
cout<<"\n\t aux1: "<<aux1<<" aux11: "<<aux11;
cout<<"\n\t aux2: "<<aux2<<" aux12: "<<aux12;//<<endl; 
//exit(-1);

///

	length_quad_rows=(int) ( (num_rows-2*dependency_length)/num_quad_rows );
	length_quad_cols=(int) ( (num_cols-2*dependency_length)/num_quad_cols );

	num_quad=num_quad_rows*num_quad_cols;

	num_tiles_within_row=( ( (length_quad_rows-1) / tile_size ) +1);
	num_tiles_within_col=( ( (length_quad_cols-1) / tile_size ) +1);

 	num_req_tiles=num_quad*num_tiles_within_row*num_tiles_within_col;
	num_tiles_within_quad=num_tiles_within_row*num_tiles_within_col;

	double *a1,*a2;
	a1=new double[grid_num_quad]; a2=new double[grid_num_quad];

	double *b1,*b2; 
	b1= new double[num_quad];b2= new double[num_quad];

	double *c1,*c2;
	c1=new double[num_req_tiles];c2=new double[num_req_tiles];

double prev_iter_diff_sum=0.95; // For 32x32 at iter=0, diff_iter=10.45
	for(int idx=0;idx<grid_num_quad;idx++)
	{
		a1[idx]=prev_iter_diff_sum;
		a2[idx]=prev_iter_diff_sum;
	}

	for(int idx=0;idx<num_quad;idx++)
	{
		b1[idx]=prev_iter_diff_sum/num_quad;//prev_iter_diff_sum*(grid_num_quad);//3*(num_rows);//*num_cols);
		b2[idx]=prev_iter_diff_sum/num_quad;//prev_iter_diff_sum*(grid_num_quad);//3*(num_rows);//*num_cols);
	}

	for(int idx=0;idx<num_req_tiles;idx++)
	{
		c1[idx]=0.034;//prev_iter_diff_sum/num_req_tiles;
		c2[idx]=0.034;//prev_iter_diff_sum/num_req_tiles;
	}
 

	int num_tiles_row = ( ( num_rows- 2*dependency_length -1 ) / tile_size ) + 1;
	int num_tiles_col = ( ( num_cols- 2*dependency_length -1 ) / tile_size ) + 1;
	int num_tiles= ( num_tiles_row * num_tiles_col );

///
double A[num_rows*num_cols];
double B[num_rows*num_cols];

double* heat_mat1;
double* heat_mat2;


//initialize_matrix(A,'r');save_initial_matrix(A,num_rows,num_cols,"matA",filename);exit(-1);
read_matrix(A,read_filename);//save_initial_matrix(A,num_rows,num_cols,"matA",filename);exit(-1);
read_matrix(B,read_filename);
 
double diff_in_tile_curr[num_tiles_col*num_tiles_row];
// vector<double> diff_in_tile_prev(num_tiles_col*num_tiles_row,0.0f);

double diff_curr_iter=0.0f;
double diff_iter[total_iters];
double diff_iter_track[total_iters];

int tile_idx=0;
int normal_iter=1;

int grid_update_times=0;
double prev_error_diff=0.01f;
double prev_error_diff2=0.01f;
int num_times_rollover=0;

cout<<"\t Tiles - row: "<<num_tiles_row<<" col: "<<num_tiles_col<<endl; 
double error_cofactor=1.1;

error_reiter_gridvalues.open( "error_reiter_gridvalues.log", ios:: out );
error_reiter_log.open("error_reiter.log",ios::out);


for (int t = 0; t < total_iters; t++)
{

	if(t%2==0)
	{
		heat_mat1=B;
		heat_mat2=A;

		curr_grid_quad_error=a2;
		curr_quad_error=b2;
		curr_tile_error=c2;

		prev_grid_quad_error=a1;
		prev_quad_error=b1;
		prev_tile_error=c1;

	}
	else
	{
		heat_mat1=A;
		heat_mat2=B;

		curr_grid_quad_error=a1;
		curr_quad_error=b1;
		curr_tile_error=c1;

		prev_grid_quad_error=a2;
		prev_quad_error=b2;
		prev_tile_error=c2;
 
	}
//cout<<"\n\t Iter: "<<t; 

		int t_div500=t/500;
switch(t_div500)
{
case 0:
	error_cofactor_quad_default=1.05;
	error_cofactor_default=1.05;
break;

case 1: 
	error_cofactor_quad_default=1.08;
	error_cofactor_default=1.08;
case 2:
	error_cofactor_quad_default=1.1;
	error_cofactor_default=1.1;
break;
case 3:
	error_cofactor_quad_default=1.13;
	error_cofactor_default=1.13;
default: 
	error_cofactor_quad_default=1.15;
	error_cofactor_default=1.15;
break;
}


	if(normal_iter) 
	diff_curr_iter=grid_analyze_per_quadrant(heat_mat1,heat_mat2,t);
	double error_dx_dy1=diff_curr_iter;
	
	if(t==1000)
	{
		cout<<endl;exit(-1);
	}
	diff_curr_iter=scan_correct(heat_mat1,heat_mat2,error_dx_dy1,t);	error_reiter_gridvalues<<"\n\t After correction: "<<diff_curr_iter<<"\t Before-correction: "<<error_dx_dy1;
 
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
	// double iter_diff=abs(diff_curr_iter-prev_iter_diff_sum); // cout<<"\n\t  iter_diff: "<< iter_diff<<" error_cofactor*prev_error_diff  "<<error_cofactor*prev_error_diff ;
	 //if( ( abs(diff_curr_iter) <= abs(error_cofactor * prev_iter_diff_sum) ) && ( iter_diff <= ( error_cofactor*prev_error_diff2 ) ) && ( diff_curr_iter > 0 ) )
	 if( ( abs(diff_curr_iter) <= abs(error_cofactor * prev_iter_diff_sum) )  && ( diff_curr_iter > 0 ) )
	{
		diff_iter[t]=diff_curr_iter;	
		char mat_name[20];
		
	 	//error_reiter_gridvalues<<"\n\t At iter: "<<t<<" diff_curr_iter "<<diff_curr_iter<<" is less than "<<prev_iter_diff_sum<<" !! ";
 		//sprintf(mat_name,"%s_%d","diff_in_tile_curr",t);//save_matrix(diff_in_tile_curr,num_tiles_row*num_tiles_col,1,mat_name,filename);
		prev_error_diff=prev_iter_diff_sum-diff_curr_iter;
		prev_error_diff2=prev_error_diff;
		prev_iter_diff_sum=diff_curr_iter;
		normal_iter=1;
		grid_update_times=0;
		num_times_rollover=0;
 
	}
	else
	{
		//if(t%1000==0) 
		//cout<<"\n\t At iter: "<<t<<" diff_curr_iter "<<diff_curr_iter<<" is not_less than "<<prev_iter_diff_sum<<" reiterating for grid-update: "<<grid_update_times;
		//error_reiter_gridvalues<<"\n\t At iter: "<<t<<" diff_curr_iter "<<diff_curr_iter<<" is not_less than "<<prev_iter_diff_sum<<" reiterating for grid-update: "<<grid_update_times; 		
		//error_reiter_log<<"\n\t At iter: "<<t<<" diff_curr_iter "<<diff_curr_iter<<" is not_less than "<<prev_iter_diff_sum<<" reiterating for grid-update: "<<grid_update_times; 		
		normal_iter=0;
		t=t-1;
 
		grid_update_times++;
		if(grid_update_times > max_iter_repeat)
		{
			grid_update_times=0;
			normal_iter=0;
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
		//cout<<"\n\t At iter: "<<t<<" diff_curr_iter "<<diff_curr_iter<<" is not_less than "<<prev_iter_diff_sum<<" reiterating for grid-update: "<<grid_update_times<<" rollover_times: "<<num_times_rollover<<"! ";
			// cout<<"\n\t At iter: "<<t<<" diff_curr_iter "<<diff_curr_iter<<" is not_less than "<<prev_iter_diff_sum<<" reiterating for grid-update: "<<grid_update_times<<" rollover_times: "<<num_times_rollover<<"! ";
			error_reiter_gridvalues<<"\n\t At iter: "<<t<<" diff_curr_iter "<<diff_curr_iter<<" is not_less than "<<prev_iter_diff_sum<<" reiterating for grid-update: "<<grid_update_times<<" rollover_times: "<<num_times_rollover<<"! ";
		}
	}
}

cout<<"\n\t After "<<total_iters<<" iterations "<<" analyze_rep: "<<diff_curr_iter;

int track_accumulator=0;
diff_iter_track[0]=-1;
for(int iter=1;iter<total_iters;iter++)
{
	if( diff_iter[iter] < diff_iter[iter-1] )
	{
		diff_iter_track[iter]=1;
		track_accumulator+=1;
	}
	else
		diff_iter_track[iter]=0;
	//cout<<"\n\t Idx: "<<" diff_iter[iter] "<<diff_iter[iter]<<" diff_iter[idx-1] "<<diff_iter[iter-1]<<" track_accumulator "<<track_accumulator;
}
cout<<"\n\n\t Track_accumulator: "<<track_accumulator<<endl;

save_matrix(diff_iter,total_iters,1,"diff_iter",filename);
save_matrix(heat_mat1,num_rows,num_cols,"matA",filename);
return 0;
}
