# Q3 – C++ Debugging

This folder is dedicated to C++ compile-time and runtime debugging exercises.

## Files

| File | Error Type | Description |
|------|-----------|-------------|
| `buggy_compile.cpp` | Compile-time | Contains intentional syntax/type errors that prevent compilation |
| `buggy_runtime.cpp` | Runtime | Compiles successfully but crashes or produces wrong output at runtime |
| `fixes/` | — | Place corrected versions of the files here (e.g., `fixed_compile.cpp`, `fixed_runtime.cpp`) |

---

## How to Compile and Run

### Prerequisites
- A C++ compiler: `g++` (GCC) or `clang++`
- GNU Make (optional)

### Compile `buggy_compile.cpp`

```bash
g++ -Wall -Wextra -o buggy_compile buggy_compile.cpp
```

This will **fail** — read the error messages and identify the mistakes.

### Compile and Run `buggy_runtime.cpp`

```bash
g++ -Wall -Wextra -g -o buggy_runtime buggy_runtime.cpp
./buggy_runtime
```

This compiles but **crashes or misbehaves** at runtime. Use a debugger to investigate:

```bash
# With GDB
gdb ./buggy_runtime
(gdb) run
(gdb) backtrace

# With AddressSanitizer (recommended)
g++ -fsanitize=address -g -o buggy_runtime_asan buggy_runtime.cpp
./buggy_runtime_asan
```

### Saving Fixes

Place corrected files in the `fixes/` folder:

```bash
cp buggy_compile.cpp fixes/fixed_compile.cpp
# Edit fixes/fixed_compile.cpp to resolve errors
g++ -Wall -Wextra -o fixes/fixed_compile fixes/fixed_compile.cpp
```

---

## Debugging Tips

1. **Read the compiler error messages carefully** — the line number and message usually point directly at the problem.
2. **Use `-Wall -Wextra`** to enable common warnings.
3. **Use AddressSanitizer** (`-fsanitize=address`) to catch memory errors at runtime.
4. **Use a debugger** (`gdb` or `lldb`) to step through runtime errors.
5. **GitHub Copilot** can suggest fixes — open the buggy file and ask: _"Fix the errors in this file"_.
