Add these defines to the projects
You are including the dll to the project

PROTOBUF_USE_DLLS
ABSL_CONSUME_DLL

Conversion tool
protoc.exe to convert *.proto to *.pb.h and *.pb.cpp

as pre-build event
"$(SolutionDir)Proto\libs\tools\protoc.exe" --cpp_out=. *.proto

as post-build event  (copy the dll to exe directories)
Debug:
copy "$(SolutionDir)\Proto\libs\debug\*.dll" "$(SolutionDir)$(Platform)\$(Configuration)\."
Release:
copy "$(SolutionDir)\Proto\libs\release\*.dll" "$(SolutionDir)$(Platform)\$(Configuration)\."

why DLL... they are more stable and doesn't break when VS studio is upgraded

-Ed Keenan
