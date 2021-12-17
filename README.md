## zerof - overwrite a file with selected characters

This is a little utility created a while
ago for systems without dd.  All it does
is overrite an existing file with a character
of your choice (or random data).

On Modern File Systems or
[Solid-state drives](https://en.wikipedia.org/wiki/Solid-state_drive),
I doubt this utility will really overwrite the actual
drive sectors.

To build, execute build.sh to generate a Makefile from
Makefile.template on most BSD/Linux systems and IBM AIX.  For MS-DOS,
it should be rather easy to create a Makefile.  I know it does compile
under MS-DOS using Microsoft c.

This requires [j\_lib2](https://github.com/jmcunx/j_lib2) to build.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, so I came up with my own method which
is a real hack.  But works for me.

