/* Round-Robin scheduling algorithm simulation
 * Tsegai Ghebremedhin
 * Jenci Chavez
 * _____________________________________________
 *       |         Overhead time in ms         |
 * ______|_____________________________________|
 * TQ    |    |       |      |     |     |     |
 * ______|__0_|_0.005_|_0.01 | 0.15| 0.2 | 0.25| 
 * 0.05  |    |       |      |     |     |     |
 * ______|__1_|____2__|___3__|___4_|__5__|_6___|
 * 0.1   |    |       |      |     |     |     |
 * ______|__7_|___8___|___9 _|__10_|_11__|_12__|
 * 0.25  |    |       |      |     |     |     |
 * ______|_13 |____14 |___15_|__16_|__17_|__18_|
 * 0.5   |    |       |      |     |     |     |
 * ______|_19_|__20 __|__21__|_22__|__23_|__24_|
 * 
 * Use this table for each run.Start with time_quantum=0.05 and over_head = 0
 * Each run will give you the waiting time of a process
 * Then you find the average waiting time for each process for each of the 24 runs
 * Example: Average waiting time P0 = waiting time in first run + waiting time in second run+....+waiting time in 24th run / 24
 * I think you have the project file so you know how to write the report.
 * Orgulloso de ti Jenci!!!
 * 
 *  
 * */

#include<iostream>
#include<fstream>
#include<list>
#include<queue>

using namespace std;

const int process_cnt=22;


struct process{

      int at;
      float bt;
      float tat;
      float st;
      float wt;
      int pid;
      int stflag;
      };
 //ready list     
list<process> ready_q;
 //initial read queue
queue<process> init_q;

process finished[process_cnt];

float curr_time=0.0f;
unsigned int max_q_len=0;
float avg_q_len=0.0f;

void read_file_init(ifstream& inData);
void check_arrival();
void RR(unsigned int & max_q_len,float & avg_q_len);
void print_results();

int main(){

    ifstream inData;
    //read module
    read_file_init(inData);
    //call to simulator module
    RR (max_q_len,avg_q_len);
    //call to print results after simulation ends

    print_results();
return 0;
}

void read_file_init(ifstream& inData){

    int count=0; //used for pid
    inData.open("input-full.txt");
    while(true){
         process entry ;
         inData >>entry.at>>entry.bt;
         //initialization
         entry.st=0.0f; 
         entry.tat=0.0f;
         entry.wt=0.0f; 
         entry.stflag=0; 
         entry.pid=count;
         count++;
         if( inData.eof() ) break;
         //push current item to initial_queue
         init_q.push(entry);
         //cout<<"Read Successful in read_file"<<endl;
        
	} 
	inData.close();

}
void print_results(){
	     
	float Atat=0.0f,Awt=0.0f;
	for(int i=0;i<process_cnt;i++){
		//calculate the Average Turn Around Time 
	    Atat+=finished[i].tat;
	    //calculate the Waiting time for each process using the start time and arrival time
	  	finished[i].wt=finished[i].st-finished[i].at;
	  	cout<<"Waiting time of process "<<finished[i].pid<<" is "<<finished[i].wt<<endl;
	  	//calculate the Average waiting time
		Awt+=finished[i].wt;
	}
	cout<<"Total length of the simulation is "<<curr_time<<" s"<<endl;
	cout<<"The average turn-around-time is :"<<float(Atat)/float(process_cnt)<<" s"<<endl;
	cout<<"The average waiting-time is :"<<float(Awt)/float(process_cnt)<<" s"<<endl;
	cout<<"Average queue length is :"<<avg_q_len<<endl;
	cout<<"The maximum queue length is :"<<max_q_len<<endl;
	
}

//module to check for events(new arrivals)
void check_arrival(){
	
	while(!init_q.empty()&& float(init_q.front().at)<=float(curr_time)){
		process pro= init_q.front();
		//cout<<"Current Time here is "<<curr_time<<endl;
		init_q.pop();
		//Process enters the ready list
		ready_q.push_back(pro);		
		}
}
//Simulator module    
void RR (unsigned int &max_q_len,float & avg_q_len){
	
	float time_q;
	cout<<"Please enter time quantum"<<endl;
	cin>>time_q;
	
	float over_hd;
	cout<<"Please enter overhead time"<<endl;
	cin>>over_hd;
 
    int counter=0;  //counter for finished array  
    float lp_counter=0; //for loop number 
    float q_size;//for queue size in each loop of the simulator
    
    //pointer to go through the circular list
    list<process>::iterator it1;
	
	
	while(!init_q.empty()||!ready_q.empty()){//scheduler begins	
	    //Check for event(new process arrives)
        check_arrival();
        //Initialize pointer to go through the ready list
        it1 = ready_q.begin();
	
 	    while(!ready_q.empty()){
			q_size+=ready_q.size();
			lp_counter++;
			if (ready_q.size()>max_q_len)
			    //maximum queue length
			    max_q_len = ready_q.size();
		    if(ready_q.size()==1)
 	    	       it1 = ready_q.begin();
 	    	else{
				//wrap around the iterator
 	    	     if(it1==ready_q.end())
		           it1==ready_q.begin();}
		    //Mark the start time of a process
		    if(it1->st==0&&it1->stflag==0) {
				it1->st=curr_time;
				it1->stflag=1;
		       } 
		    //Process can finish in one full or less than full time_quantum
		    if(it1->bt>0.0f && it1->bt<=time_q){ 
				//current time is updated adding the service time of the process  
		    	curr_time+=it1->bt;
		    	//TAT of the process is calculated by subtracting the process arrival time fromt the current time
		    	it1->tat=curr_time-it1->at; 
		    	//Context switch
		    	curr_time+=over_hd;  
		    	it1->bt=0.0f;  
		    	//cout<<"End time of process "<<it1->pid<<" is "<<curr_time<<" and loop count is " <<lp_counter<<endl;
		    	//lp_counter=0; 
		    	//copy process to finished array
		    	finished[counter]=*it1;
		    	//remove process from ready list
		    	it1 = ready_q.erase(it1); 
		    	//keeps track of next space in finished array                
			    counter++;			     
		    }
	   	    else
	   	    {//Process needs more CPU time
				//service process				
		     	it1->bt-=time_q;
		     	//update system time
			    curr_time+=time_q;
			    //check for events(new arrivals)
			    check_arrival();
			    //context switch	
			    curr_time+=over_hd;	
			    //advance pointer			    
			    ++it1;
			    //for average q length
			    ++lp_counter;
		    } 	
	    }
	    //No more events to process
	    if(ready_q.empty()){
			  if(!init_q.empty())
			    //CPU idle(increase current time until new processes arrive) 
	            curr_time+=0.001f; 
                //cout<<"Curr time now "<<curr_time<<endl;  
      }
  }  
     //will use this for ave_q
     //cout<<"lp"<<q_size<<endl;
     //cout<<"lp_counter "<<lp_counter<<endl;
     
     avg_q_len=float(q_size/lp_counter);
     cout<<"Simulator Ends "<<endl;
      
	}
