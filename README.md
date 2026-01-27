# 💡 GPIO Driver Module for Raspberry Pi

A **Linux kernel character device driver** that provides user-space control over Raspberry Pi GPIO pins through a device file interface.

---

## 📋 Project Description

This project implements a **custom Linux kernel module** that exposes GPIO pins as a character device, allowing user applications to control hardware through standard file operations. It demonstrates the fundamentals of Linux device driver development.

### What I Built:
- **Kernel Module**: Loadable kernel module with complete lifecycle management
- **Character Device**: `/dev/ext_device` for GPIO control
- **GPIO Abstraction**: Clean interface for GPIO pins 20, 21, and 22
- **Sysfs Export**: GPIO pins accessible via `/sys/class/gpio/`

### Real-World Application:
GPIO drivers are essential for any embedded Linux system that needs to interact with external hardware - from simple LED control to complex sensor networks. This pattern is used in industrial automation, robotics, and IoT gateways.

---

## 🛠️ Technologies Used

| Category | Technology |
|----------|------------|
| **Platform** | Raspberry Pi 3B+ |
| **Kernel** | Linux (Raspbian) |
| **Language** | C (Kernel space) |
| **API** | Linux GPIO Subsystem |
| **Driver Type** | Character Device Driver |
| **Build System** | Kbuild / Makefile |

---

## 📊 System Architecture

```
    ┌────────────────────────────────────────────┐
    │              USER APPLICATION              │
    │   write(fd, &value, sizeof(value))         │
    └──────────────────┬─────────────────────────┘
                       │
    ┌──────────────────┴─────────────────────────┐
    │          /dev/ext_device                   │
    │        Character Device File               │
    └──────────────────┬─────────────────────────┘
                       │
    ═══════════════════╪═════════════════════════ Kernel Boundary
                       │
    ┌──────────────────┴─────────────────────────┐
    │         GPIO DRIVER MODULE                 │
    │                                            │
    │   Value   │   Action                       │
    │   ────────┼──────────────────              │
    │     1     │   GPIO 20 → HIGH               │
    │     2     │   GPIO 21 → HIGH               │
    │     4     │   GPIO 22 → HIGH               │
    │    -1     │   GPIO 20 → LOW                │
    │    -2     │   GPIO 21 → LOW                │
    │    -4     │   GPIO 22 → LOW                │
    └──────────────────┬─────────────────────────┘
                       │
    ┌──────────────────┴─────────────────────────┐
    │              HARDWARE                      │
    │   GPIO 20    GPIO 21    GPIO 22            │
    │     │          │          │                │
    │    LED        LED        LED               │
    └────────────────────────────────────────────┘
```

---

## 🔑 Key Skills Demonstrated

### Linux Kernel Module Development
- Module macros (`MODULE_LICENSE`, `MODULE_AUTHOR`, `MODULE_DESCRIPTION`)
- Initialization and cleanup functions
- Kernel logging with `printk`
- Module parameter handling

### Character Device Framework
- Dynamic major/minor number allocation
- Character device structure (`struct cdev`)
- File operations structure (`struct file_operations`)
- Device class and node creation in sysfs

### GPIO Subsystem Mastery
- GPIO validation (`gpio_is_valid`)
- GPIO resource management (`gpio_request`, `gpio_free`)
- Direction control (`gpio_direction_output`, `gpio_direction_input`)
- Value operations (`gpio_set_value`, `gpio_get_value`)
- Descriptor-based API (`gpio_to_desc`, `gpiod_export`)

### Error Handling & Resource Management
- Goto-based error unwinding pattern
- Proper cleanup on module removal
- User-space buffer validation
- Return code conventions

### Kernel-User Communication
- `copy_from_user()` for write operations
- `copy_to_user()` for read operations
- Safe data transfer across privilege boundaries

---

## 📁 Project Structure

| File | Purpose |
|------|---------|
| `gpio_driver_v1.c` | Kernel module with GPIO control logic |
| `Makefile` | Kernel module build configuration |

---

## 🎯 What I Learned

- Linux kernel module lifecycle
- Character device driver architecture
- GPIO subsystem abstraction layers
- Safe kernel programming patterns
- Hardware-software interface design
