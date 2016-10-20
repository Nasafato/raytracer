all:
	g++ -g -Wall -o scn2pov -I/usr/include/OpenEXR -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -L/usr/local/lib *.cc

test:
	./scn2pov scenes/scene_1.scn output4/scene_1-test.exr; \
	./scn2pov scenes/scene_2.scn output4/scene_2-test.exr; \
	./scn2pov scenes/scene_3.scn output4/scene_3-test.exr; \
	./scn2pov scenes/scene_4.scn output4/scene_4-test.exr; \
	./scn2pov scenes/scene_5.scn output4/scene_5-test.exr; \
	./scn2pov scenes/scene_7.scn output4/scene_7-test.exr; 
