windres resources.rc -O coff -o resources.res
gcc *.c -o window.exe -mwindows resources.res -Wall -Wextra -Wno-unused-parameter -Werror
Rem gcc *.c -o window.exe -m32 -mwindows resources.res -Wall -Wextra -Wno-unused-parameter -Werror
Rem ^^^ FOR COMPILATION OF 32 BIT BINARY. ENSURE THAT MINGW32 GCC IS BEING USED (CHECK PATH VARIABLE)