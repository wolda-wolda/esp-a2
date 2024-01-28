CC            := clang
CCFLAGS       := -Wall -Wextra -pedantic -std=c17 -g
ASSIGNMENT    := a2

.DEFAULT_GOAL := default
.PHONY: default clean bin all run test help


default: help

clean:                ## cleans up project folder
	@printf '[\e[0;36mINFO\e[0m] Cleaning up folder...\n'
	rm -f $(ASSIGNMENT)
	rm -f testreport.html
	rm -rf valgrind_logs

bin:                  ## compiles project to executable binary
	@printf '[\e[0;36mINFO\e[0m] Compiling binary...\n'
	$(CC) $(CCFLAGS) -o $(ASSIGNMENT) a2.c
	chmod +x $(ASSIGNMENT)
	chmod +x testrunner

all: clean bin  ## all of the above

.IGNORE: run
run: all              ## runs the project
	@printf '[\e[0;36mINFO\e[0m] Executing binary...\n'
	@./$(ASSIGNMENT) A;\
	printf "[\e[0;36mINFO\e[0m] Program returned $$?\n"

ARCH := $(shell uname -m)
OS := $(shell uname -s)

test: all             ## runs public testcases on the project
ifneq ($(ARCH), x86_64)
		@printf '[\e[0;31mWARNING\e[0m] You are not using a x86_64 machine.\n'
		@printf '[\e[0;31mWARNING\e[0m] Please replace the testrunner with the correct binary from\n'
		@printf '[\e[0;31mWARNING\e[0m] https://gitlab.tugraz.at/testrunner/testrunner/-/releases/v2.0.3\n'
		@printf '[\e[0;31mWARNING\e[0m] aarch64-unknown-linux-gnu -> ARM based Linux (use this if you are using the UTM VM on macOS)\n'
		@printf '[\e[0;31mWARNING\e[0m] x86_64-apple-darwin -> macOS Intel\n'
		@printf '[\e[0;31mWARNING\e[0m] aarch64-apple-darwin -> macOS ARM (m1,m2,m3)\n'
else ifeq ($(OS), Darwin)
		@printf '[\e[0;31mWARNING\e[0m] You are using the testrunner on an Intel Mac\n'
		@printf '[\e[0;31mWARNING\e[0m] Please replace the testrunner with the correct binary from\n'
		@printf '[\e[0;31mWARNING\e[0m] https://gitlab.tugraz.at/testrunner/testrunner/-/releases/v2.0.3\n'
		@printf '[\e[0;31mWARNING\e[0m] x86_64-apple-darwin -> macOS Intel\n'
endif
	@printf '[\e[0;36mINFO\e[0m] Executing testrunner...\n'
	./testrunner -c test.toml

help:                 ## prints the help text
	@printf "Usage: make \e[0;36m<TARGET>\e[0m\n"
	@printf "Available targets:\n"
	@awk -F':.*?##' '/^[a-zA-Z_-]+:.*?##.*$$/{printf "  \033[36m%-10s\033[0m%s\n", $$1, $$2}' $(MAKEFILE_LIST)
