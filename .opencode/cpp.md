# Guidelines — C++

C++-specific conventions and constraints for this project.

## Build System

- **No PlatformIO.** The only build system is a manual Makefile compiling with
  the xtensa-lx106-elf toolchain. Host-based unit/integration tests also use a
  separate Makefile targeting the host g++.
- **No native/host application binary.** The firmware target is exclusively the
  ESP-12F (ESP8266). Testing is done purely through unit and integration tests
  on the host.

## Code Style

- **Language-standard casing.**
  - `PascalCase` for classes, structs, enums, and type aliases.
  - `snake_case` for functions, variables, namespaces, and members.
  - `UPPER_SNAKE_CASE` for macros.
- **Comments on non-trivial code.** Anything beyond a plain field declaration,
  accessor, or trivial mutator gets a `// Does X.` comment describing its own
  behaviour (not its callers).
- **No `std::` prefixes.** Use file-scope `using` declarations for every
  standard-library type used in that file.
- **Static globals.** File-scope globals use explicit `static` linkage, not
  implicit external linkage.
