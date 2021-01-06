// FileIO.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>  // basic C++ input/output (e.g., cin, cout)
#include <fstream>	// needed to open files in C++
#include <sstream>   // needed if you are using sstream (C++)
#include <stdio.h>	// needed to open files in C
#include <string.h> // needed for C string processing (e.g., strstr)
#include<vector>   
#include <algorithm>
#include <string>
#include <map>
#include <stack>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <set>
#include <unordered_map>
#include <math.h>       /* exp */
#include <chrono> 

using namespace std;
using namespace std::chrono; 


int num_of_Net;
int num_of_Block;
ofstream outputfile;

//unordered_map<vector<string>,pair<float,float>>visited;
string outputresult;
int finalw=0;
int finalh=0;
struct block{
	int isoptimalchoice;
	int state; // 0 for nodeblock  1 for | vertical partitin 2 for -
	int w;
	int h;
	int posx;
	int posy;
	block * left_or_down;  //link to the subgraph that construct it.
	block * right_or_top; //
	block * parent;
//	vector<pair<int<x,y>>poses;
	
	block(int x,int y,int sta){
	    w=x;
		h=y;
		//sizes.push_back(make_pair(x,y));
		//poses.push_back(make_pair(0,0));
		state=sta;

	}
	bool operator()(const block& temp1,const block&temp2){
		return temp1.w<temp2.w;
	}
};
struct c_unique {
  int current;
  c_unique() {current=0;}
  int operator()() {return ++current;}
} UniqueNumber;

struct Net{
	vector<int>connects;
	int degree;
	Net(int x){
		degree=x;
	}
};
vector<vector<block*>>blocks;
vector<Net*>Nets;

int randomfunc(int j) 
{ 
    return rand() % j; 
}  
void normalized(vector<string>&polish,int pos=-1){
	if(pos==-1){
		for(int i=1;i<polish.size()-1;i++){
			if(polish[i]=="|"&&polish[i+1]=="|"){
				polish[i+1]="-";
			}
			if(polish[i]=="-"&&polish[i+1]=="-"){
				polish[i+1]="|";
			}
		}
	}
	else{
		if(polish[pos]=="|"&&polish[pos+1]=="|"){
				polish[pos]="-";
			}
			if(polish[pos]=="-"&&polish[pos+1]=="-"){
				polish[pos]="|";
			}
		
	}
}
bool isvalid(vector<string>&polish){
	int count=0;
	for(int i=0;i<polish.size();i++){
		if(polish[i]=="|"||polish[i]=="-"){
			if(count<2){
				return false;
			}
			count--;			
		}else{
			count++;
		}
	}
	return true;
}
void print_polish(vector<string>&polish){
   cout<<"polish:";
   for(string&a:polish){
		cout<<a;
	}
	//cout<<endl;
}
void assignwidth_height(block* root);
int parsefile(char *filename);
vector<string>generate_random();
pair<double,double> generate_polish(vector<string>&polish); 
vector<string> annealing(int option);
int main(int argc, char*argv[]){
	if(argc!=3){
		cout<<"not enough input";
		return -1;
	}
	if(parsefile(argv[1])==-1){
		printf("fail to parse the file");
		return -1;
	}
	outputresult=string(argv[1])+"_zhou_yihan_out2";
	int option;
    cout<<argv[2];
	if(strncmp(argv[2],"-a",2)==0){
		option=0;cout<<"optimize area"<<endl;
		}
	else if(strncmp(argv[2],"-w",2)==0)
	{option=1;cout<<"optimize wire hh"<<endl;}
	else if(strncmp(argv[2],"-c",2)==0){option=2;cout<<"optimize both"<<endl;}
	else{
		cout<<"Invalid option, must be -a -w -c"<<endl;
		exit(0);
	}
	auto start = high_resolution_clock::now(); 

	auto polish=annealing(option);
	auto stop = high_resolution_clock::now(); 
	auto duration = duration_cast<microseconds>(stop - start); 
     outputfile<< "Time taken by function: "
         << duration.count() << " microseconds" << endl;
	
	auto output=generate_polish(polish);
	//outputfile<<" Final polish expression "<<polish<<endl;
	outputfile<<"  HPWL: "<<output.second<<endl;
	outputfile<<" Width: "<<finalw<<" Height: "<<finalh<<" Area: "<< output.first<<endl;
	for(int i=0;i<num_of_Block;i++){
	  for(auto&s : blocks[i]){
		  if(s->isoptimalchoice==1){
	  outputfile<<i<<" "<<s->posx<<" "<<s->posy<<" "<<s->w<<" "<<s->h<<endl;
		  }
	  }
	}
     outputfile.close();
	//vgenerate_random(argv[1]);
	//generate_polish(polish);

	//	return -1;
	//	}

	

	// myfile<<"Width:  "<<finalblock[optimal]->w<<"   Height:  "<<finalblock[optimal]->h<<endl;
	// myfile<<"The area of the whole floorplan  "<<finalblock[optimal]->w*finalblock[optimal]->h<<endl;

//	assignwidth_height(finalblock[optimal]);
	
	return 0;
	
}


