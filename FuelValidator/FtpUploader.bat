@echo off
echo user xxx>> ftpcmd.dat
echo xxx>> ftpcmd.dat
echo bin>> ftpcmd.dat
echo cd public_html>> ftpcmd.dat

:TOP
IF (%1) == () GOTO END
echo put %1>> ftpcmd.dat
SHIFT
 GOTO TOP
:END

echo quit>> ftpcmd.dat
ftp -n -s:ftpcmd.dat xxx
del ftpcmd.dat