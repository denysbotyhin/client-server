all: prepare

install:
	sudo apt-get install gcc g++ cmake

prepare:
	rm -rf build
	mkdir build
