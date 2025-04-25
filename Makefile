all: prepare

prepare:
	rm -rf build
	mkdir build

build:
	cmake --build . --config Release -j 10 --
