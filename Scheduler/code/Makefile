build:
	gcc process_generator.c -o process_generator.out
	gcc clk.c -o CLK.out
	gcc scheduler.c -o scheduler.out -lm
	gcc process.c -o process.out
	gcc test_generator.c -o test_generator.out

clean:
	rm -f *.out  processes.txt
	cat /dev/null > scheduler.log
	cat /dev/null > scheduler.perf
	
run:
	./process_generator.out Input.txt
	
all: clean build
