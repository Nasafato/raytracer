all:
	g++ -g -Wall -o prog_out -I/usr/include/OpenEXR -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -L/usr/local/lib -std=c++11 \
	-Wno-deprecated-register *.cc

1.3:
	./prog_out output_hw_1.3/t1.scn output_hw_1.3/t1.exr; \
	./prog_out output_hw_1.3/t2.scn output_hw_1.3/t2.exr; \
	./prog_out output_hw_1.3/t3.scn output_hw_1.3/t3.exr; \
	./prog_out output_hw_1.3/t4.scn output_hw_1.3/t4.exr; \
	./prog_out output_hw_1.3/t5.scn output_hw_1.3/t5.exr;


test:
	./prog_out scenes/scene_1.scn output_test/scene_1-test.exr; \
	./prog_out scenes/scene_2.scn output_test/scene_2-test.exr; \
	./prog_out scenes/scene_3.scn output_test/scene_3-test.exr; \
	./prog_out scenes/scene_4.scn output_test/scene_4-test.exr; \
	./prog_out scenes/scene_5.scn output_test/scene_5-test.exr; \
	./prog_out scenes/scene_7.scn output_test/scene_7-test.exr; \
	./prog_out scenes/scene_8.scn output_test/scene_8-test.exr; \
	./prog_out scenes/scene_11.scn output_test/scene_11-test.exr;

submit:
	./prog_out scenes/scene_1.scn output_submission/scene_1-submission.exr; \
	./prog_out scenes/scene_2.scn output_submission/scene_2-submission.exr; \
	./prog_out scenes/scene_3.scn output_submission/scene_3-submission.exr; \
	./prog_out scenes/scene_4.scn output_submission/scene_4-submission.exr; \
	./prog_out scenes/scene_5.scn output_submission/scene_5-submission.exr; \
	./prog_out scenes/scene_7.scn output_submission/scene_7-submission.exr; \
	./prog_out scenes/scene_8.scn output_submission/scene_8-submission.exr; \
	./prog_out scenes/scene_9.scn output_submission/scene_9-submission.exr; \
	./prog_out scenes/scene_10.scn output_submission/scene_10-submission.exr;

flipped:
	./prog_out scenes/scene_10.scn output_test/scene_10-test.exr;

teapot:
	./prog_out scenes/scene_9.scn output_test/scene_9-test.exr;

bunny:
	./prog_out scenes/bunny.scn output_test/bunny.exr;

box:
	./prog_out scenes/scene_1.scn output_test/args_scene_1.exr 1; \
	./prog_out scenes/scene_2.scn output_test/args_scene_2.exr 1; \
	./prog_out scenes/scene_3.scn output_test/args_scene_3.exr 1; \
	./prog_out scenes/scene_4.scn output_test/args_scene_4.exr 1; \
	./prog_out scenes/scene_5.scn output_test/args_scene_5.exr 1; \
	./prog_out scenes/scene_7.scn output_test/args_scene_7-test.exr 1; \
	./prog_out scenes/scene_8.scn output_test/args_scene_8-test.exr 1; \
	./prog_out scenes/scene_11.scn output_test/args_scene_11-test.exr 1;

noplane:
	./prog_out scenes/scene_3.scn output_test/scene_3_noplane.exr;
