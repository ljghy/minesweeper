# Minesweeper

```
git clone --recursive https://github.com/ljghy/minesweeper.git
```

## Build and run

```
mkdir build
cd build
```

#### Windows MSVC

```
cmake ..
cmake --build . --config [Debug|Release]
..\bin\minesweeper.exe
```

#### Windows MinGW-w64

```
cmake -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=[Debug|Release] ..
mingw32-make
..\bin\minesweeper.exe
```

#### Linux GCC

```
cmake -DCMAKE_BUILD_TYPE=[Debug|Release] ..
make 
export MESA_GL_VERSION_OVERRIDE=3.3
../bin/minesweeper
```

![minesweeper_hard](https://github.com/ljghy/minesweeper/blob/master/res/screenshots/minesweeper_hard.png)
