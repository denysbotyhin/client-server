all: build

build:
	rm -rf build
	mkdir build
	cmake --build . --config Release -j 10 --
