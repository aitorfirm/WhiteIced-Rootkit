<p align="center"><strong>ngl, I think u wasted too many syscalls here 🌟</strong></p>

<p align="center">
  <img src="resources/logon.png" width="500">
</p>

# Whiteice Rootkit

Whiteice is a modular kernel rootkit designed for lab environments and virtual machine analysis. It incorporates advanced techniques for hiding processes, files, and communications using sophisticated hooking methods and direct kernel manipulation.

## Key Features

- Advanced process hiding through DKOM (Direct Kernel Object Manipulation) on EPROCESS structures.  
- SSDT (System Service Descriptor Table) hooking and inline hooking techniques to intercept and modify critical system calls.  
- Configurable logging system with support for DbgPrint and optional serial console output for real-time analysis without impacting performance.  
- Complete handling of basic network protocols (IP, TCP, UDP) to facilitate the development of traffic cloaking modules in future versions.  
- Modular architecture organized into specific components: encryptor, SHA, rootkit, and hooks, allowing easy maintenance and extension.  
- Configurable definitions through macros to adapt the rootkit to different scenarios and hiding requirements.

## Technical Requirements

- Windows 7 or higher running in a virtualized environment (QEMU, VMware, VirtualBox) with driver signature enforcement disabled (`bcdedit /set testsigning on`).  
- Windows Driver Kit (WDK) and Visual Studio for building and development.  
- Solid knowledge of kernel mode development, driver handling, and system call hooking.

## Technical References

- Process hiding via EPROCESS manipulation:  
  https://www.ired.team/miscellaneous-reversing-forensics/windows-kernel-internals/manipulating-activeprocesslinks-to-unlink-processes-in-userland
- SSDT and inline hooking techniques:
  https://www.blackhat.com/presentations/bh-usa-08/BH_US_08-Petroni-Skrien.pdf  
- Advanced kernel rootkit analysis:  
  https://www.sans.org/white-papers/341/
  
<p align="center">
  <img src="resources/boot.gif" width="400">
</p>

## Logging and Debugging

The rootkit implements a configurable logging system that can enable or disable output to DbgPrint and serial console, allowing detailed monitoring during testing without significant system performance impact.

## Modular Architecture

The codebase is divided into specific folders for each functionality, facilitating integration of new techniques and project maintainability.
