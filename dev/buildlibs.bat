@rem buildlibs.bat
@rem JFL 12 Dec 06; pathb
@echo off
set prevpath=%path%
set path=..\..\bin
:loop
..\..\bin\gmake -f build\makefile.mf listchoices
set str=
set /p str=?
@rem if '%str%'=='r' set str=run
if '%str%'=='v' set str=vers
if '%str%'=='q' goto exit
if '%str%'=='quit' goto exit
..\..\bin\gmake -f build\makefile.mf %str%
rem if '%str%'=='' goto done
goto loop
:done
pause
:exit
set path=%prevpath%
