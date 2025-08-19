
# C Memory Project

This project explores memory management concepts in C, including custom memory allocation, debugging, and code quality enforcement. The repository contains source code, headers, scripts, and supporting files for learning and experimenting with C memory handling.

## Project Goals
- Implement and understand custom memory allocation routines in C.
- Practice debugging and analyzing C programs.
- Enforce code style and quality using linting tools.
- Visualize and analyze data related to memory usage.

## Folder and File Overview

### Top-Level Files
- `cpplint.py`: Python script for checking C/C++ code style and quality.
- `debugme`: Compiled binary or script for debugging exercises (see `debugme.c`).
- `debugme.c`: C source file for debugging practice; contains code with intentional bugs or issues for learning debugging techniques.
- `metadata.yml`: Metadata file describing the project or its configuration.
- `README.md`: This documentation file.
- `scatterplot.R`: R script for generating scatterplots, likely used for visualizing memory usage or performance data.

### `hw7/` Directory
Contains the main implementation for the custom memory allocator:
- `cpplint.py`: Another copy of the linting script for code style checks within this folder.
- `Makefile`: Build instructions for compiling the memory allocator and related files.
- `mem_internal.h`: Internal header for memory allocator implementation details (not exposed to users).
- `mem.h`: Public header for the memory allocator API; defines functions and types for memory management.
- `memory.c`: Main C source file implementing the custom memory allocator functions.

## How to Use
1. Review the source code in `hw7/` to understand custom memory allocation logic.
2. Use the `Makefile` to build the project components.
3. Run `cpplint.py` to check code style and quality.
4. Experiment with `debugme.c` for debugging practice.
5. Use `scatterplot.R` for data visualization if needed.

## Getting Started
- Make sure you have a C compiler (e.g., gcc) and Python installed.
- Navigate to the `hw7/` directory and run `make` to build the memory allocator.
- Use the provided scripts and source files to explore memory management and debugging in C.

---
For questions or further information, refer to comments in each source file or contact the project maintainer.
