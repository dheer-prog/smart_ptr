# smart_ptr
Simple header file for smart pointer-style cleanup in C.

This header uses GCC/Clang's `cleanup` attribute so a pointer is automatically
freed when it goes out of scope.

## Header

```c
#define smart __attribute__((cleanup(cleaner)))
```

That means code like `smart int *p = malloc(sizeof *p);` will call `free(p)`
automatically when the current scope ends.

## Example

```c
#include <stdio.h>
#include <stdlib.h>
#include "smart_ptr.h"

int main(void) {
    smart int *value = malloc(sizeof *value);
    if (!value) {
        return 1;
    }

    *value = 42;
    printf("%d\n", *value);

    // No manual free(value); needed here.
    // cleaner() runs automatically when main returns.
    return 0;
}
```

Compile it with a compiler that supports GNU-style attributes:

```sh
cc -std=c11 -Wall -Wextra -pedantic example.c -o example
```

## What It Is Good For

- Avoiding simple "forgot to free" mistakes in short local scopes.
- Making early returns cleaner because `free()` still happens automatically.
- Giving C code a small RAII-like convenience for heap pointers.

## Faults / Limitations

- It does not enforce unique ownership. Copying the same pointer into two
  `smart` variables can cause a double free and undefined behavior.
- It only calls `free()`. This makes it unsuitable for resources that need
  another cleanup function such as `fclose()`, `close()`, or custom destructors.
- It relies on `__attribute__((cleanup(...)))`, which is a GNU extension and is
  not portable to all C compilers.
- The pointer must come from an allocator compatible with `free()`. Using it on
  stack memory, string literals, or memory owned elsewhere is invalid.
- It does not provide reference counting, shared ownership, borrow checking, or
  lifetime tracking.
- The `move()` helper in the header is currently broken as written because its
  assertion references undeclared identifiers (`t1` and `t2`), so it should not
  be relied on in its current form.

## Unsafe Example

This is exactly the kind of usage that can go wrong:

```c
smart int *a = malloc(sizeof *a);
smart int *b = a; // both variables now think they own the same allocation
```

When the scope ends, both cleanups will call `free()` on the same pointer.
