.PHONY: delete build rebuild run default

ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

default:rebuild run
rebuild: delete build
delete:
	rm -f *.o pro2
build: pro2 
pro2: main.o input.o func.o output.o
	gcc -o pro2 main.o input.o func.o output.o
main.o: main.c data.h input.h func.h output.h 
	gcc -c main.c
input.o: input.c input.h data.h
	gcc -c input.c
find.o: find.c find.h data.h
	gcc -c find.c
output.o: output.c output.h data.h
	gcc -c output.c
run:
	./pro2 $(RUN_ARGS)