buildStatic:
	g++ -O3 -static ./src/main.cpp ./src/core/*.cpp ./src/utils/*.cpp

buildDynamic:
	g++ -O2 ./src/main.cpp ./src/core/*.cpp ./src/utils/*.cpp