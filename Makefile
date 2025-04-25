all: prepare

prepare:
	rm -rf build
	mkdir build

build:
	cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug
	cmake --build build --config Release
