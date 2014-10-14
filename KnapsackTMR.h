#include "error_inject_operator.h"

 inline error_inject_operators<int> tmr_greater( error_inject_operators<int> a1,error_inject_operators<int> a2)
 {
 	error_inject_operators<int> res1,res2,res3;
 	res1= a1 > a2;
 	res2= a1 > a2;
 	res3= a1 > a2;
 	error_inject_operators<int> result;
 	if(  res1 == res2 )
 	{
 		result=res1;
 	}
 	else if (res2 == res3) 
 	{
 		result=res2; 	
 	}
 	else if (res1 == res3)
 	{
 		result=res3; 	
 	}
 	else
 	{
 		cout<<"\n\t FATAL tmr_greater op1 "<<a1<<" op2 "<<a2;
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	tmr_greater(a1,a2);//exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	return result;
 }
 
inline error_inject_operators<int> tmr_greater( error_inject_operators<int> a1,int a2)
 {
 	error_inject_operators<int> res1,res2,res3;
 	res1= a1 > a2;
 	res2= a1 > a2;
 	res3= a1 > a2;
 	error_inject_operators<int> result;
 	if(  res1 == res2 )
 	{
 		result=res1;
 	}
 	else if (res2 == res3) 
 	{
 		result=res2; 	
 	}
 	else if (res1 == res3)
 	{
 		result=res3; 	
 	}
 	else
 	{
 		cout<<"\n\t FATAL tmr_greater op1 "<<a1<<" op2 "<<a2;
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	tmr_greater(a1,a2);//exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	return result;
 }


 inline error_inject_operators<int> tmr_add ( error_inject_operators<int> a1,error_inject_operators<int> a2)
 {
 	error_inject_operators<int> res1,res2,res3;
 	res1= a1 + a2;
 	res2= a1 + a2;
 	res3= a1 + a2;
 	error_inject_operators<int> result;
 	if(  (res1 == res2 ) || (res1==res2) || (res1==res2) )
 	{
 		result=res1;
 	}
 	else if ( (res3 == res2 ) || (res3==res2) || (res3==res2) )
 	{
 		result=res2; 	
 	}
 	else if ( (res1 == res3 ) || (res1==res3) || (res1==res3) )
 	{
 		result=res3; 	
 	}
 	else
 	{
 		cout<<"\n\t FATAL tmr_add op1 "<<a1<<" op2 "<<a2; 	
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	tmr_add(a1,a2);//exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	return result;
 }

 inline error_inject_operators<int> tmr_add ( error_inject_operators<int> a1,int a2)
 {
 	error_inject_operators<int> res1,res2,res3;
 	res1= a1 + a2;
 	res2= a1 + a2;
 	res3= a1 + a2;
 	error_inject_operators<int> result;
 	if(  (res1 == res2 ) || (res1==res2) || (res1==res2) )
 	{
 		result=res1;
 	}
 	else if ( (res3 == res2 ) || (res3==res2) || (res3==res2) )
 	{
 		result=res2; 	
 	}
 	else if ( (res1 == res3 ) || (res1==res3) || (res1==res3) )
 	{
 		result=res3; 	
 	}
 	else
 	{
 		cout<<"\n\t FATAL tmr_add op1 "<<a1<<" op2 "<<a2; 	
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	tmr_add(a1,a2);//exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	return result;
 }


 inline error_inject_operators<int> tmr_sub ( error_inject_operators<int> a1,error_inject_operators<int> a2)
 {
 	error_inject_operators<int> res1,res2,res3;
 	res1= a1 - a2;
 	res2= a1 - a2;
 	res3= a1 - a2;
 	error_inject_operators<int> result;
 	if(  (res1 == res2 ) || (res1==res2) || (res1==res2) )
 	{
 		result=res1;
 	}
 	else if ( (res3 == res2 ) || (res3==res2) || (res3==res2) )
 	{
 		result=res2; 	
 	}
 	else if ( (res1 == res3 ) || (res1==res3) || (res1==res3) )
 	{
 		result=res3; 	
 	}
 	else
 	{
 		cout<<"\n\t FATAL tmr_sub op1 "<<a1<<" op2 "<<a2; 	
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	tmr_sub(a1,a2);//exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	return result;
 }

 inline error_inject_operators<int> tmr_sub ( int a1,int a2)
 {
 	error_inject_operators<int> res1,res2,res3;
 	res1= a1 - a2;
 	res2= a1 - a2;
 	res3= a1 - a2;
 	error_inject_operators<int> result;
 	if(  (res1 == res2 ) || (res1==res2) || (res1==res2) )
 	{
 		result=res1;
 	}
 	else if ( (res3 == res2 ) || (res3==res2) || (res3==res2) )
 	{
 		result=res2; 	
 	}
 	else if ( (res1 == res3 ) || (res1==res3) || (res1==res3) )
 	{
 		result=res3; 	
 	}
 	else
 	{
 		cout<<"\n\t FATAL tmr_sub op1 "<<a1<<" op2 "<<a2; 	
	 	cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" are not equal and hence, exitting! \n ";
	 	tmr_sub(a1,a2);//exit(-1);
 	}
 	
 	//cout<<"\n\t res1: "<<res1<<" res2: "<<res2<<" res3: "<<res3<<" result "<<result<<endl;
 	return result;
 }
 
class Knapsack
{

	int num_items;
	int max_weight;
	int checkpoint_length;
	int num_checkpoints;
	int*  weights;
	int*	values;
	error_inject_operators<int> final_value;
	error_inject_operators<int>* array_checkpoints_1;
	error_inject_operators<int>* array_checkpoints_2;	
	error_inject_operators<int>* array_checkpoints_3;		
	error_inject_operators<int>* curr_item_1_checkpoints;
	error_inject_operators<int>* curr_item_2_checkpoints;
	error_inject_operators<int>* curr_item_checkpoints;	
	error_inject_operators<int>** eval_mat;
public:
	Knapsack(int num_items_ip,int max_weight_ip,int checkpoint_length_ip,int seed_reset_ip=0)
	{
		if(seed_reset_ip)
		{
			struct timeval t1;
			gettimeofday(&t1,0);
			srand(t1.tv_sec * t1.tv_usec);
		}
		num_items=num_items_ip;
		max_weight=max_weight_ip;
		checkpoint_length=checkpoint_length_ip;
		num_checkpoints=( (max_weight-1)/checkpoint_length)+1;
		cout<<"\n\t Num-items: "<<num_items<<"\t Max-weight: "<<max_weight<<"\t Checkpoint-length: "<<checkpoint_length<<"\t Num_checkpoints: "<<num_checkpoints<<endl;
		weights= new int[num_items];
		values= new int[num_items];
		array_checkpoints_1=new error_inject_operators<int>[num_checkpoints] ;
		array_checkpoints_2=new error_inject_operators<int>[num_checkpoints] ;
		array_checkpoints_3=new error_inject_operators<int>[num_checkpoints] ;				
		eval_mat=new error_inject_operators<int>*[num_items];
				
		for(int i=0;i<num_items;i++)
		{
			weights[i]=rand()%5;
			weights[i]=weights[i]+1;
			values[i]=rand()%100;
			eval_mat[i]=new error_inject_operators<int>[max_weight];
		}
		
	}
	
	void print_info()
	{
	
		for(int i=0;i<num_items;i++)
			cout<<"\n\t I: "<<i<<" W: "<<weights[i]<<" V: "<<values[i];
		cout<<endl;
		for(int i=0;i<num_items;i++)
		{
			cout<<"\n \n Item^^>>"<<i;
			for(int j=0;j<max_weight;j++)
			cout<<"\t "<<eval_mat[i][j];
		
		}			
		cout<<"\n\n\t Final-value: "<<final_value<<endl<<endl;
		//cout<<endl;
	}
	
	void fill_matrix()
	{
	
		cout<<"\n\t In fill-matrix() \n";
		for(int weight=0;weight<max_weight;weight++)
			eval_mat[0][weight]=0;

		/* for(int items=0;items<num_items;items++)
			eval_mat[items][0]=0;

		{
			int items=1;
			curr_item_1_checkpoints=array_checkpoints_3;
			curr_item_2_checkpoints=array_checkpoints_1;				
			for(int checkpoint_zone=0,weight=0;checkpoint_zone<num_checkpoints;checkpoint_zone++)
			{
				//cout<<"\n -- In Checkpoint-zone: "<<checkpoint_zone<<" weight: "<<weight<<endl;
				weight=checkpoint_zone*checkpoint_length;
				error_inject_operators<int> max_in_zone;
				max_in_zone=0;				
				int weights_bound= min( (weight+checkpoint_length),max_weight );
				error_inject_operators<int> duh; //Need a constructor which can take value at declaration.

				duh=0;
				for( ;weight<weights_bound;weight++)
				{
					int trace_flag=0;						
					eval_mat[items][weight]=eval_mat[items-1][weight];

					if( (weight- weights[items]) >=0 )
					{
						duh=(eval_mat[items-1][weight-weights[items]]+values[items] );
						if(  eval_mat[items][weight] < ( duh)  )
						//	eval_mat[items][weight]=eval_mat[items][weight];
						//else
						{
							eval_mat[items][weight]=duh;
							//trace_flag=1;
						}
				
					}
					if( max_in_zone < eval_mat[items][weight] )
						max_in_zone=eval_mat[items][weight];					
					//cout<<"\n\t\t I: "<<items<<" weight: " <<weight<<" weights[items]: "<<(weights[items])<<" trace-flag: "<<trace_flag<<" max_in_zone: "<<max_in_zone<<" eval_mat[items][weight]: "<<eval_mat[items][weight];
				}
				curr_item_1_checkpoints[checkpoint_zone]=max_in_zone;
				curr_item_2_checkpoints[checkpoint_zone]=0;
		
			}
			
		} */
		
		//for(int items=2;items<num_items;items++)
		for(int items=2;items<num_items;items++)
		{
			error_inject_operators<int> duh;
			error_inject_operators<int> temp;
			error_inject_operators<int> temp1;	
			error_inject_operators<int> temp2;			
			for(int weight=0; weight< max_weight ;weight++)
			{
					eval_mat[items][weight]=eval_mat[items-1][weight];

					//if( ( weight- weights[items]) >=0 )
					temp=weight-weights[items]; //tmr_sub(weight,weights[items]);
					temp1=tmr_greater( temp ,0 );
					if( temp1.operand )
					{
						duh=tmr_add(eval_mat[items-1][weight-weights[items]],values[items] );
						temp2=tmr_greater( duh, eval_mat[items][weight]);
						if(  temp2.operand)
						{
							eval_mat[items][weight]=duh;
							//trace_flag=1;
						}
				
					}
			
			
			}		
		
			/*switch( items%3)
			{
				case 0: 
					curr_item_checkpoints=array_checkpoints_1;
					curr_item_1_checkpoints=array_checkpoints_2;
					curr_item_2_checkpoints=array_checkpoints_3;					
					//cout<<"\n\t Case-0 !!";
				break;
				case 1:
					curr_item_checkpoints=array_checkpoints_3;
					curr_item_1_checkpoints=array_checkpoints_1;
					curr_item_2_checkpoints=array_checkpoints_2;					
					//cout<<"\n\t Case-1 !!";
				break;
				case 2:
					curr_item_checkpoints=array_checkpoints_2;
					curr_item_1_checkpoints=array_checkpoints_3;
					curr_item_2_checkpoints=array_checkpoints_1;					
					//cout<<"\n\t Case-2 !! "<<curr_item_1_checkpoints[0]<<" "<<curr_item_2_checkpoints[0];
				break;
			
			}
		
		
				error_inject_operators<int> b4_rollback_max;
				error_inject_operators<int> yet_to_rollback_prev_item;
				b4_rollback_max=0;
				yet_to_rollback_prev_item=1;		
				error_inject_operators<int> max_in_zone,max_in_prev_zone;
				max_in_zone=0;				
		
		
		
			for(int checkpoint_zone=0,weight=0;checkpoint_zone<num_checkpoints;checkpoint_zone++,max_in_prev_zone=curr_item_checkpoints[checkpoint_zone-1],max_in_zone=0)
			{
				//cout<<"\n -- In Checkpoint-zone: "<<checkpoint_zone<<" weight: "<<weight<<endl;
				weight=checkpoint_zone*checkpoint_length;
				int weights_bound= min( (weight+checkpoint_length),max_weight );
				error_inject_operators<int> duh; //Need a constructor which can take value at declaration.

				int checkpoint_zone1=( (weight-weights[items])/checkpoint_length ) ;
			//	if(!checkpoint_zone1) checkpoint_zone1=0;
				int checkpoint_zone2=((weight+checkpoint_length-weights[items])/checkpoint_length);
			//	if(!checkpoint_zone2) checkpoint_zone2=0;				
				error_inject_operators<int> max_checkpoint;
				max_checkpoint=values[items];
				if( tmr_greater( curr_item_1_checkpoints[checkpoint_zone1] , curr_item_1_checkpoints[checkpoint_zone2] ) >=1 )
				{
					max_checkpoint=tmr_add(max_checkpoint , curr_item_1_checkpoints[checkpoint_zone1]);
					//cout<<"\n\t Zone1 max_checkpoint: "<<max_checkpoint;
				}
				else
				{
					max_checkpoint=tmr_add(max_checkpoint , curr_item_1_checkpoints[checkpoint_zone2]);				
					//cout<<"\n\t Zone2 max_checkpoint: "<<max_checkpoint;					
				}
				if( tmr_greater(curr_item_1_checkpoints[checkpoint_zone] ,max_checkpoint ) >=1 )
				{
					max_checkpoint=curr_item_1_checkpoints[checkpoint_zone];
					//cout<<"\n\t MyZone ";
				}
				//cout<<"\n\t -- In Checkpoint-zone: "<<checkpoint_zone<<" max_checkpoint "<<max_checkpoint<<" curr_item_1_checkpoints[checkpoint_zone] "<<curr_item_1_checkpoints[checkpoint_zone]<<" values[items] "<< values[items]<<" checkpoint_zone1: "<<( curr_item_1_checkpoints[checkpoint_zone1] )<<" checkpoint_zone2: "<<( curr_item_1_checkpoints[checkpoint_zone2] );
				//cout<<"\n\t -- In Checkpoint-zone: "<<checkpoint_zone<<" max_checkpoint "<<max_checkpoint<<" checkpoint_zone1 "<<checkpoint_zone1<<" checkpoint_zone2 "<<checkpoint_zone2;
				duh=0;
				for( ;weight<weights_bound;weight++)
				{
					int trace_flag=0;						
					eval_mat[items][weight]=eval_mat[items-1][weight];

					if( (weight- weights[items]) >=0 )
					{
						duh=(eval_mat[items-1][weight-weights[items]]+values[items] );
						if(  eval_mat[items][weight] < ( duh)  )
						//	eval_mat[items][weight]=eval_mat[items][weight];
						//else
						{
							eval_mat[items][weight]=duh;
							trace_flag=1;
						}
				
					}
					if( max_in_zone < eval_mat[items][weight] )
						max_in_zone=eval_mat[items][weight];					
					//cout<<"\n\t\t I: "<<items<<" weight: " <<weight<<" weights[items]: "<<(weights[items])<<" trace-flag: "<<trace_flag<<" max_in_zone: "<<max_in_zone<<" eval_mat[items][weight]: "<<eval_mat[items][weight];
				}
			
				
				
				 //The or part takes care of cases when fault happens at last point of the checkpoint zone- The 2nd operand to and takes care of cases where the first item is being considered ( where its expected to have max_in_zone==values[items])
				duh=(eval_mat[items-1][weight-1-weights[items]]+values[items] ); // Weight should not be zero is the assumption :'(
				//yet_to_rollback_prev_item
				//if( (max_in_zone!=eval_mat[items][weight-1] ) || ( ( !(max_in_zone - eval_mat[items][weight-1]) ) && ( (eval_mat[items][weight-1]!=duh  ) && (eval_mat[items-1][weight-1]!=eval_mat[items][weight-1]) ) ) )
				if(  ( max_in_zone >  max_checkpoint )  ) //|| (max_in_zone!=eval_mat[items][weight-1] ) )
				{
					if( yet_to_rollback_prev_item ==1 )
					{
						yet_to_rollback_prev_item=0;
						checkpoint_zone--;	
						b4_rollback_max=max_in_zone;				
					}
					else
					{
						yet_to_rollback_prev_item=1;
						//if( b4_rollback_max != max_in_zone)
						{
							cout<<"\n\t FATAL----Need to roll back a row/item at: "<<items<<" checkpoint-zone: "<<checkpoint_zone<<" max_in_zone: "<<max_in_zone<<" max_checkpoint "<<max_checkpoint;
							checkpoint_zone--;
							items--;
						}
						//else
						//	b4_rollback_max=0;
						
					}
					cout<<"\n\t --- Item-#: "<<items<<" weight-iter: "<<weight<<"\t checkpoint_zone: "<<checkpoint_zone<<"\t weight: "<<weights[items]<<"\t eval_mat[items][weight] "<<eval_mat[items][weight-1]<<" max_in_zone: "<<max_in_zone<<" max_checkpoint "<<max_checkpoint<<" eval_mat[items-1][weight-1-weights[items]] "<< eval_mat[items-1][weight-1-weights[items]]<<" values[items] "<<values[items]<<" b4_rollback_max: "<<b4_rollback_max<<endl;
					
				}
				else
				{
					//b4_rollback_max=0;
					curr_item_checkpoints[checkpoint_zone]=max_in_zone;
					if(max_in_zone>max_checkpoint) cout<<"\n\t NEW_FATAL_ALERT: max_in_zone: "<<max_in_zone<<" max_checkpoint "<<max_checkpoint;
					//cout<<"\n\t Item-#: "<<items<<" weight-iter: "<<weight<<"\t checkpoint_zone: "<<checkpoint_zone<<" max-in-zone: "<<max_in_zone<<" max_checkpoint "<<max_checkpoint<<" duh: "<<duh<<" eval_mat[items][weight-1]: "<<eval_mat[items][weight-1]<<" values[items]: "<<values[items];
				}
				//cout<<"\n -- Done with checkpoint-zone: "<<checkpoint_zone<<" weight: "<<weight<<endl;				
			}*/
		}	
		
		int items=num_items-1;
		int curr_weight=max_weight-1;
		final_value=0;
		while( (items>0) && ( curr_weight >0 ) )
		{
			if(eval_mat[items][curr_weight]!=eval_mat[items-1][curr_weight])
			{
				//printf("\n\t W: %d V: %d ",weight[iter],value[iter]);	
				cout<<"\n\t W "<<weights[items]<<" value[iter] "<<values[items];
				curr_weight-=weights[items];
				final_value=final_value+values[items];
				items=items-1;
			}
			else
				items=items-1;
		}	
		cout<<"\n\t Final-value: "<<final_value<<endl;
		
	}
	
	~Knapsack()
	{
		delete[] weights;
		delete[] values;
		for(int i=0;i<num_items;i++)
			delete[] eval_mat[i];
		delete[] eval_mat;
	}
	
	
	
};


