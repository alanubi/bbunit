# bbunit

*bbunit -- C library for unit testing*

### Contents of the package

`libbbunit.a`
> Static library file with the unit testing framework.

`bbunit/bbunit.h`
> The header with short aliases to all necessary commands.

`bbunit/engine.h`
> The header with the commands that occur within a test.

`bbunit/report.h`
> The header with the commands necessary to run a test.

`bbunit/internal.h`
> The header with the command for running a test manually.

`demo.c`
> Look at this file to learn how to use the library.

---
### System requirements

Make sure you have installed `bbdump` library
and have a POSIX-compliant system.

---
### Installation

First of all, edit the file `Config` to fit your needs
(or set the variables from that file in a command line).
On a typical Linux machine you can keep it untouched.

Then build the program by running this command:

    $ make

You can check that the header files are
self-contained and compilable *(optional step)*:

    $ make face

You can also run the provided demo program *(optional step)*:

    $ make demo
    $ build/demo

And finally, install the library and its header files
(you need superuser privileges):

    # make install

Now you can use the library by including the header files
into your sources and adding `-lbbunit` flag to your compilations.

---
### Development

To remove the `build` directory, run this command:

    $ make clean

To package all the sources into a tarball, run this command:

    $ make dist
