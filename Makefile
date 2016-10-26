all:
	g++ -g -Wall -o prog_out -I/usr/include/OpenEXR -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -L/usr/local/lib *.cc

test:
	./prog_out scenes/scene_1.scn output_test/scene_1-test.exr; \
	./prog_out scenes/scene_2.scn output_test/scene_2-test.exr; \
	./prog_out scenes/scene_3.scn output_test/scene_3-test.exr; \
	./prog_out scenes/scene_4.scn output_test/scene_4-test.exr; \
	./prog_out scenes/scene_5.scn output_test/scene_5-test.exr; \
	./prog_out scenes/scene_7.scn output_test/scene_7-test.exr; \
	./prog_out scenes/scene_8.scn output_test/scene_8-test.exr;
