This is the kernel code for the spring 2022 offering of CSC 395: Advanced Operating Systems at Grinnell College. This project include a basic kernel with graphical rendering capability. For more information, please see section 3 (Project's structure and user manual).

By Quang Nguyen & Mario Manalu

1. Requirements and dependencies:
This section is quoted from CSC 395 "Setting Up a Development Environment" instruction by professor Charlie Curtsinger. 
Source: https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/activities/setup.html 

a. For MacOS:
> brew install xorriso x86_64-elf-binutils qemu

b. For Linux: 
> sudo apt-get install clang xorriso qemu-system
Next, we need to install Binutils. Download a copy of Binutils from: https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.gz. Open the terminal in the extracted directory and run:
> ./configure --target=x86_64-elf
> make
> sudo make install


2. Installation:
To build and run this project
> cd kernel 
> make clean
> make run 


3. Project's structure:
The project contains the kernel code, standard library, and other folders for demo programs (which are loaded as Stivale2 modules because the current kernel does not support a file system). The structure of the project is as follows:
- kernel/kernel: The kernel's source code. 
- kernel/stdlib: The standard library that can be used by user's program. 
- kernel/shell: a user application, runs as a terminal when the kernel boot. It can launch other applications.
- kernel/demo_term: A simple text based program that asks you to type input and outputs the exact string.
- kernel/demo_window: A demo of user's window.
- kernel/space_invaders: A simple Space Invaders game as a demo. 
- kernel/demo_3d: An interactive 3D cube to demo graphical capability.


4. User manual:
a. shell: This is a terminal which allow you to launch other applications. Once a program exit, it would launch shell. The inputs include:
- "clear": Clear the terminal.
- Name of a program: Launch the program. Currently, shell accepts "demo_term", "demo_window", "space_invaders", "demo_3d", "shell". Wrong input name would lead to an error message. The shell currently does not accept arguments (Some say it is hard to work with but we disagree :) ).

b. demo_term: This is a demonstration for the read/write IO of the kernel's system calls. The program will ask you type an input string (backspace is functional) and output the exact input. Press 'q' to quit and return to shell.

c. demo_window: This program show a mock up of a window (managed by the user's program) on top of the screen (managed by the kernel). The control includes:
- 'a': move left.
- 'd': move right.
- 'w': move up.
- 's': move down.
- 'f': toggle flipping the window (usually used to change the origin point between top-left downward and bottom-left upward).
- 'q': quit and return to shell.

d. space_invaders: This demonstration showcases the 2D graphic library, keyboard interaction, and animation. The user control the spaceship (the triangle) and shoot down the aliens (the square). The control includes:
- 'a': move left.
- 'd': move right.
- space bar: shoot.
- 'q': quit and return to shell.

e. demo_3d: This demonstration showcases the 3D graphic library, keyboard interaction, and animation. The user transforms the cube using the keyboard inputs. The control includes:
- 'a': move left.
- 'd': move right.
- 'w': move up.
- 's': move down.
- 'r': toggle rotation.
- 'f': toggle fill for the cube.
- 'j': reduce the size of the cube.
- 'k': increase the size of the cube.
- 'q': quit and return to shell.

5. Acknowledgements
This starter code is based on the following example projects:
- [OSDev.org Bare Bones Kernel](https://wiki.osdev.org/Bare_bones)
- [Stivale2 Barebones Kernel](https://github.com/stivale/stivale2-barebones)

In addition to the above example projects, the following references were used when setting up this code:
- [OSDev.org GCC Cross Compiler Instructions](https://wiki.osdev.org/GCC_Cross-Compiler)
- [Stivale2 Reference](https://github.com/stivale/stivale/blob/master/STIVALE2.md)