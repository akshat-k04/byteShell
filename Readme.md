
# ByteShell

## Overview

ByteShell is a lightweight Unix-like shell implemented in C++. It supports basic shell operations and built-in commands, including file and directory manipulation, command history management, and more. The shell aims to provide a simple and educational example of how a shell works, demonstrating fundamental concepts like command execution, file operations, and command history.

## Features

- **Command Execution**: Executes various built-in shell commands.
- **File Operations**: Create, delete files, and directories.
- **Directory Traversal**: Recursively list directory contents.
- **Command History**: View and re-execute previous commands.
- **Command Help**: Provides usage information for built-in commands.

## Built-in Commands

1. **`echo [OPTION]... [STRING]...`**
   - Print STRING(s) to the standard output.
   - Options:
     - `-n`    Do not print the trailing newline character.
     - `-e`    Enable interpretation of backslash escapes (e.g., `\n`, `\t`).

2. **`cd [DIRECTORY]`**
   - Change the current working directory to DIRECTORY.
   - Special case: `~` for the home directory.

3. **`exit`**
   - Exit the shell.

4. **`pwd`**
   - Print the current working directory.

5. **`history [OPTION] [ARGUMENT]`**
   - Display the command history.
   - Options:
     - `-c`    Clear the command history.
     - `| grep [SEARCH_STR]`  Filter history by SEARCH_STR.

6. **`!!`**
   - Re-execute the last command from history.

7. **`!N`**
   - Re-execute the command at index N from history.

8. **`touch [FILENAME]`**
   - Create a new file named FILENAME.

9. **`mkdir [DIRECTORY_NAME]`**
   - Create a new directory named DIRECTORY_NAME.

10. **`rm [FILENAME]`**
    - Delete the file named FILENAME.

11. **`rmdir [DIRECTORY_NAME]`**
    - Delete the directory named DIRECTORY_NAME.

12. **`ls [-R]`**
    - List directory contents.
    - Options:
      - `-R`    Recursively list subdirectories.

13. **`ps`**
    - Display process information (not implemented yet).

14. **`kill`**
    - Terminate a process (not implemented yet).

## Building and Running

### Prerequisites

- **C++ Compiler**: A modern C++ compiler compatible with C++11 or later (e.g., `g++`).
- **WSL (Windows Subsystem for Linux)**: Ensure that WSL is installed and set up on your Windows machine.

### Compilation

To compile the ByteShell project, run the following command:

```sh
g++ -o byteshell main.cpp shell.cpp
```

### Running

To run ByteShell, execute the compiled binary:

```sh
./byteshell
```

## Development

The project is structured as follows:

- **`main.cpp`**: Contains the main entry point and the main loop of the shell.
- **`shell.h`**: Header file declaring global variables and function prototypes.
- **`shell.cpp`**: Contains the implementation of the shell commands and helper functions.
- **`README.md`**: Documentation for the ByteShell project.

---