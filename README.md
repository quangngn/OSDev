# Kernel Project
This is the kernel code for the spring 2022 offering of CSC 395: Advanced Operating Systems at Grinnell College. This project includes a basic kernel with graphical rendering capability. For more information, please see section 3 (Project's structure and user manual).  

By Quang Nguyen & Mario Manalu  

## 1. Requirements and dependencies:
This section is quoted from CSC 395 "Setting Up a Development Environment" instruction by professor Charlie Curtsinger.  
Source: [Setting Up a Development Environment](https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/activities/setup.html)  

### a. For macOS:
```
> brew install xorriso x86_64-elf-binutils qemu
```

### b. For Linux:
```
> sudo apt-get install clang xorriso qemu-system
```
Next, we need to install Binutils. Download a copy of Binutils from: https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.gz. Open the terminal in the extracted directory and run:
```
> ./configure --target=x86_64-elf
> make
> sudo make install
```


## 2. Installation:
To build and run this project
```
> cd kernel 
> make clean
> make run 
```


## 3. Project's structure:
The project contains the kernel code, standard library, and other folders for demo programs (which are loaded as Stivale2 modules because the current kernel does not support a file system). The structure of the project is as follows:
- kernel/kernel: The kernel's source code. 
- kernel/stdlib: The standard library that can be used by user's programs. 
- kernel/shell: a user application, that runs as a terminal when the kernel boot. It can launch other applications.
- kernel/demo_term: A simple text-based program that asks you to type input and outputs the exact string.
- kernel/demo_window: A demo of the user's window.
- kernel/space_invaders: A simple Space Invaders game as a demo. 
- kernel/demo_3d: An interactive 3D cube to demonstrate graphical capability.


## 4. User manual:
### a. shell:
This is a terminal that allows you to launch other applications. Once a program exits, it would launch shell. The inputs include:
- "clear": Clear the terminal.
- Name of a program: Launch the program. Currently, shell accepts "demo_term", "demo_window", "space_invaders", "demo_3d", "shell". The wrong input name would lead to an error message. The shell currently does not accept arguments (Some say it is hard to work with but we disagree :) ).

### b. demo_term:

This is a demonstration of the read/write IO of the kernel's system calls. The program will ask you to type an input string (backspace is functional) and output the exact input. Press 'q' to quit and return to shell.

https://user-images.githubusercontent.com/43867447/168721688-2375e4ef-be95-4711-aca3-871a636f21e9.mov


### c. demo_window:
This program shows a mock-up of a window (managed by the user's program) on top of the screen (managed by the kernel). The control includes:
- 'a': move left.
- 'd': move right.
- 'w': move up.
- 's': move down.
- 'f': toggle flipping the window (usually used to change the origin point between top-left downward and bottom-left upward).
- 'q': quit and return to shell.

https://user-images.githubusercontent.com/43867447/168721712-bce53813-176f-4398-92c2-6e96b682a093.mov


### d. space_invaders:
This demonstration showcases the 2D graphic library, keyboard interaction, and animation. The user controls the spaceship (the triangle) and shoots down the aliens (the squares). The control includes:
- 'a': move left.
- 'd': move right.
- space bar: shoot.
- 'q': quit and return to shell.

https://user-images.githubusercontent.com/43867447/168721764-628e2fdd-e4cf-4efa-a9a3-ad53e68ceaa5.mov


### e. demo_3d:
This demonstration showcases the 3D graphic library, keyboard interaction, and animation. The user transforms the cube using the keyboard inputs. The control includes:
- 'a': move left.
- 'd': move right.
- 'w': move up.
- 's': move down.
- 'r': toggle rotation.
- 'f': toggle fill for the cube.
- 'j': reduce the size of the cube.
- 'k': increase the size of the cube.
- 'q': quit and return to shell.

https://user-images.githubusercontent.com/43867447/168721816-745bd0c6-ff24-4678-a39e-40a8cc4b80cb.mov


## 5. Acknowledgements
This starter code is based on the following example projects:
- [OSDev.org Bare Bones Kernel](https://wiki.osdev.org/Bare_bones)
- [Stivale2 Barebones Kernel](https://github.com/stivale/stivale2-barebones)

In addition to the above example projects, the following references were used when setting up this code:
- [OSDev.org GCC Cross Compiler Instructions](https://wiki.osdev.org/GCC_Cross-Compiler)
- [Stivale2 Reference](https://github.com/stivale/stivale/blob/master/STIVALE2.md)
