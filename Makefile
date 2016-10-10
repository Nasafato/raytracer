all:
	g++ -g -Wall -std=c++11 -o scn2pov -I/usr/include/OpenEXR -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -L/usr/local/lib *.cc
