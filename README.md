*This project has been created as part of the 42 curriculum by akaidi.*

# get_next_line

## Description

`get_next_line` is a C function that returns one line per call from a file
descriptor:

```c
char *get_next_line(int fd);
```

- The returned line includes its terminating `\n`, except for a last line
  that does not end with one.
- It returns `NULL` when there is nothing left to read or when an error
  occurs (invalid fd, `read` failure, `malloc` failure).
- It works on regular files, pipes and standard input.
- Allowed functions: `read`, `malloc`, `free`. No libft, no `lseek`, no
  global variables.

The goal of the project is to learn how `read` behaves and how to keep state
between calls with a `static` variable.

### Files

| File | Content |
|---|---|
| `get_next_line.c` | `get_next_line` and its static helpers |
| `get_next_line_utils.c` | string helpers (`ft_strlen`, `ft_strchr_nl`, `ft_strjoin_free`, `ft_copy`) |
| `get_next_line.h` | prototypes, includes, default `BUFFER_SIZE` |
| `*_bonus.c`, `*_bonus.h` | bonus: several file descriptors at once |

## Instructions

Compile with the buffer size of your choice:

```
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

Without `-D BUFFER_SIZE`, the default of 42 from the header is used.

Bonus (never compile it together with the mandatory files, both define
`get_next_line`):

```
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

Example:

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	while ((line = get_next_line(fd)))
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
```

The caller must free every returned line.

## Algorithm and justification

**The problem.** `read(fd, buf, BUFFER_SIZE)` returns up to `BUFFER_SIZE` bytes
and knows nothing about lines. One read can contain a full line plus the
beginning of the next one. Bytes already consumed cannot be pushed back into
the fd (`lseek` is forbidden, and it fails on pipes and terminals anyway), so
the surplus must be kept between calls.

**Data structure: one heap string, the "stash", held by a static pointer.**
The static pointer survives between calls (it lives in `.bss` and starts as
`NULL`), and the text it points to lives on the heap. A single string is the
simplest structure that works: appending a chunk and cutting a line are both
plain string operations, and every helper takes the stash as a parameter, so
the bonus only changes the glue code.

**One call:**

1. Return `NULL` if `fd < 0` or `BUFFER_SIZE <= 0`.
2. `read_to_stash`: while the stash has no `\n`, `read` a chunk into a heap
   buffer of `BUFFER_SIZE + 1`, put a `'\0'` after the bytes read, and append
   it to the stash. Stop on `\n`, on end of file (`read` returns 0), or on
   error (free everything, return `NULL`).
3. If the stash is `NULL`, there is nothing left: return `NULL`.
4. `extract_line`: copy the stash up to and including the first `\n`
   (or the whole stash if there is none).
5. `trim_stash`: keep only what follows the `\n` in a new string and free
   the old stash (`NULL` if nothing follows).
6. Return the line.

**Why this algorithm.**

- It reads as little as possible: the loop stops as soon as a `\n` is in the
  stash, and if the stash already contains one, `read` is not called at all.
- The read buffer is allocated with `malloc`, not on the stack, so
  `BUFFER_SIZE=10000000` does not overflow the stack.
- `read` returning 0 means "no more input", not "no more output": a leftover
  stash is returned as the last line, without an invented `\n`.
- Ownership rule: every function that replaces the stash frees the old one,
  and on failure frees what it owns and returns `NULL`. This avoids leaks and
  double frees. The caller frees the returned line.
- Rejected alternatives: reading the whole file first (does not scale, and
  never ends on stdin), reading one byte at a time (ignores `BUFFER_SIZE`,
  many system calls), a local `char buf[BUFFER_SIZE]` (stack overflow).

**Bonus: several file descriptors.** The single stash becomes one static
array, `static char *stash[FD_MAX]`, indexed by fd. It is still one static
variable. Each fd has its own offset in the kernel and its own leftover in
the array, so alternating calls on fd 3, 4 and 5 never mix lines. An fd outside
`0 .. FD_MAX - 1` (1024 by default) returns `NULL`, which also keeps the
indexing safe. The trade-off is a fixed cap on the fd number. A linked list
would remove it at the cost of more code.

**Known limits.** Behavior is undefined if the file changes between calls or
for binary files. A stash still holding text when the caller stops reading
early stays allocated, so read each fd to its end. The array cannot see a
`close`, so closing and reopening a fd before reaching its end keeps the old
leftover.

## Resources

- `man 2 read`, `man 2 open`, `man 3 malloc`, `man 3 free`
- Peer learning.
- The C standard on `static` storage duration
- 42 Norm documentation

**Use of AI.** <!-- edit this section so it is accurate -->
AI was used as a tutor, not as a code generator for the whole project.


