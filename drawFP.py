#!/usr/bin/python3
import sys;
import matplotlib.pyplot as plt;
import matplotlib.patches as patches;

# importing the library
class block:
	def __init__(self, x, y, w, h):
		self.x = x; #x location of block corner
		self.y = y; #y location of block corner
		self.w = w; #width of block
		self.h = h; #height of block
def readcostfunction(filename):

    file=open(filename);
    costfunction=[];
    x=[];
    i=0;
    
    for line in file:
        x.append(i);
        costfunction.append(float(line));
        i=i+1;
    return (x,costfunction);

def readacceptmovement():
	file=open("rda.txt");
	accept=[]
	reject=[]
	x=[]
	i=0
	for line in file:
		line=line.rstrip();
		s=line.split(" ");
		x.append(i);
		accept.append(int(s[2]));
		reject.append(int(s[4]));
		#print(i,s[2],s[4])
		i=i+1
	
	return (x,accept,reject)



def plotcost(x,y,filename):
	
#	ax1 = plt.subplot(2,1,1)
#	ax2 = plt.subplot(2,1,2)
#	plt.sca(ax1)
	res=str(filename);
	if res[-1]=="c":
		plt.title("Cost for area + wire Length"+res)
	if res[-1]=="a":
		plt.title("Cost for area "+res)	
	if res[-1]=="w":
		plt.title("Cost for wire Length"+res)
	plt.xlabel("Step")
	plt.ylabel("Cost function")
	plt.plot(x, y, color ="green")
#	plt.sca(ax2)

#	plt.plot(x, accept, label="accept",color ="g")


	
	
	plt.savefig(str(res))
	plt.show()

	return
def plotaj(x,accept,reject,filename):
	fig,ax = plt.subplots()
	plt.title("Accept and Reject Times"+str(filename))
	plt.xlabel("Step")
	plt.ylabel("Time")
	#plt.plot(x,x)
	#plt.ylim(30);

	plt.plot(x,reject,color ="r")
	plt.plot(x, accept, label="accept",color ="g")
	plt.savefig("reject_and_accept"+str(filename)+'.png')
	#plt.grid(True)
	#plt.legend(loc='best')
	plt.show()	 

if __name__ == "__main__":
	#main program
#	(chipSize, blockArr) = readAnnealOut(sys.argv[1]);
#
#	drawFloorPlan(chipSize, blockArr);
	(x,y)=readcostfunction(sys.argv[1]);
	(x,accept,reject)=readacceptmovement();
	plotcost(x,y,sys.argv[2]);

	plotaj(x,accept,reject,sys.argv[2])
