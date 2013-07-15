#include "error_inject_operator.h"

class Knapsack
{

	int num_items;
	int max_weight;
	int checkpoint_length;
	int num_checkpoints;
	int*  weights;
	int*	values;
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
		eval_mat=new error_inject_operators<int>*[num_items];
				
		for(int i=0;i<num_items;i++)
		{
			weights[i]=rand()%5;
			weights[i]=weights[i]+3;
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
		cout<<endl;
	}
	
	void fill_matrix()
	{
	
		cout<<"\n\t In fill-matrix() \n";
		for(int items=0;items<max_weight;items++)
			eval_mat[0][items]=0;

		for(int weight=0;weight<num_items;weight++)
			eval_mat[weight][0]=0;

		
		for(int items=1;items<num_items;items++)
		{
				error_inject_operators<int> b4_rollback_max;
				error_inject_operators<int> yet_to_rollback_prev_item;
				b4_rollback_max=0;
				yet_to_rollback_prev_item=1;		
		
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
							trace_flag=1;
						}
				
					}
					if( max_in_zone < eval_mat[items][weight] )
						max_in_zone=eval_mat[items][weight];					
					cout<<"\n\t\t I: "<<items<<" weight: " <<weight<<" weights[items]: "<<(weights[items])<<" trace-flag: "<<trace_flag<<" max_in_zone: "<<max_in_zone<<" eval_mat[items][weight]: "<<eval_mat[items][weight];
				}
				 //The or part takes care of cases when fault happens at last point of the checkpoint zone- The 2nd operand to and takes care of cases where the first item is being considered ( where its expected to have max_in_zone==values[items])
				duh=(eval_mat[items-1][weight-1-weights[items]]+values[items] ); // Weight should not be zero is the assumption :'(
		
				if( (max_in_zone!=eval_mat[items][weight-1] ) || ( ( !(max_in_zone - eval_mat[items][weight-1]) ) && ( (eval_mat[items][weight-1]!=duh  ) && (eval_mat[items-1][weight-1]!=eval_mat[items][weight-1]) ) ) )
				{
					
					cout<<"\n\t --- Item-#: "<<items<<" weight-iter: "<<weight<<"\t checkpoint_zone: "<<checkpoint_zone<<"\t weight: "<<weights[items]<<"\t eval_mat[items][weight] "<<eval_mat[items][weight-1]<<" max_in_zone: "<<max_in_zone<<" eval_mat[items-1][weight-1-weights[items]] "<< eval_mat[items-1][weight-1-weights[items]]<<" values[items] "<<values[items]<<" b4_rollback_max: "<<b4_rollback_max<<endl;
					if( (b4_rollback_max!=max_in_zone) )
					{
						checkpoint_zone--;					
						b4_rollback_max=max_in_zone;	
					}
					else
					{
						if( yet_to_rollback_prev_item==1 )
						{
							items--;
							error_inject_operators<int> local_max_in_zone;
							local_max_in_zone=0;
							for(weight-=checkpoint_length;weight<weights_bound;weight++)	
							{
								eval_mat[items][weight]=eval_mat[items-1][weight];
								int trace_flag=0;
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
									
									cout<<"\n\t\t\t ALERT I: "<<items<<" weight: " <<weight<<" weights[items]: "<<(weights[items])<<" trace-flag: "<<trace_flag<<" max_in_zone: "<<max_in_zone<<" eval_mat[items][weight]: "<<eval_mat[items][weight];				
								}
								if( local_max_in_zone < eval_mat[items][weight] )
									local_max_in_zone=eval_mat[items][weight];	
								
							}
							// Untested code of line : ---> if( local_max_in_zone == max_in_zone )	{items--;goto one}	
							// POTENTIALLY the above portion could cause error and this measure might not be upto no good!! :'(		
							
							if(local_max_in_zone>=max_in_zone)
							{
								checkpoint_zone=-1;								
								items--;
								yet_to_rollback_prev_item=1;							
								b4_rollback_max=0;
								cout<<"\n\t FATAL MOVE!!! local_max_in_zone: "<<local_max_in_zone<<" is equal to "<<max_in_zone<<endl;
							}
							else
							{
								items++;
								cout<<"\n\t ALERT!!! local_max_in_zone: "<<local_max_in_zone<<" is equal to "<<max_in_zone<<endl;								
							}
						}
						else
						{
							cout<<"\n\t ALERT!! not rolling back since b4_rollback_max=max_in_zone \n";
							b4_rollback_max=0;
							yet_to_rollback_prev_item=1;
						}
					}
				}
				else
				{
					//b4_rollback_max=0;
					cout<<"\n\t Item-#: "<<items<<" weight-iter: "<<weight<<"\t checkpoint_zone: "<<checkpoint_zone<<" max-in-zone: "<<max_in_zone<<" duh: "<<duh<<" eval_mat[items][weight-1]: "<<eval_mat[items][weight-1]<<" values[items]: "<<values[items];
				}
				//cout<<"\n -- Done with checkpoint-zone: "<<checkpoint_zone<<" weight: "<<weight<<endl;				
			}
		}	
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


