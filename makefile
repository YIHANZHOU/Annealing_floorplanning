run: zhou1298.cpp 
	g++ -std=c++11 zhou1298.cpp -o parser 
	./parser n50.fp -w
	python3 drawFP.py costfunction.txt n50-w


.PHONY:clean 
clean: 
	rm parser
	rm *.txt

