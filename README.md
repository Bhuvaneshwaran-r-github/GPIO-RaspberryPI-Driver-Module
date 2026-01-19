# GPIO Raspberry Pi Driver Module

A Linux kernel character device driver for Raspberry Pi 3B+ providing GPIO control through a simple device file interface.

## Features

- Custom Linux kernel character device driver
- Read/Write GPIO states through device file operations
- Support for GPIO pins 20, 21, and 22
- GPIO sysfs export for user-space access
- Configurable GPIO direction (input/output)

## Hardware Requirements

- Raspberry Pi 3B+
- LEDs, buttons, or other GPIO peripherals
- Connecting wires

## Software Prerequisites

- Raspberry Pi OS (Linux kernel headers required)
- GCC compiler
- Make utility

## Building the Driver

```bash
# Build the kernel module
make

# Load the module
sudo insmod file1.ko

# Verify module is loaded
lsmod | grep file1

# Check kernel messages
dmesg | tail
```

## Usage

### Device File Operations

```bash
# The driver creates /dev/ext_device

# Write to GPIO (from user application)
./app1

# Read GPIO states
./app2
```

### GPIO Control Values

| Value | Action |
|-------|--------|
| 1 | Set GPIO 20 HIGH |
| 2 | Set GPIO 21 HIGH |
| 4 | Set GPIO 22 HIGH |
| -1 | Set GPIO 20 LOW |
| -2 | Set GPIO 21 LOW |
| -4 | Set GPIO 22 LOW |

## File Structure

| File | Description |
|------|-------------|
| `file1.c`, `file2.c` | Kernel module driver implementations |
| `app1.c`, `app2.c` | User-space test applications |
| `Makefile` | Build configuration |

## How It Works

1. Kernel module registers a character device
2. Device file `/dev/ext_device` is created
3. User applications open the device and perform read/write operations
4. Driver translates file operations to GPIO hardware control
5. GPIO states are exported to sysfs for direct access

## Unloading the Module

```bash
sudo rmmod file1
```