int Move(vector<string>&polish){
	
	int option=rand()%3;
	int flag=0;
	float cost=0;
	pair<float,float>result;

	//cout<<flag;
//	if(option==0){  // OP1
	while(!flag){
		
		option=rand()%3;
		if(option==0){// OP1 means swap 2 adject (no number in between) 
		//cout<<"move1"<<endl;

		int random=rand()%(polish.size()-2); //number
		while(polish[random]=="-"||polish[random]=="|"){ //not a number; 
		random=(random+1)%polish.size();   //  in this way , the possiblity of finding a number is 1 but does not make sure this number has a neighbor to change
		}
		int j=random+1;
		while(j<polish.size()-2){
				if(polish[j]!="|"&&polish[j]!="-"){ //number;
				swap(polish[random],polish[j]);
				flag=1;
				break;
				}
				j++;
			}
		}
		if(option==1){
		//	cout<<"move2"<<endl;
          //  int range=(polish.size()-2)+2;//from 2 to n
			int random=rand()%polish.size();// 
			while(polish[random]!="-"&&polish[random]!="|"){ //is not a operator, then find an operator; 
		     random=(random+1)%polish.size();   //  in this way , the possiblity of finding a operator is 1  but does not make sure this operation is just betwwen two number
		    }
			string temp=polish[random];
			string temp2=temp=="-"?"|":"-";
			if(polish[random-1]!="|"&&polish[random-1]!="-"&&(random+1==polish.size()||(polish[random+1]!="|"&&polish[random+1]!="-"))){ //into a valid operator to change 
			   polish[random]=temp2;
			   flag=1;
				break;
			}	
		}
		if(option==2){
		//	cout<<"move3"<<endl;
			int random=rand()%(polish.size()-2)+1; // from 1 to n-2
			//cout<<random;
			if(polish[random]!= "|"&&polish[random]!="-"){// an integer
				if(polish[random+1]=="|"||polish[random+1]=="-"){// right not an interger
					swap(polish[random],polish[random+1]);

					if(isvalid(polish)){ // a valid expression
					    if(polish[random-1]==polish[random]){
							polish[random]=polish[random]=="|"?"-":"|";
						}
                        flag=1;
				//		result=generate_polish(polish);
						break;
					}
					swap(polish[random],polish[random+1]);

					}
				if(polish[random-1]=="|"||polish[random-1]=="-"){ //left not a interger
					swap(polish[random],polish[random-1]);
					//result=generate_polish(polish);
					if(isvalid(polish)){
					 if(polish[random+1]==polish[random]){
							polish[random]=polish[random]=="|"?"-":"|";
					  }
                    flag=1;
						break;
					}
					swap(polish[random],polish[random-1]);
				}
					
			}else{  //random  is an operator
				if(polish[random+1]!="|"&&polish[random+1]!="-"){//a number 
					swap(polish[random],polish[random+1]);//
					//result=generate_polish(polish);
					if(isvalid(polish)){
					   if(random+2!=polish.size()&&polish[random+1]==polish[random+2]){
							polish[random+1]=polish[random+1]=="|"?"-":"|";
					     }
                        flag=1;
						break;
					}
					swap(polish[random],polish[random+1]);
						
					}
				if(polish[random-1]!="|"&&polish[random-1]!="-"){ // a number
					swap(polish[random],polish[random-1]);
					//result=generate_polish(polish);
					if(isvalid(polish)){
					if(random-2!=-1&&polish[random-1]==polish[random-2]){   
							polish[random-1]=polish[random-1]=="|"?"-":"|"; // nomalize it 
					     }
                        flag=1;
						break;
					}
					swap(polish[random],polish[random-1]);
				}

			} 

				
			}


		}
		//print_polish(polish);
		return option;
				
//	return generate_polish(polish);

	
}
 
