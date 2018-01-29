if [ -f "./app/wen/*.txt"];then
rm ./app/wen/*.txt
fi
	g++ -std=c++11 -c Application.cpp Task.cpp general.cpp Vertex.cpp main.cpp 
	g++ -g Application.o main.o general.o Vertex.o Task.o -o a.out
