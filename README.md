# Software Defined Vehicle

This is my **playground** around the SDV topic. The aim is to experiment with building blocks
needed for an SDV ecosystem, focused on its onboard non-safety-critical parts.

This is **NOT** production code. It often covers only a happy path, is not thread-safe, does
not have documentation etc.

## Software First

SDV needs to start with software, and not hardware. No "do the same thing we did twenty years
ago, just in software". Let's first do the software platform, and then find hardware for it.

As a (nice) consequence, one must be able to develop, run and test the complete vehicle software
purely virtually, even locally, without any need to *always* deploy-and-test on test racks. I do
not say hardware testing is not needed, I am saying it must not be the only option.

## Build Instructions

You need:
- The latest compiler which supports C++23.
- CMake 3.28 or later (for C++ modules).
- Ninja (because C++ modules, and GNU Make does not support it).
  - Visual Studio should work as well, but I did not try it out.

After that it is the default CMake workflow, e.g.:

```sh
cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -G Ninja .. && ninja
```

**Note:** This project uses the latest-greatest of C++, which is still not equally good supported
by all toolchains. Clang 17 is known to work.