vector<string>generate_random(){
	//	string Mystring(fName);
//	Mystring.erase(remove(Mystring.begin(), Mystring.end(), '.'), Mystring.end());
//	string file= Mystring+"_zhou1298_yihan_zhou.out1";
//	ofstream myfile(file);
	vector<string>polish(num_of_Block*2-1,"0");
	for(int i=0;i<num_of_Block;i++){
		polish[i]=to_string(i);
	}
	 for(int i=num_of_Block;i<num_of_Block*2-1;i++){
	//	polish+=(rand()%10>5)?'|':'-';       //generate a random number between 0-1
	     polish[i]=i%2==0?'|':'-'; 
	// 	//polish+='|';       //generate a random number between 0-1
	}
	//cout<<polish<<endl;
	// for(int i=num_of_Block;i<3*num_of_Block/2;i++){
	// //	polish+=(rand()%10>5)?'|':'-';       //generate a random number between 0-1
	//     polish[i]="-";
	// 	//polish+='|';       //generate a random number between 0-1
	// }
	// for(int i=3*num_of_Block/2;i<num_of_Block*2-1;i++){
	// //	polish+=(rand()%10>5)?'|':'-';       //generate a random number between 0-1
	//     polish[i]="|";
	// 	//polish+='|';       //generate a random number between 0-1
	// }
	volatile int flag=0;
	while(true){
	srand(time(0));
	random_shuffle(polish.begin(),polish.end()-2,randomfunc); //(no need to change last 2 operators)

	//print_polish(polish);
	if(isvalid(polish)){
		break;
	}
	}
	 normalized(polish,-1);
	// //print_polish(polish);
	//	polish="3742659018-|-|-|-|-";
	// for(string &a:polish){
	// 	cout<<a;
	// 	myfile<<a;
	// }

//	cout<<endl;
	return polish;
//	myfile<<endl;
}
bool accept_move(double diff,double T,double k){
	if(diff<0){
		return true;
	}else{
		double boltz=exp(-diff/(k*T));
		//cout<<"boltz"<<boltz;
        double r = ((double) rand() / (RAND_MAX));
		//cout<<"r"<<r;
		if(r<boltz){
			return true;
		}

	}
	return false;

}

