portable:
	g++ -O3 -static ./src/main.cpp ./src/core/*.cpp ./src/utils/*.cpp -o gxf

build:
	g++ -O2 ./src/main.cpp ./src/core/*.cpp ./src/utils/*.cpp -o gxf