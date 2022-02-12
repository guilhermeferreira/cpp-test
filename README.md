# C++ TEST FRAMEWORK

Framework to C++ development


--------------------------------------------------------------------------------
## Table of Contents

* [1. PROBLEM](#1-problem)
  * [1.1. Introduction](#11-introduction)
  * [1.2. Description](#12-description)
  * [1.3. Requirements](#13-requirements)
* [2. SOLUTION](#2-solution)
  * [2.1. Project setup and structure](#21-project-setup-and-structure)
    * [2.1.1. ENVIRONMENT](#211-environment)
    * [2.1.2. SETUP](#212-setup)
    * [2.1.3. STRUCTURE](#213-structure)
  * [2.2. Run](#22-run)
  * [2.3. Features](#23-features)
  * [2.4. Code Structure](#24-code-structure)
    * [2.4.1. PERSISTENCE](#241-persistence)
    * [2.4.2. INTERFACE](#242-interface)
  * [2.5. Code Analysis](#25-code-analysis)
    * [2.5.1. STATIC ANALYSIS](#251-static-analysis)
    * [2.5.2. DYNAMIC ANALYSIS](#252-dynamic-analysis)
    * [2.5.3. SYMBOLIC EXECUTION](#253-symbolic-execution)
  * [2.6. Code Testing](#26-code-testing)
    * [2.6.1. UNIT TEST](#261-unit-test)
    * [2.6.2. INTEGRATION TEST](#262-integration-test)
    * [2.6.3. CODE COVERAGE](#263-code-coverage)
    * [2.6.4. SECURITY TEST](#264-security-test)
    * [2.6.5. PERFORMANCE TEST](#265-performance-test)
  * [2.7. Tools](#27-tools)


--------------------------------------------------------------------------------
## 1. PROBLEM

This section describes the application.


--------------------------------------------------------------------------------
### 1.1. Introduction

This is a simple project to demonstrate my coding skills and to keep track of
important tools.


--------------------------------------------------------------------------------
### 1.2. Description

Linux application that can store and retrieve settings and other string values.
The values have to be a key-value pair (so you store and retrieve information
based on a key) and have to be stored persistently.

The application needs to be controlled via commandline (so using stdin/stdout),
see the code below for an example of what kind of interaction is expected.

    $ ./KVPStorage
    > SET DeviceName TestDevice_123
    OK
    > GET DeviceName
    TestDevice_123
    > DELETE DeviceName
    OK
    > GET DeviceName
    >


--------------------------------------------------------------------------------
### 1.3. Requirements

1.3.1. Your application has to be developed in C or C++ (any version), C++ is
       preferred.
1.3.2. Your application has to be able to compile and run on Linux.
1.3.3. Your application has to store the settings on disk.
1.3.4. You are free to use libraries to help you as you see fit.
1.3.5. Your application has to support SET/GET/DELETE commands as shown above.


--------------------------------------------------------------------------------
## 2. SOLUTION

This section describes the solution to the test assignment. The purpose of this
documentation is to meet the requirement of section 1.1.4.

In this test assignment I want to show you how I tackle project organization
(sections 2.1, 2.4, 2.5, and 2.5) and program features (sections 2.2 and 2.3).


--------------------------------------------------------------------------------
### 2.1. Project setup and structure

This section describes my approach to requirement 1.1.1.


#### 2.1.1. ENVIRONMENT

The project provides a Docker image for build, deployment and execution.

Assuming we are at the project root directory. We build a Docker image with the
following command:

    $ pushd dist
    $ docker build -t dist:latest .
    $ popd
    $ docker build -t cpp-img:latest .

Run the Docker container:

    $ docker run --name cpp-cont -ti cpp-img bash


#### 2.1.2. SETUP

My approach to build is make to make it as easier and fast as possible. Also,
both unit and integration tests are integrated in the build.

First, install the requirements. Both tests frameworks are included in this
package.

    $ sudo apt install expect
    $ sudo apt install cmake
    $ sudo apt install lcov

Then, build the system (it runs the tests too):

    $ ./build.sh


#### 2.1.3. STRUCTURE

The table bellow describes the project structure.

Encapsulate what varies
Favor composition over inheritance
Program to interfaces, not implementations
Loose coupling betweem components
Single responsability
Open for extension, but close for modification
Liskov substituition 
Interface segregation

| DIRECTORY        | DESCRIPTION                              |
|------------------|------------------------------------------|
| src              | Classes for program logic.               |
| src/interface    | Classes for user interface.              |
| src/persistence  | Classes for data storage and formatting. |
|                  |                                          |
| test/unit        | Unit test files (Google Test).           |
| test/integration | Integration tests (Tcl/Expect).          |
|                  |                                          |
| cfg              | Configuration files (INI)                |
|                  |                                          |
| build            | Build products (binaries)                |


--------------------------------------------------------------------------------
### 2.2. Run

Run the program:

    $ ./Debug/src/KVPStorage --database cfg/database.ini

Store a key:

    > SET key1 value1
    OK

Retrieve a key:

    > GET key1
    KeyAsService

Remove a key:

    > DELETE key1
    OK

Terminate the program:

    > exit


--------------------------------------------------------------------------------
### 2.3. Features

Store a key with whitespaces:

    > SET key1 'value 1'
    OK
    > GET key
    'value 1'


--------------------------------------------------------------------------------
### 2.4. Code Structure

This section describes code strucure as requested in section 1.1.2.


#### 2.4.1. PERSISTENCE

Persistence can be any storage (e.g. File, Network) and have any format (e.g.
Json, YAML, XML, INI). Bellow is an UML class diagram of the implemented classes.

```
    +------------+
    |    Core    |
    +------------+
    | - command  |    +-----------+
    | - storage  |---o|  Storage  |
    +------------+    +-----------+
    | + loop     |    |           |
    | + command  |    +-----------+
    +------------+    | + load()  |
                      | + store() |
                      | + get()   |
                      | + set()   |
                      | + del()   |
                      +-----------+
                            A
                            |____________________________________
                            |                                    |
                     +-------------+                      +-------------+
                     | FileStorage |                      |  DBStorage  |
                     +-------------+                      +-------------+
                     | - settings  |                      |             |
                     | - file      |    +------------+    +-------------+
                     | - format    |---o|   Format   |    | + load()    |
                     +-------------+    +------------+    | + store()   |
                     | + load()    |    | # contents |    | + get()     |
                     | + store()   |    +------------+    | + set()     |
                     | + get()     |    | + load()   |    | + del()     |
                     | + set()     |    | + store()  |    +-------------+
                     | + del()     |    +------------+
                     +-------------+          A
                                              |
                                        +------------+
                                        |  Iniformat |
                                        +------------+
                                        |            |
                                        +------------+
                                        | + load()   |
                                        | + store()  |
                                        | + read()   |
                                        | + parse()  |
                                        +------------+
```

#### 2.4.2. INTERFACE

The logic is designed to be decoupled from the interface. It might be theoretically
interfaced by terminal, networking, and so on.

```
    +------------+
    |    Core    |
    +------------+    +-------------+
    | - command  |---o|  Interface  |
    | - storage  |    +-------------+
    +------------+    |             |
    | + loop     |    +-------------+
    | + command  |    | + request() |
    +------------+    | + reply()   |
                      +-------------+
                             A
                             |___________________________________________________________________________________________
                             |                       |                     |                      |                      |
                  +-------------------+    +-----------------+    +-----------------+    +-----------------+    +-----------------+
                  | TerminalInterface |    | ScriptInterface |    | WindowInterface |    |  WebInterface   |    | TelnetInterface |
                  +-------------------+    +-----------------+    +-----------------+    +-----------------+    +-----------------+
                  |                   |    |                 |    | - window        |    | - thread        |    |                 |
                  +-------------------+    +-----------------+    +-----------------+    +-----------------+    +-----------------+
                  | + request()       |    | + request()     |    | + request()     |    | + request()     |    | + request()     |
                  | + reply()         |    | + reply()       |    | + reply()       |    | + reply()       |    | + reply()       |
                  +-------------------+    +-----------------+    +-----------------+    +-----------------+    +-----------------+
```

#### 2.4.2.1. TerminalInterface

It reads input from command line terminal. Run it by omiting the `--ui` argument.

    $ ./Debug/src/KVPStorage --database cfg/database.ini


#### 2.4.2.2. ScriptInterface

It reads commands from a script file. Run it by passing a script file to the
`--ui` argument.

    $ ./Debug/src/KVPStorage --database cfg/database.ini --ui cfg/script.txt


#### 2.4.2.3. WindowInterface

It receives commands from a GUI. Run it by passing `--ui gui` argument.

    $ ./Debug/src/KVPStorage --database cfg/database.ini --ui gui

The class diagram:

```
    +-----------------+    +--------------+    +---------------+
    | WindowInterface |    |    Window    |    |  QMainWindow  |
    +-----------------+    +--------------+    +---------------+
    | - window        |o---|              |    |               |
    +-----------------+    +--------------+    +---------------+
    | + request()     |    | + request()  |    |               |
    | + reply()       |    | + reply()    |    +---------------+
    +-----------------+    | + show()     |            A
                           +--------------+            |
                                  A                    |
                                  |____________________|
                                  |
                           +--------------+
                           |   QtWindow   |
                           +--------------+
                           |              |
                           +--------------+
                           | + request()  |
                           | + reply()    |
                           | + show()     |
                           +--------------+
```

#### 2.4.2.5. WebInterface

It receives commands from a website. The web interface is available at the
`localhost:8080`.

Run it by passing `--ui web` argument.

    $ ./Debug/src/KVPStorage --database cfg/database.ini --ui web

If you run the program from within a Docker container, you have to map the TCP
port from host to container.

    $ docker run --name cpp-cont -ti cpp-img -p 8080:8080 bash
    # ./Debug/src/KVPStorage --database cfg/database.ini --ui web


#### 2.4.2.5.  TelnetInterface

Allows remote commands to be sent via Telnet protocol.


--------------------------------------------------------------------------------
### 2.5. Code Analysis

This section describes the tools to analyze the code.


#### 2.5.1. STATIC ANALYSIS

I use cppcheck to analyze the source code. As a static analysis tool, it scans
the source code looking for common programming bugs.

    $ cppcheck --enable=all --std=c++11 src/


#### 2.5.2. DYNAMIC ANALYSIS

Valgrind is an instrumentation framework for dynamic analysis tools. Among
these tools, we use memcheck. It's is a memory error detector.

    $ cmake -DSANITIZER=Off -S . -B build
    $ make -C build
    $ valgrind --leak-check=full --show-leak-kinds=all ./Debug/src/KVPStorage \
      --database ./test/unit/input/database/database.db \
      --ui ./test/unit/input/script/script.txt


#### 2.5.3. SYMBOLIC EXECUTION

KLEE is a symbolic execution tool.

    $ ulimit -s unlimited
    $ clang++ -emit-llvm -c -g -DSYMBOLIC=1 src/persistence/IniFormat.cpp
    $ klee --libc=uclibc \
           --libcxx \
           --write-cov \
           --write-test-info \
           --emit-all-errors \
           --readable-posix-inputs \
           --posix-runtime \
           --link-llvm-lib=${TEST_DST_DIR}/IniFormat.bc \
           ${TEST_DST_DIR}/IniFormatSymbolic.bc \
           --output-dir=${TEST_DST_DIR}

or using the provided script:

    $ ./symbolic.sh


--------------------------------------------------------------------------------
### 2.6. Code Testing

This section describes my test methodology as requested by section 1.1.3.

My goal with the tests is to cover close to 100% of the code base (2.6.3). There
are two levels of testing, unit and integration.

Unit testing focus on testing individual modules (2.6.1), and the integration
focuses on testing the program as a whole (2.6.2). I created an automate testing
inclusion so the user has just to create a file and it is automatically added to
the testing routine.

According to the Agile Testing Quadrants from Brian Marick, we have:

```
                               Business/Problem
                                    Domain
               +----+-----------------+-----------------+----+
               | Q2 |                 |                 | Q3 |
               +----+                 |                 +----+
               | Functional           | Usability            |
               |                      | Exploratory          |
               |                      | Acceptance           |
               |                      |                      |
               |                      |                      |
               |                      |                      |
     Guide     +----------------------+----------------------+ Critique
  Development  | Unit (2.6.1)         | Performance (2.6.5)  | Product
               | Component            | Load                 |
               | Integration (2.6.2)  | Stress               |
               |                      | Security (2.6.4)     |
               |                      | Reliability          |
               |                      |                      |
               +----+                 |                 +----+
               | Q1 |                 |                 | Q4 |
               +----+-----------------+----------------------+
                             Technology/Solution
                                    Domain
```

IMPORTANT: Some tests fail ON PURPOSE because I want to illustrate pending
           testing scenarios and how the framework (specially the integration)
           displays failing tests.


#### 2.6.1. UNIT TEST

Unit test is a functional test. It's implemented as a white-box test of individual
modules (or units): functions, methods, and classes.

There are two ways to group test cases, using a test suite or a test fixture.
The difference is that test fixtures provide initialization for each test
case.

Unit tests use Google Test (GTest). In order to run the unit tests, execute the
commands:

    $ cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
    $ make -C build check

or using the provided script:

    $ ./build.sh


#### 2.6.2. INTEGRATION TEST

Integration test is another functional test. It checks if a group of modules
(or units) work together. The goal is to find defects in the interface between
these units.

Integration tests use TCL language and expect program. The integration test
framework was specially designed for this test assignment. It has the same look-
and-feel as the GTest. To run the integration tests, execute the commands:

    $ cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
    $ make -C build distcheck

or using the provided script:

    $ ./build.sh


#### 2.6.3. CODE COVERAGE

Coverage uses GCov. The coverage is NOT part of the build.sh script. In order
to run it, you must execute the commands (it requires firefox to display the
results):

    $ cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
    $ make -C build coverage

or using the provided scripts:

    $ ./build.sh
    $ ./coverage.sh


#### 2.6.4. SECURITY TEST

Security testing is a non-functional form of testing. Its goal is to reveal
flaws that might cause unexpected behaviors and crashes when the program
receives ill-formed input. We use fuzzing as security test.

Fuzzing allows the creation of many random inputs to check if the program
crashes.

We use American Fuzzy Lop (AFL) to fuzzing. This tools requires intrumentation
of the source code in order to properly work. The instrumentation is done by
the AFL compiler wrappers, afl-gcc and afl-g++.

After the program is instrumented, we run it with AFL fuzzer (afl-fuzz).

For example:

    $ cmake -DFUZZING=On -S . -B build
    $ make -C build security

or using the provided script:

    $ ./security.sh


#### 2.6.5. PERFORMANCE TEST

Performance testing is a non-functional form of test. For time sensitive
software, a timed response is as import as a correct response. Thus, it's
import to guarantee the software works within a time range.

There are two concepts related to performance, profiling and benchmarking.


##### 2.6.5.1. PROFILING

Profiling is about learning where your program spent its time. Which
functions called which other functions while it was executing.

I'm profiling the code using GProf.

The following instructions profile the code:

    $ cmake -DPROFILER=On -DCMAKE_BUILD_TYPE=Debug -S . -B build
    $ make -C build profile

or using the provided script:

    $ ./profile.sh


##### 2.6.5.2. BENCHMARKING

Benchmarking is about learning how much time your program spend on each
function.

I'm benchmarking using Google Benchmark.

The following instructions benchmark the code:

    $ make -C build benchmark

or using the provided script:

    $ ./benchmark.sh

Through the performance test I discovered the DBStorage outperforms FileStorage
with its load()/store() performing at O(1) whereas the latter performing at O(1).


--------------------------------------------------------------------------------
### 2.7. Tools

I use the tools required by section 1.3 and the ones Kalin told me during my
interview on October 19, 2020.

| Tool                  | Version                                                |
|-----------------------|--------------------------------------------------------|
| Programmming language | C++17 (ISO/IEC 14882:2017). Required by section 1.3.1. |
| Compiler              | GCC (10.2.0)                                           |
|                       | Clang (9.0.0)                                          |
| C Library (libc)      | GNU libc (2.23)                                        |
| Code convention       | Google Style Guide. Mentioned in the interview.        |
| Build system          | CMake (3.18.4). Required by the job description.       |
| Source Control        | Git (2.20.1)                                           |
|                       |                                                        |
| Static Analysis       | cppcheck (1.86)                                        |
| Dynamic Analysis      | Valgrind (3.14.0)                                      |
| Symbolic Execution    | KLEE (2.2)                                             |
|                       |                                                        |
| Unit test             | Google Test (1.10.0)                                   |
| Integration test      | TCL/Expect (5.45)                                      |
| Scripting             | Bash shell (4.3.48)                                    |
| Fuzzer                | American Fuzzy Lop (2.57b)                             |
| Coverage              | lcov (1.15)                                            |
| Benchmark             | Google Benchmark (1.6.0)                               |
| Database              | SQLite3 (3.34.0)                                       |
| Libraries             | Boost (1.68.0)                                         |
|                       | Crow (0.1)                                             |
|                       |                                                        |
| Operating System      | Ubuntu (16.04)                                         |
| Linux kernel          | 4.15.0-120-generic                                     |