// set<vector<string>>isvisited;
vector<string>annealing(int opt){
	vector<string>polish=generate_random();
	pair<float,float>cost=generate_polish(polish);
	print_polish(polish);
	int num_of_move_per_timestamp=num_of_Block;
	ofstream myfile("rda.txt");
	ofstream costfunction("costfunction.txt");
	
	cout<<"Initial Cost:"<<cost.first+cost.second<<"  block:"<<cost.first<<" WILE"<<cost.second<<endl;
	//costfunction<<cost.first<<endl;
	int timestamp=0;
	int k=40;
	double diffsum=0;
	double T=40000;
	double T_frezz=0.098;
	double coolingfactor=0.98;
	vector<int>moves(3,0);


	if(opt==0){
		outputfile.open(outputresult+'a');
		cout<<"optimal area"<<opt<<endl;
	//	float new_T=cost.first;
		double curent_T=cost.first;
		//auto initial_polish=polish;

		for(int i=0;i<50;i++){
			Move(polish);
			double temp=generate_polish(polish).first;
			diffsum+=max(temp-curent_T,curent_T-temp);
			curent_T=temp;
		}
		double avgcost=diffsum/50;
		k=avgcost/(T*log(1.0/0.98));
		cout<<"value of k"<<k<<" "<<curent_T;
	
	while(T>T_frezz){
	    int rejectmove=0;
        int acceptmove=0;
		double last_step=curent_T;
		for(int i=0;i<num_of_move_per_timestamp;i++){
		   vector<string>polish_last=polish;
	       int moveoption=Move(polish);
		  // print_polish(polish);
		   auto new_cost=generate_polish(polish);
		   double cost_diff=new_cost.first-curent_T;
		   diffsum+=cost_diff;
		   //cout<<"cost different:"<<cost_diff<<endl;
		   if(!accept_move(cost_diff,T,k)){
			   //cout<<"reject this move"<<endl;
			   polish=polish_last;
			   rejectmove++;
			   moves[moveoption]++;

		   }else{			
	//		print_polish(polish);
	 //       cout<<"Cost_T:"<<curent_T<<" Cost_new_block:"<<new_cost.first<<"  WILE"<<new_cost.second<<"cost different:"<<cost_diff<<endl;
			curent_T=new_cost.first;
		    acceptmove++;
			moves[moveoption]++;
			
		   }
		   diffsum+=cost_diff;

		}
		myfile<<timestamp<<" accept_move: "<<acceptmove<<" reject_move: "<<rejectmove<<endl;	
		costfunction<<curent_T<<endl;
		timestamp++;
		T*=coolingfactor;
	    // if(num_of_move_per_timestamp>100){
		// T-=max(0.0,(last_step-curent_T)/last_step)*T;
		// }
		
	}
	cout<<"Cost_T:"<<curent_T<<" Cost_new_block:"<<generate_polish(polish).first<<"  WILE"<<generate_polish(polish).second<<endl;

	}else if(opt==1){
	outputfile.open(outputresult+'w');
	double curent_T=cost.second;
	
	for(int i=0;i<50;i++){
			Move(polish);
			double temp=generate_polish(polish).second;
			diffsum+=max(temp-curent_T,curent_T-temp);
			curent_T=temp;
		}
		double avgcost=diffsum/50;
		k=avgcost/(T*log(1.0/0.98));
		cout<<k;
	
	while(T>T_frezz){
	    int rejectmove=0;
        int acceptmove=0;
		double last_step=curent_T;


		for(int i=0;i<num_of_move_per_timestamp;i++){
		   vector<string>polish_last=polish;
	       int moveoption=Move(polish);
		  // print_polish(polish);
		   auto new_cost=generate_polish(polish);
		   double cost_diff=new_cost.second-curent_T;
		   //cout<<"cost different:"<<cost_diff<<endl;
		   if(!accept_move(cost_diff,T,k)){
			   //cout<<"reject this move"<<endl;
			   polish=polish_last;
			   rejectmove++;
			   
		   }else{			
		//	print_polish(polish);
	      //  cout<<"Cost_T:"<<curent_T<<" Cost_new_block:"<<new_cost.first<<"  WILE"<<new_cost.second<<"cost different:"<<cost_diff<<endl;
			curent_T=new_cost.second;
		    acceptmove++;
			moves[moveoption]++;

		   }
		   diffsum+=cost_diff;

		}
		myfile<<timestamp<<" accept_move: "<<acceptmove<<" reject_move: "<<rejectmove<<endl;
		costfunction<<curent_T<<endl;
		//T=(0.98-max(0.0,last_step-curent_T)/last_step)*T;
		T*=coolingfactor;
		// if(num_of_move_per_timestamp>100){
		// T-=max(0.0,(last_step-curent_T)/last_step)*T;
		// }
		timestamp++;
	}
	cout<<"Cost_T:"<<curent_T<<" Cost_new_block:"<<generate_polish(polish).first<<"  WILE"<<generate_polish(polish).second<<endl;
	}if(opt==2){
	outputfile.open(outputresult+'c');
	double curent_T=cost.first+cost.second;
	int portional=cost.first/cost.second/2;
	auto initial_polish=polish;
	for(int i=0;i<50;i++){
			Move(polish);
			auto temp1=generate_polish(polish);
			double temp=temp1.first+temp1.second;
			diffsum+=max(temp-curent_T,curent_T-temp);
			curent_T=temp;
		}
		double avgcost=diffsum/50;
		k=avgcost/(T*log(1.0/0.98));
		cout<<k;
	polish=initial_polish;
	while(T>T_frezz){
	  
	    int rejectmove=0;
        int acceptmove=0;
		double last_step=curent_T;

		for(int i=0;i<num_of_move_per_timestamp;i++){
		   vector<string>polish_last=polish;
	       int moveoption=Move(polish);
		  // print_polish(polish);
		   auto new_cost=generate_polish(polish);
		   double cost_diff=new_cost.first+new_cost.second-curent_T;
		   //cout<<"cost different:"<<cost_diff<<endl;
		   if(!accept_move(cost_diff,T,k)){
			   //cout<<"reject this move"<<endl;
			   polish=polish_last;
			   rejectmove++;
			   
		   }else{			
		//	print_polish(polish);
	     //   cout<<"Cost_T:"<<curent_T<<" Cost_new_block:"<<new_cost.first<<"  WILE"<<new_cost.second<<"cost different:"<<cost_diff<<endl;
			curent_T=new_cost.first+new_cost.second;
			moves[moveoption]++;
		    acceptmove++;
		   }
		   diffsum+=cost_diff;
		   
		}
		myfile<<timestamp<<" accept_move: "<<acceptmove<<" reject_move: "<<rejectmove<<endl;
		costfunction<<curent_T<<endl;
		timestamp++;
		//T=(0.98-max(0.0,last_step-curent_T)/last_step)*T;
		T*=coolingfactor;
		// if(num_of_move_per_timestamp>100){
		// T-=max(0.0,(last_step-curent_T)/last_step)*T;
		// }
	}
//	print_polish(polish);
	cout<<"Cost_T:"<<curent_T<<" Cost_new_block:"<<generate_polish(polish).first<<"  WILE"<<generate_polish(polish).second<<endl;
	}
	cout<<"k"<<k;
	cout<<"options"<<moves[0]<<" "<<moves[1]<<" "<<moves[2];
	//print_polish(polish); 
	//outputfile.close();
	//cout<<num_of_Block;
	return polish;


}



