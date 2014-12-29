Network
=======
In one sentence `Network is a network visualiser which displays host-to-host communication observed through pcap live capture.`

Building
========
You will need:
- CMake >= 3.0.1
- ZMQ + cppzmq >= 4.0.0
- Qt >= 5.3

1). Obtain the source code

        git clone https://github.com/hutchinson/Network.git
2). Create a build directory

        mkdir ../Network_build && cd ../Network_build
3). Run cmake and make

        cmake ../Network && make

Running
=======
Run

        sudo bin/Network

sudo is required to allow pcap access to the network interfaces.

Notes
=====
This project is still in an alpha state 
