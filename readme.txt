1. run and extcute
	./parser n300.fp -a  // will generate rda.txt for accepet and reject times
	python3 drawFP.py costfunction.txt n300-a  // will plot picture of cost function and reject times and store a png file with finame  "-a"  "-c" "-w" in the end

2. Stratetries

2. Initialize k based on average absolute cost from first 50 steps.
3. I made a little change to the cooling schedule so that if generate a move decrease a lot of cost function, it would 

3. Implementation
1. To make initial polish expression more random, Generate_random() would shuffle the polish expression from index 2 to end -2 and isvalid() would check will this expression is valid. and Nomalize would nomalize it.

Move(vector<string>&polish) generate a new polish expression by equal probablity for the 3 kind of moves, and then generate a random number to decide which number/operator need to change, if the random index does not point to 
a wanted number or operator, it would iterate until it find one, in this way, making possibility of the decision towards 3 moves as even as possible.
Which has been detailed described in cpp file.
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



2. Annealing() will first call generate_random() iniliaze a random polish and then find k and then do movement for num_of_move_per_timestamp in each timestep. Which is the same for its number of gates.
 compare random value with boltze cost function to decide whether to reject or accpet and record the numbers.
Follow from Phase 1
1. The basic structure for each block constains its state(0 represent no lower level block contains, 1 means this block can be vertical partiontioned and 2 means this block can be horizontally partitioned)
And there will be pointer for each block with state 0 or 1 , means its a super block for some lower level blocks.
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

2. Parsefile(char*fName)
when parsing the file, construct a list of blocks two blocks with swapping width. and blocks[num_of_blocks] is a global variable, a list of block list stores the information of every most basic blocks parsing from the given file. 
vector<vector<block*>>blocks;
int large=max(a,b); int small=min(a,b);
vector<block*>aaa;
aaa.push_back(new block(small,large,0)); // first block always has larger height
aaa.push_back(new block(large,small,0));
blocks[i]=aaa;

3. generate_random();
First generate a random polish expression, a vector list of string by random_shuffle(polish.begin(),polish.begin()+num_of_Block,randomfunc) to generate random number sequence.
Then using construct a bigger block using bottom up way. 
stack<vector<block*>>st;
Put the list of blocks corresponds to the list of blocks in blocks[number] in the stack, every time there is a '-'or '|', pop up the last two numbers in the stack, finding the minimun combinations of these blocks and put the lists of combinations in the stack.
Finally only the stack only has one element, storing every possible combinations of final block, find the block and using left_or_down and right_or_top to trace back its child.
use a recursion to find the position of each block.
void assignwidth_height(block* root){
	if(!root){
		return;
	}
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