pair<double,double> generate_polish(vector<string>&polish){

	stack<vector<block*>>st;
	for(int i=0;i<polish.size();i++){
		if(polish[i]!="-"&&polish[i]!="|"){// must be a integer
			int num=stoi(polish[i]);

			//vector<block*>temp;
			//temp.push_back(blocks[i]);
			for(auto &s:blocks[num]){
				s->isoptimalchoice=0;
			}
			st.push(blocks[num]);
			continue;
		}
		//cout<<i<<" "<<polish[i]<<endl;

		if(polish[i]=="|"){
		if(st.empty()){
			cout<<i<<polish[i]<<endl;
			return make_pair(-1,-1);
		}
		vector<block*> temp2=st.top();
		st.pop();
		if(st.empty()){
						cout<<i<<polish[i]<<endl;

			return make_pair(-1,-1);
		}
		vector<block*> temp1=st.top();
		st.pop();
		vector<block*>res;
		int ii=0;int jj=0;
	//	cout<<temp1.size();
	//	cout<<temp2.size();
		while(ii<temp1.size()&&jj<temp2.size()){
			//cout<<ii<<" "<<jj<<" "<<temp1[ii]->w<<" "<<temp2[jj]->w<<endl;

			int ck=temp1[ii]->w+temp2[jj]->w;
			int dk=max(temp1[ii]->h,temp2[jj]->h);
			block*superblock=new block(ck,dk,1);
			superblock->left_or_down=temp1[ii];
			superblock->right_or_top=temp2[jj];
		//	cout<<superblock->w<<" "<<superblock->h<<endl;
			res.push_back(superblock);
			if(dk==temp1[ii]->h){
				ii=ii+1;
			}else{
				jj=jj+1;
			}			
		}
		
		st.push(res);
		}
		if(polish[i]=="-"){
		if(st.empty()){
						cout<<i<<polish[i]<<endl;

			return make_pair(-1,-1);
		}
		vector<block*> temp1=st.top(); //bottom
		st.pop();
		if(st.empty()){
			
			cout<<i<<polish[i]<<endl;

			return make_pair(-1,-1);
		}
		vector<block*> temp2=st.top(); //top
		st.pop();
		vector<block*>res;
		int ii=temp1.size()-1;int jj=temp2.size()-1;
	//	cout<<temp1.size();
	//	cout<<temp2.size();
		while(ii>-1&&jj>-1){
		//	cout<<ii<<" "<<jj<<" "<<temp1[ii]->w<<" "<<temp2[jj]->w<<endl;

		//	cout<<temp1[ii]->w<<" "<<temp2[jj]->w;
			int ck=temp1[ii]->h+temp2[jj]->h;
			int dk=max(temp1[ii]->w,temp2[jj]->w);
			block*superblock=new block(dk,ck,2);
			superblock->left_or_down=temp1[ii];
			superblock->right_or_top=temp2[jj];
			temp1[ii]->parent=superblock;
			temp2[jj]->parent=superblock;
			//cout<<superblock->w<<" "<<superblock->h<<endl;

			res.push_back(superblock);
			if(dk==temp2[jj]->w){
			//	cout<<"temp2 larger"<<temp2[jj]->w;
				jj=jj-1;
			}else{
				ii=ii-1;
			}			
		}
		reverse(res.begin(),res.end());
		st.push(res);

		}		
	}
	if(st.empty()){
		cout<<"empty final blcok"<<endl;
	}
	vector<block*> finalblock=st.top();
	int res=finalblock[0]->w*finalblock[0]->h;
	int optimal=0;
	for(int i=0;i<finalblock.size();i++){
	//	cout<<finalblock[i]->w<<" "<<finalblock[i]->h<<endl;
		if(finalblock[i]->w*finalblock[i]->h<res){
          optimal=i;
		}
	}
	// myfile<<"Width:  "<<finalblock[optimal]->w<<"   Height:  "<<finalblock[optimal]->h<<endl;
	// myfile<<"The area of the whole floorplan  "<<finalblock[optimal]->w*finalblock[optimal]->h<<endl;
	finalw=finalblock[optimal]->w;
	finalh=finalblock[optimal]->h;

	finalblock[optimal]->posx=0;
	finalblock[optimal]->posy=0;
	assignwidth_height(finalblock[optimal]);
	double HPWL=0;
	for(int i=0;i<num_of_Net;i++){
		Net* temp=Nets[i];
		vector<double>elements_x(temp->degree,0);
		vector<double>elements_y(temp->degree,0);
		for(int j:temp->connects){
			for(auto&s:blocks[j]){
				if(s->isoptimalchoice){
					elements_x.push_back(s->posx+s->w/2);
					elements_y.push_back(s->posy+s->h/2);
					break;
				}
			}
		}
		double minx=*min_element(elements_x.begin(),elements_x.end());
		double maxx=*max_element(elements_x.begin(),elements_x.end());
		double miny=*min_element(elements_y.begin(),elements_y.end());
		double maxy=*min_element(elements_y.begin(),elements_y.end());
        HPWL+=maxx-minx+maxy-miny;
	}
//	float cost=finalblock[optimal]->w*finalblock[optimal]->h+HPWL;
    return make_pair(finalblock[optimal]->w*finalblock[optimal]->h,HPWL);
	// for(int i=0;i<num_of_Block;i++){
	
	//   for(auto&s : blocks[i]){
	// 	  if(s->isoptimalchoice==1){
	//   myfile<<i<<" "<<s->posx<<" "<<s->posy<<" "<<s->w<<" "<<s->h<<endl;
	// 	  }
	//   }

	// }




	

	
}


