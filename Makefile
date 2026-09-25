portable:
	g++ -O3 -static ./src/main.cpp ./src/core/*.cpp ./src/utils/*.cpp -o gxpp

build:
	g++ -O2 ./src/main.cpp ./src/core/*.cpp ./src/utils/*.cpp -o gxpp

buildDev:
	g++ -O2 -g -fsanitize=address,undefined ./src/main.cpp ./src/core/*.cpp ./src/utils/*.cpp -o gxpp