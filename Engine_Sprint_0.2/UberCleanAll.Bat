REM
REM Keenan's Hack to remove intermediate files
REM Designed to work for C++ or C# visual studio projects
REM
REM If you find a way to enhance, 
REM please return updates to ekeenan2@cdm.depaul.edu
REM

REM cleanup root directory

    if exist ..\Azul rmdir /S /Q ..\Azul
    if exist *.user del /F /s *.user
    if exist Azul rmdir /S /Q Azul
    if exist Logs rmdir /S /Q Logs
    if exist Debug rmdir /S /Q Debug
    if exist Release rmdir /S /Q Release
    if exist x64 rmdir /S /Q x64
    if exist ipch rmdir /S /Q ipch
    if exist .vs rmdir /S /Q .vs



    if exist shaders\compiled rmdir /S /Q shaders\compiled


    if exist *.suo del /F /S /A:H *.suo
    if exist *.sdf del /F /S *.sdf
    if exist *.suo del /F /S *.suo
    if exist *.db del /F *.db

REM call each projects cleanme

cd Engine
call "CleanMe.Bat"
cd ..

cd Libs

cd Math
call "CleanMe.Bat"
cd ..

cd AnimTime
call "CleanMe.Bat"
cd ..

cd File
call "CleanMe.Bat"
cd ..

cd PCSTree
call "CleanMe.Bat"
cd ..

cd Manager
call "CleanMe.Bat"
cd ..

cd Events
call "CleanMe.Bat"
cd ..

cd Layer
call "CleanMe.Bat"
cd ..

cd Memory
call "CleanMe.Bat"
cd ..