void assignwidth_height(block* root){
	if(!root){
		return;
	}
	// if(root->state==0){

	// }
	root->isoptimalchoice=1;
	if(root->state==2){//'-'{
		root->left_or_down->posx=root->posx;
		root->left_or_down->posy=root->posy;

		root->right_or_top->posx=root->posx;
		root->right_or_top->posy=root->posy+root->left_or_down->h;


		assignwidth_height(root->left_or_down);
		assignwidth_height(root->right_or_top);

	}else if(root->state==1){//'|'
	    root->left_or_down->posx=root->posx;
		root->left_or_down->posy=root->posy;

		root->right_or_top->posx=root->posx+root->left_or_down->w;
		root->right_or_top->posy=root->posy;
		assignwidth_height(root->left_or_down);
		assignwidth_height(root->right_or_top);
		

	}
}


int parsefile(char *fName){
	char lineBuf[1024];
	//vecto
	ifstream ifs(fName);

	if (ifs.is_open() == 0){ // or we could say if (!ifs)
		cout << "Error opening file " << fName << endl;
		return -1;
	}
    ifs.getline(lineBuf, 1023);
	num_of_Block=stoi(string(lineBuf)); // convert to C++ string
	blocks.resize(num_of_Block);
	int a;int b;int temp;
	for(int i=0;i<num_of_Block;i++){
	   ifs.getline(lineBuf, 1023);
	    string lineStr(lineBuf); // convert to C++ string
	     istringstream iss(lineStr);
	    iss>>temp>>a>>b;	// read one line
		int large=max(a,b); int small=min(a,b);
		//blocks[temp]=vector<block*>(;
		vector<block*>aaa;
        aaa.push_back(new block(small,large,0)); // first block always has larger height
		aaa.push_back(new block(large,small,0));

		blocks[i]=aaa;

		//blocks[temp]=new block(b,a,0);
	//	cout<<blocks[i]->w<<blocks[i]->h<<endl;
	}
    ifs.getline(lineBuf, 1023);
    ifs.getline(lineBuf, 1023);
	num_of_Net=stoi(string(lineBuf)); // convert to C++ string
     //cout<<num_of_Net;
	Nets.resize(num_of_Net);
	for(int i=0;i<num_of_Net;i++){
	   ifs.getline(lineBuf, 1023);
	    string lineStr(lineBuf); // convert to C++ string
	    istringstream iss(lineStr);
	    iss>>temp;	// read one line
		//cout<<temp<<" ";
		Nets[i]=new Net(temp);
		for(int j=0;j<temp;j++){
			iss>>a;
			Nets[i]->connects.push_back(a);
	//	cout<<a<<" ";
		}
	//	cout<<endl;
	}
return 0;	
}

// void generate_file(char *fName){
// 	// string Mystring(fName);
// 	// string file= Mystring+"_zhou1298_yihan_zhou.out1";
// 	// ofstream myfile(file);
// 	// for(int i=0;i<num_of_Block;i++){
	
// 	//   for(auto&s : blocks[i]){
// 	// 	  if(s->isoptimalchoice==1){
// 	//   myfile<<i<<" "<<s->posx<<" "<<s->posy<<endl;
// 	// 	  }
// 	//   }

// 	// }

	
	
// }
