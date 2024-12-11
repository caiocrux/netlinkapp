# NetlinkApp

NetlinkApp is a lightweight application designed to demonstrate the use of Linux's Netlink sockets for communication between kernel and user space. The project serves as an example of how to implement Netlink communication in a clean and organized way.

## Objective
The primary objective of NetlinkApp is to provide a practical example of using Netlink sockets to facilitate communication between the Linux kernel and user-space applications. This can be useful for:
- Sending and receiving custom messages.
- Monitoring kernel events.
- Performing actions triggered by kernel signals.

### Prerequisites
To build and run the NetlinkApp, ensure the following tools are installed:
- GCC or a compatible C compiler
- CMake (version 3.10 or higher)
- Linux kernel headers (matching your system's kernel version)

1. Create a build directory and configure the project using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

2. Build the application:
   ```bash
   cmake --build .
   ```

   This will compile the source files and generate the executable binaries in the `build/` directory.

