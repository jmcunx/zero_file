## zerof - overwrite a file with selected characters

This is a little utility created a while
ago for systems without dd.  All it does
is overrite an existing file with a character
of your choice (or random data).

This should **not** be used on
[Solid-state drives](https://en.wikipedia.org/wiki/Solid-state_drive).

On Modern File Systems
I doubt this utility will really overwrite the actual
drive sectors.

This requires [j\_lib2](https://github.com/jmcunx/j_lib2) to build.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, but this seems to be good enough for me.

**To compile:**
* If "DESTDIR" is not set, will install under /usr/local
* Execute ./build.sh to create a Makefile
* Works on Linux, BSD and AIX

_To uninstall_, execute
"make uninstall"
from the source directory
