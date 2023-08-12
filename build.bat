@REM If build dir does not exist, create it
if not exist build mkdir build

@REM Change directory to build
cd build

@REM Run cmake
cmake ..

@REM Build the project
cd ..
cmake --build .\build\