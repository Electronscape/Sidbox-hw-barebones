setlocal enabledelayedexpansion
REM (USE THIS) put the following in the pro-build cmd.exe /c "${ProjDirPath}\go.cmd"
REM put the following in pre-build --- NO QUOTES   C:\Windows\System32\cmd.exe /c "W:\Work\Electronics\STM32 Projects\SidboxWorkSpace\sidbox5\go.cmd"
cd "W:\Work\Electronics\STM32 Projects\SidBox5.6"
w:

set major=5
set minor=4
set buildnum=

del /Q ..\bin\*.*

for /f "delims=;" %%i in (build_number.txt) do set buildnum=%%i

rem cp dist/default/production/piclibs.X.production.hex ../bin
rem mv ../bin/piclibs.X.production.hex ../bin/v%major%_%minor%_%buildnum%.hex

echo off

set /a buildnum=%buildnum% + 1


@SETLOCAL ENABLEDELAYEDEXPANSION

@REM Use WMIC to retrieve date and time
@echo off
FOR /F "skip=1 tokens=1-6" %%A IN ('WMIC Path Win32_LocalTime Get Day^,Hour^,Minute^,Month^,Second^,Year /Format:table') DO (
    IF NOT "%%~F"=="" (
        SET /A SortDate = 10000 * %%F + 100 * %%D + %%A
        set YEAR=!SortDate:~2,2!
        set MON=!SortDate:~4,2!
        set DAY=!SortDate:~6,2!
        @REM Add 1000000 so as to force a prepended 0 if hours less than 10
        SET /A SortTime = 1000000 + 10000 * %%B + 100 * %%C + %%E
        set HOUR=!SortTime:~1,2!
        set MIN=!SortTime:~3,2!
        set SEC=!SortTime:~5,2!
    )
)
@echo on
echo DATE=%DATE%, TIME=%TIME%
echo HOUR=!HOUR! MIN=!MIN! SEC=!SEC!
echo YR=!YEAR! MON=!MON! DAY=!DAY! 
set DATECODE=!DAY!/!MON!/!YEAR! !HOUR!:!MIN!:!SEC!

set NLM=^


set NL=^^^%NLM%%NLM%^%NLM%%NLM% 
@echo %buildnum%;> build_number.txt
@echo //                       ARYEMNHRMNSS %NL%#define build "%major%.%minor%.%buildnum% - %DATECODE%" %NL%#define CPT_DAY %DAY% %NL%#define CPT_MON %MON% %NL%#define CPT_YEAR 20%YEAR% > build_number.h 
