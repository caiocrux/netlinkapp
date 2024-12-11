# NetlinkKernelModule

NetlinkKernelModule is a Linux kernel module that demonstrates the use of Netlink sockets for efficient communication between kernel space and user space. It serves as a companion to the NetlinkApp and provides a foundation for implementing custom Netlink protocols.

## Objective
The objective of NetlinkKernelModule is to:
- Illustrate how to create and handle Netlink sockets in the Linux kernel.
- Provide a simple example for exchanging messages between kernel and user space.
- Act as a reference for kernel-level communication mechanisms.

## Building the Kernel Module

### Prerequisites
To build and load the kernel module, ensure the following tools are installed:
- GCC or a compatible C compiler
- Linux kernel headers (matching your system's kernel version)
- Debian packaging tools (e.g., `dpkg`, `debhelper`)

### Build Instructions

1. Build the Debian package:
   ```bash
   dpkg-buildpackage -us -uc
   ```

   This will generate a `.deb` package in the parent directory.

2. Install the kernel module package:
   ```bash
   sudo dpkg -i ../netlink-kernel-module_<version>_<architecture>.deb
   ```

3. Verify that the module is loaded:
   ```bash
   lsmod | grep netlink_kernel_module
   ```

4. Check kernel logs for output:
   ```bash
   dmesg | tail
   ```

### Unloading the Module
To remove the kernel module, use the following commands:
```bash
sudo rmmod netlink_kernel_module
sudo dpkg -r netlink-kernel-module
```
