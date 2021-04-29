P2.out : Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P2.out Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project2.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h 
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

clean : 
	rm *.o P2.out 

submit : Project2.cpp LexicalAnalyzer.h SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt LexicalAnalyzer.cpp LexicalAnalyzer.h
	rm -rf zimmermanP2
	mkdir zimmermanP2
	cp Project2.cpp zimmermanP2
	cp LexicalAnalyzer.cpp zimmermanP2
	cp LexicalAnalyzer.h zimmermanP2
	cp SyntacticalAnalyzer.h zimmermanP2
	cp SyntacticalAnalyzer.cpp zimmermanP2
	cp makefile zimmermanP2
	cp README.txt zimmermanP2
	tar cfvz zimmermanP2.tgz zimmermanP2
	
