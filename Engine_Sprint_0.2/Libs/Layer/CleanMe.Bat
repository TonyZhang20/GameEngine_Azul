REM
REM Keenan's Hack to remove intermediate files
REM If you find a way to enhance, please return updates to
REM ekeenan2@cdm.depaul.edu
REM

REM cleanup root director

    if exist ..\Azul rmdir /S /Q ..\Azul
    if exist *.user del /F /s *.user
    if exist Azul rmdir /S /Q Azul
    if exist Logs rmdir /S /Q Logs
    if exist Debug rmdir /S /Q Debug
    if exist Release rmdir /S /Q Release
    if exist x64 rmdir /S /Q x64
    if exist ipch rmdir /S /Q ipch
    if exist .vs rmdir /S /Q .vs
    if exist Temp12 rmdir /S /Q Temp12
    if exist Temp13 rmdir /S /Q Temp13

    if exist *.suo del /F /S /A:H *.suo
    if exist *.sdf del /F /S *.sdf
    if exist *.suo del /F /S *.suo
    if exist *.db del /F *.db
    if exist *.bin del /F *.bin
    if exist *.tlog del /F /S *.tlog

  
REM walk through subdirectories delete Release & Debug subs

for /d %%D in (*) do (
	REM echo %%~fD
	if exist %%~D\*.bin del /F %%~D\*.bin
	if exist %%~D\Debug rmdir /S /Q %%~D\Debug
	if exist %%~D\Release rmdir /S /Q %%~D\Release
	if exist %%~D\x64 rmdir /S /Q %%~D\x64
	if exist %%~D\Azul rmdir /S /Q %%~D\Azul
	if exist %%~D\*.user del /F /s %%~D\*.user
	if exist %%~D\*.tlog del /F /S %%~D\*.tlog
)