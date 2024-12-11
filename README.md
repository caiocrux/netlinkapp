# NetlinkApp

NetlinkApp is a comprehensive project designed to demonstrate communication between user space and kernel space in Linux using Netlink sockets. It includes both a user-space application and a kernel module, showcasing the implementation and interaction of Netlink communication protocols.

## Objective
The main objectives of the NetlinkApp project are:
- To provide a practical example of using Netlink sockets for kernel-to-user and user-to-kernel communication.
- To serve as a reference for developers learning about inter-process communication in Linux.
- To facilitate experimentation with custom Netlink protocols.

### Key Components
- **NetlinkApp/**: Contains the user-space application that communicates with the kernel module using Netlink sockets.
- **NetlinkKernelModule/**: Implements the kernel-side Netlink socket handling.

### Prerequisites

To build the project, you will need:
- GCC or a compatible C compiler
- CMake (for the user-space application)
- Linux kernel headers (for the kernel module)
- Debian packaging tools (for building the kernel module as a Debian package)

## Usage

1. Load the kernel module:
   ```bash
   sudo insmod netlink_kernel_module.ko
   ```

2. Run the user-space application to send and receive messages:
   ```bash
   ./netlink_app
   ```

3. Check kernel logs for additional details:
   ```bash
   dmesg | tail
   ```

4. To unload the kernel module:
   ```bash
   sudo rmmod netlink_kernel_module
   ```


