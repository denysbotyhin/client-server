# [Educational] Client-Server application in C++

This is client-server application created for educational purposes.
It implements both UDP and TCP version of the transport layer and serves clients in async mode

## Quick start

The project uses CMake build tools for project configuration.

### Installation guide

You will need a C++ compiler which supports the C++20 standard. Tested with Clang 16.0.0 and GNU 14.2

* clone the repository
* `make build`
* the executables can be found in the {project_root}/build/bin/Release

### Usage example

To test the message transfering simply start the server specifying the server host and port to listen. Then run the corresponding client providing server address and port optionally passing the message as the third argument. Example for the UDP:

* `./build/bin/Release/udp_server localhost 12121`
* `./build/bin/Release/udp_client localhost 12121`
* `./build/bin/Release/udp_client localhost 12121 test`
