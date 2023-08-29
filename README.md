## zerof - overwrite a file with selected characters

**Note:** Starting October 12 2023, GitHub is forcing me to use
my Cell Phone for 2FA to allow access to my repositories.  I do
not want Microsoft to have access to my phone.  So updates to
this project will no longer occur on GitHub.  I will make a note
of where source can be retrieved from on my Gemini Capsule once
everything is moved off of GitHub.
Capsule: gemini://gem.sdf.org/jmccue/ or mirror gemini://sdf.org/jmccue/

This is a little utility created a while
ago for systems without dd.  All it does
is overwrite an existing file with a character
of your choice (or random data).

This should **not** be used on
[Solid-state drives](https://en.wikipedia.org/wiki/Solid-state_drive)
or similar drives.
If used, I doubt it will really clear the empty space.

Not sure if this will overwrite data on some Modern File Systems.

[j\_lib2](https://github.com/jmcunx/j_lib2) is an **optional** dependency.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, but this seems to be good enough for me.

**To compile:**
* If "DESTDIR" is not set, will install under /usr/local
* Execute ./build.sh to create a Makefile
* Works on Linux, BSD and AIX

_To uninstall_, execute
"make uninstall"
from the source directory
