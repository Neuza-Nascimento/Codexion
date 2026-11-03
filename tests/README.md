*This project has been created as part of the 42 curriculum by nedo-nas.*

# Get Next Line

## Description
Welcome to **Get Next Line**! This project is a fundamental step in the 42 curriculum where the goal is to create a function that reads a file descriptor line by line. 

In C programming, reading from a file or standard input can be a bit tricky when you don't know the size of the content beforehand. This project introduces the concept of **static variables**, which allow us to "remember" what we've already read between function calls. It’s a highly practical tool that you'll likely use in many future projects to handle data input efficiently.

## Instructions

### Compilation
To use this function in your project, you need to include the header file and compile the `.c` files. A key part of this project is the `BUFFER_SIZE` macro, which defines how many bytes are read at a time.

You can compile the project using `cc` with the following flags:
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl
```
*Note: You can change `42` to any value to test how the function behaves with different buffer sizes.*

### Execution
Since this is a function and not a standalone program, you should call it within a loop in your main code to read an entire file:
```c
#include "get_next_line.h"

int main()
{
    int fd = open("example.txt", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

## Bonus Part

### How it Works
Instead of having a separate static variable for each file descriptor, this bonus implementation uses a single static variable (often a linked list or an array of structures) to store the remaining data for *all* active file descriptors. When `get_next_line` is called for a specific file descriptor, it searches this shared static structure for any buffered data related to that `fd`. If data exists, it processes it; otherwise, it reads from the `fd` and stores any leftover in the shared structure.

This approach demonstrates a deeper understanding of memory management and data structures, allowing for efficient handling of multiple concurrent reads without losing track of the reading state for each file.

### Compilation for Bonus
To compile the bonus part, you will typically have separate files (e.g., `get_next_line_bonus.c`, `get_next_line_utils_bonus.c`, `get_next_line_bonus.h`). The compilation command would look similar to the mandatory part, but including these bonus files:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c -o gnl_bonus
```

### Usage with Multiple File Descriptors
To use the bonus version, you would call `get_next_line` with different file descriptors. The function should correctly return lines from each `fd` without mixing them up:

```c
#include "get_next_line_bonus.h"

int main()
{
    int fd1 = open("file1.txt", O_RDONLY);
    int fd2 = open("file2.txt", O_RDONLY);
    char *line1, *line2;

    while ((line1 = get_next_line(fd1)) != NULL || (line2 = get_next_line(fd2)) != NULL)
    {
        if (line1)
        {
            printf("From file1: %s", line1);
            free(line1);
        }
        if (line2)
        {
            printf("From file2: %s", line2);
            free(line2);
        }
    }
    close(fd1);
    close(fd2);
    return (0);
}
```

## Algorithm Explanation
The core challenge of `get_next_line` is handling cases where the `read()` function returns more data than what is needed for a single line. Here is how the algorithm works:

1.  **Reading into a Buffer**: We read a chunk of data (defined by `BUFFER_SIZE`) from the file descriptor.
2.  **Storing in a Static Variable**: Since we might read past a newline character (`\n`), we store the extra data in a **static variable**. This ensures that the next time the function is called, it starts right where it left off.
3.  **Extracting the Line**: We search the accumulated data for a newline. If found, we "slice" the string: the part before and including the `\n` is returned to the user, and the remaining part is kept in the static variable for the next call.
4.  **Handling EOF**: If the end of the file is reached and there's still data in the static variable, we return that last bit. If everything has been read, we return `NULL`.

This approach is memory-efficient because it avoids reading the entire file into memory at once, processing only what is necessary to find the next line.

## Resources
To build this project, I explored several classic C programming concepts:
*   [Static Variables in C](https://www.geeksforgeeks.org/static-variables-in-c/) - Understanding how variables persist across function calls.
*   [File Descriptors and read()](https://man7.org/linux/man-pages/man2/read.2.html) - Learning how the system handles input/output.
*   [Memory Management](https://en.cppreference.com/w/c/memory) - Ensuring every `malloc` has a corresponding `free` to avoid leaks.

### AI Usage
In the spirit of the 42 curriculum's AI guidelines, AI was used as a **learning companion** rather than a shortcut:
*   **Conceptual Clarification**: I used AI to explain the lifecycle of static variables and how they are stored in the data segment of memory.
*   **Debugging Logic**: When facing edge cases (like a `BUFFER_SIZE` of 1 or very large values), I consulted AI to help visualize the pointer arithmetic and memory shifts.
*   **Documentation**: AI helped in structuring this README to ensure it meets all the professional requirements while remaining easy to read.
*   **Code Implementation**: All the logic and code were written manually to ensure a deep understanding of the underlying mechanics, as required by the 42 pedagogy.
