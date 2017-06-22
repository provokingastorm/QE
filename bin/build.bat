@rem build.bat
@rem Created by Peter Bennett 10/05/07
@echo off
set prevpath=../
set path=../Tools
:loop
..\Tools\gmake -f ArtBuild-Make.mf listchoices
set str=
set /p str=?
if '%str%'=='run' set str=run
if '%str%'=='version' set str=vers
if '%str%'=='face' set str=facetest
if '%str%'=='diag' set str=diag
if '%str%'=='q' goto exit
if '%str%'=='quit' goto exit
..\Tools\gmake -f ArtBuild-Make.mf %str%
rem if '%str%'=='' goto done
goto loop
:done
pause
:exit
set path=%prevpath%
