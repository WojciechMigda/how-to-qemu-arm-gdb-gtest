# How to debug and unit test code compiled for ARM on QEmu

# Setup
First you need to install few thing, if you haven't already:
```sh
$ sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf qemu-system-arm qemu-user gdb-multiarch
```
(see `setup.sh`)
This will install hard-float `gcc` cross-compiler & toolchain, `qemu`, and `gdb`.

```sh
$ arm-linux-gnueabihf-g++ --version
arm-linux-gnueabihf-g++ (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609
Copyright (C) 2015 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

# "Hello ARM!"
Let's compile and run the simplest of examples - "Hello ARM!" program.
```c++
#include <stdio.h>

int main()
{
    printf("Hello ARM!\n");
    return 0;
}
```
(see `hello.cpp`)
Our compiler is `arm-linux-gnueabihf-g++` and we will invoke it with from within a simple wrapper (`compile.sh`), which should yield `hello` executable.
It can be run with `qemu-arm` to which we also need to pass path to the runtime libraries.
```sh
$ qemu-arm -L /usr/arm-linux-gnueabihf ${@}
```
(see `qemu.sh`)
After running `./qemu.sh ./hello` we should be greeted with `Hello ARM!` on our terminal.

# Debugging
To debug our application we will tell `qemu-arm` to start it in debug mode listening on specified port, e.g.
```sh
$ qemu-arm -L /usr/arm-linux-gnueabihf -g 1234 ./hello
```
(see `qemu-gdb.sh`)
In another terminal we will launch `gdb-multiarch`:
```sh
$ gdb-multiarch
GNU gdb (Ubuntu 7.11.1-0ubuntu1~16.04) 7.11.1
Copyright (C) 2016 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word".
(gdb) file hello
Reading symbols from hello...done.
(gdb) set architecture armv5te
The target architecture is assumed to be armv5teset sysroot /usr/arm-linux-gnueabihf/
(gdb) set sysroot /usr/arm-linux-gnueabihf/
(gdb) target remote localhost:1234
Remote debugging using localhost:1234
Reading symbols from /usr/arm-linux-gnueabihf/lib/ld-linux-armhf.so.3...(no debugging symbols found)...done.
0xf67d6a40 in ?? () from /usr/arm-linux-gnueabihf/lib/ld-linux-armhf.so.3
(gdb) break main
Breakpoint 1 at 0x104dc: file hello.cpp, line 5.
(gdb) continue
Continuing.

Breakpoint 1, main () at hello.cpp:5
5           printf("Hello ARM!\n");
(gdb)
```
In the above first we had to tell gdb from where to load debug symbols (`file`), then we have set the hardware architecture (`set architecture`), followed by specifying location on runtime libraries (`set sysroot`). Once done with that we connect to `qemu` started just a while ago (`target remote`), set the breakpoint, and let the execution to continue.

You can also execute the above in single command:

```
gdb-multiarch --ex="set arch armv5te" --ex="set sysroot /usr/arm-linux-gnueabihf/" --ex="target remote localhost:1234" --ex="break main" hello
```

If you prefer `ddd` this is the command for you:

```
ddd --debugger='gdb-multiarch --ex="set arch armv5te" --ex="set sysroot /usr/arm-linux-gnueabihf/" --ex="target remote localhost:1234" --ex="break main" hello' --gdb
```

# Unit testing
For unit testing we will use Google Test/Mock combo. We will need native ARM libraries which we will compile ourselves. One thing to note - `gtest` will be built with threads disabled to remove dependency on `pthreads`, which we do not have cross-compiled.
```sh
$ cmake \
-DCMAKE_BUILD_TYPE=Release \
-DBUILD_GTEST=ON -DBUILD_GMOCK=ON \
-Dgtest_disable_pthreads=ON \
-DCMAKE_INSTALL_PREFIX:PATH=../googletest-arm \
-DCMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -DCMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ \
../googletest

make -j 4 && make install
```
(see `build-gtest.sh`)
With that we can compile simple unit testing suite (`ut.cpp`) and run it just as we did with `hello`. We should see this after running it:
```sh
$ ./qemu.sh ./ut
Running main() from gmock_main.cc
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from Sqrt
[ RUN      ] Sqrt.gives_some_value_for_positive_input
[       OK ] Sqrt.gives_some_value_for_positive_input (0 ms)
[ RUN      ] Sqrt.gives_none_value_for_negative_input
[       OK ] Sqrt.gives_none_value_for_negative_input (0 ms)
[----------] 2 tests from Sqrt (1 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (3 ms total)
[  PASSED  ] 2 tests.
```

# Tcp server within `qemu`
We can also launch tcp server within `qemu` and it will be able to communicate with the outside x86 world.
Once we compile attached golf server code (`echo_golf.c`) we can start it and then connect to it with `telnet`:
```sh
$ ./qemu.sh ./echo_golf 1234 &
$ telnet localhost 1234
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
128
Connection closed by foreign host.
```
It outputs `128`, which is a sum of decimal IPv4 address octet components.
