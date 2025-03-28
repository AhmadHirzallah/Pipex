# Pipex

## Table of Contents

- [Introduction](#introduction)
- [Project Description](#project-description)
- [Installation & Compilation](#installation--compilation)
- [Usage](#usage)
- [Test Cases](#test-cases)
- [Bonus Features](#bonus-features)
- [Requirements & Guidelines](#requirements--guidelines)

## Introduction

Pipex is a project designed to explore a core UNIX mechanism—pipes—by implementing them in C. The goal is to replicate the behavior of shell piping in a program by chaining commands together and handling input/output redirection. This project not only reinforces your understanding of process management and file descriptors in UNIX but also deepens your skills in error handling, memory management, and adherence to coding standards.

### Project Demo

[![Pipex Project Demo](https://img.youtube.com/vi/MPfBGJQ-m6g/0.jpg)](https://www.youtube.com/watch?v=MPfBGJQ-m6g)

## Project Description

Your Pipex program must behave as follows:

```bash
./pipex file1 cmd1 cmd2 file2
```

This command should mimic the shell behavior of:

```bash
< file1 cmd1 | cmd2 > file2
```

Where:
- **file1**: Input file name.
- **cmd1**: First command with its parameters.
- **cmd2**: Second command with its parameters.
- **file2**: Output file name.

In the bonus part, the program should support multiple commands and a “here_doc” mode to handle input directly from the terminal.

## Installation & Compilation

1. **Clone the repository:**

   ```bash
   git clone <your-repository-url>
   cd pipex
   ```

2. **Compile the project using the provided Makefile:**

   ```bash
   make
   ```

   The Makefile includes the rules `$(NAME)`, `all`, `clean`, `fclean`, and `re` as required. It is configured to compile with the flags `-Wall -Wextra -Werror` using `cc`.

## Usage

After compiling, you can run your Pipex executable as follows:

- **Basic Usage:**

  ```bash
  ./pipex infile "cmd1 with args" "cmd2 with args" outfile
  ```

  This is equivalent to running the shell command:

  ```bash
  < infile cmd1 with args | cmd2 with args > outfile
  ```

- **Here Document (Bonus) Mode:**

  ```bash
  ./pipex here_doc LIMITER "cmd1 with args" "cmd2 with args" outfile
  ```

  This mimics:

  ```bash
  cmd1 << LIMITER | cmd2 >> outfile
  ```

## Test Cases

To help you and others evaluate the program, here are several test cases that you can run:

1. **Standard Pipe Tests:**

   ```bash
   ./pipex infile "ls -l" "wc -l" outfile
   ./pipex infile "grep a1" "wc -w" outfile
   ```

2. **Valgrind Memory Leak Checks:**

   ```bash
   valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes ./pipex here_doc END "cat" "wc -l" output.txt
   valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes ./pipex here_doc END "sleep 0" "wc -l" outfile
   valgrind -s --leak-check=full --track-fds=yes --trace-children=yes ./pipex infile "ls -l" "grep infile" outfile
   valgrind ./pipex here_doc STOP "sort" "uniq" output
   ```

3. **Additional Here Document Tests:**

   ```bash
   ./pipex here_doc END "cat" "wc -l" output.txt
   ./pipex here_doc END "sleep 5" "wc -l" outfile
   /pipex here_doc "MY LIMIT" "cat" "wc -c" output
   ```

4. **Complex Command Chain (Bonus) Tests:**

   ```bash
   valgrind ./pipex input.txt "tr a-z A-Z" "grep ERROR" "ls -l" output
   valgrind ./pipex input.txt "ls /invalid" "sort" output.txt
   ./pipex here_doc END "cat" "wc -l" output.txt
   valgrind ./pipex here_doc STOP "grep hello" "sort" output.txt
   ```

*Note:* Replace `infile`, `outfile`, `output.txt`, etc., with actual file names present on your system to conduct tests properly.

## Bonus Features

If you have successfully implemented the mandatory part, you can earn extra points by:

- **Handling multiple pipes:**  
  Allow your program to support more than two commands. For example:

  ```bash
  ./pipex file1 "cmd1" "cmd2" "cmd3" ... "cmdn" file2
  ```

  This should simulate:

  ```bash
  < file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2
  ```

- **Here Document Support:**  
  In here_doc mode, your program should work with commands such as:

  ```bash
  ./pipex here_doc LIMITER "cmd1" "cmd2" outfile
  ```

  Which is equivalent to:

  ```bash
  cmd1 << LIMITER | cmd2 >> outfile
  ```

## Requirements & Guidelines

- **Language:** C  
- **Norm Compliance:** Your project must adhere strictly to the provided Norm. Even bonus files/functions must pass the norm check.
- **Memory Management:** Ensure that all heap-allocated memory is freed appropriately. Memory leaks will result in a failing evaluation.
- **Error Handling:** The program must not terminate unexpectedly (segmentation faults, bus errors, double free, etc.) except in cases of undefined behavior.
- **Allowed Functions:**  
  Allowed external functions include: `open`, `close`, `read`, `write`, `malloc`, `free`, `perror`, `strerror`, `access`, `dup`, `dup2`, `execve`, `exit`, `fork`, `pipe`, `unlink`, `wait`, `waitpid` and optionally `ft_printf` (or its equivalent).
- **Libft:**  
  If using your own libft, copy its sources and Makefile into a `libft/` directory. The main Makefile should compile the library before compiling Pipex.
- **Makefile Requirements:**  
  Your Makefile must include the following rules: `$(NAME)`, `all`, `clean`, `fclean`, and `re`. The Makefile should not perform unnecessary relinking.
