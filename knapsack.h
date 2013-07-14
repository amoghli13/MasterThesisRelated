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
			cout<<"\n \n";
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
		
			for(int checkpoint_zone=0,weight=0;checkpoint_zone<num_checkpoints;checkpoint_zone++)
			{
				//cout<<"\n -- In Checkpoint-zone: "<<checkpoint_zone<<" weight: "<<weight<<endl;
				error_inject_operators<int> max_in_zone;
				max_in_zone=0;				
				int weights_bound= min( (weight+checkpoint_length),max_weight );
				for( ;weight<weights_bound;weight++)
				{
					int trace_flag=0;						
					eval_mat[items][weight]=eval_mat[items-1][weight];

					if( (weight- weights[items]) >=0 )
					{
						int duh=(eval_mat[items-1][weight-weights[items]]+values[items] );
						if(  eval_mat[items][weight] > ( duh)  )
							eval_mat[items][weight]=eval_mat[items][weight];
						else
						{
							eval_mat[items][weight]=duh;
							max_in_zone=eval_mat[items][weight] ;
							trace_flag=1;
						}
						if( max_in_zone < eval_mat[items][weight] )
							max_in_zone=eval_mat[items][weight];
						
					}
					//cout<<"\n\t I: "<<items<<" weight: " <<weight<<" weights[items]: "<<(weights[items])<<" trace-flag: "<<trace_flag;
				}
				if(max_in_zone!=eval_mat[items][weight-1])
					cout<<"\n\t --- Item-#: "<<items<<" weight-iter: "<<weight<<"\t checkpoint_zone: "<<checkpoint_zone<<"\t weight: "<<weights[items]<<"\t eval_mat[items][weight] "<<eval_mat[items][weight-1]<<" max_in_zone: "<<max_in_zone<<endl;
				else
					cout<<"\n\t Item-#: "<<items<<" weight-iter: "<<weight<<"\t checkpoint_zone: "<<checkpoint_zone<<" max-in-zone: "<<max_in_zone;
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


