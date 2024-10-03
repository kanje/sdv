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

## Structure Vision

SDV consists of onboard and offboard components. The onboard part is sometimes called
"the edge" and consists of all software which runs inside cars. The offboard part consists
of software running in the cloud, i.e. various backend services.

Onboard components can be further subdivided into safety-critical and non-safety-critical
ones. The difference is what are the consequences when something goes wrong. A malfunction
inside a safety-critical component can result in physical damage. Think, for example, about
autonomous driving. This imposes various limitations on how such software is developed.
For instance, dynamic memory allocation at runtime is forbidden, or only certified tooling
is allowed which tends to be quite conservative.

Non-safety-critical software has much more relaxed requirements. Generally, it can be
developed using the latest versions of libraries, compilers and other tooling, and provide
more freedom for developers to express themselves. This is one of the reasons why we are
here :) At the same time non-safety-critical components are the core of what is a software
defined vehicle. This is where most of the high level business logic happen. This is the part
which communicates with the outer world: the backend and other cars and devices.

### Application Framework

An application framework is a set of libraries which enables other applications and provides
the same look-and-feel for every developer. This is where common problems are solved once,
and hopefully are solved well.

This includes things like logging, execution model, lifecycle, persistent storage, domain
types, access to vehicle information (VSS), monitoring and alerting, telemetry, performance
metrics, testing and debugging, and others. Note, many of these concerns have a dedicated
service which does heavy lifting, while the framework provides APIs to access this service.

### Virtual Environment

A virtual environment provides means to run all software virtually, without deploying to
vehicle or test hardware. It allows faster development cycles and makes testing (at least
parts of it) easier.

It could be a Docker image per ECU, for example.

### Message Bus / Message Broker / IPC

Applications need to interact. There needs to be some sort of IPC mechanism, as well as an
interface definition language (IDL) to define inter-component interfaces.

### More

The SDV ecosystem needs more stuff, e.g. application monitoring, network gateway, software
update, log store etc. These, however, can come only after the basics mentioned above are
done.
