# Changelog
All notable changes to this project will be documented in this file.

### [Unreleased]

#### Known Issues
- `cli::log_insert()` breaks when input line not in view / scroll overflow.

#### Todo
- `.write_array()` method.
- `.write_spi()` method.
- Centralized `cli::log()` method with log level, colors etc.
- `constexpr` for expressions, common defualt file.
- Improved error handling and messages.

<!-- ----------------------------------------------------------------------------------------- -->

### [1.4.0] - 2019-03-25

#### Added

- `.set_pwm()` + command entry "set pwm".
- `.get_enc()` + command entry "get enc".
- `cmd_func(x)` define for shorter lambdas; currently unused.
- 500ms delay in UART `EXCEPTION` handler state.
- `.split_str()` method.
- `.write_array()` prototype and placeholder + CMD table entry.
- `.write_spi()` prototype and placeholder + CMD table entry.

#### Changed

- Changed `.callback_msg()` thread lambda clause to `by value`.
- Changed `.msgbox()` to take arguments by-value.
- Changed `.msgbox()` thread lambda clause to `by value`.
- Beautified CMD table; rename define `newcmd` to `subcmd`.
- Update changelog layout to match that of `MCU/docs`.
- Cleaned up layout.
- Changed `.write()` to `.write_byte()` + updated CMD table.

#### Fixed
- Messages from CLI occasionally showed gibberish.

---

<!-- ----------------------------------------------------------------------------------------- -->

### [1.3.0] - 2019-03-17

#### Added
- `cli::msgbox()` for dialog boxes.
- `cli::log_insert()` for inserting lines from thread.
- `sys::echo()` for test with MCU.
- Added proper changelog.

#### Changed
- Added some `const ...&` (by-ref) optimizations.

#### Fixed
- `uart::reciever`'s callbacks were `static`; moved definition into `.c` and updated to `extern` in `.h`.
- Callback lambda from `uart::send()` could be called too late, trying to access out memory; now reset to `nullptr` every time.

---

<!-- ----------------------------------------------------------------------------------------- -->

### [1.2.0] - 2019-03-15

#### Changed
- Made CLI a static class/namespace (`cli.h`).
- Made SYS a static class/namespace (`system.h`).

#### Fixed
- Remaining args had extra space (" COM3") in `cli::parse_input()`.
- 8-bit checksum generator included checksum field in iterations.

---

<!-- ----------------------------------------------------------------------------------------- -->

### [1.1.0] - 2019-03-14

#### Added
- UART static class/namespace (`uart.h`).
- Include `chksum.h` from embedded C.

---

<!-- ----------------------------------------------------------------------------------------- -->

### [1.0.0] - 2017-03-05

#### Added
- Created project.
- Basic command parser structs and methods.

<!-- ----------------------------------------------------------------------------------------- -->

[Unreleased]: #changelog
[1.3.0]: #changelog
[1.2.0]: #changelog
[1.1.0]: #changelog
[1.0.0]: #changelog
