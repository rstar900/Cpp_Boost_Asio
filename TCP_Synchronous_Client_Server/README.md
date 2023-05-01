# TCP_Synchronous_Client_Server

Code is based on the tutorial at https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

## Requirements
cmake > 3.15, Conan 2 and a suitable version of g++ which supports C++17 standard

## Build and Run (Run commands from this folder)

Make sure you have installed Conan 2 (https://docs.conan.io/2/installation.html) and configured it accordingly. <br><br>

1. Run the command below in case conan does not detect a profile

``` conan profile detect --force <br> <br> ```

2. then make sure build diretory does not exist already, if it does then

``` rm -rf build ```

3. Then run the following commands on Unix based systems (Linux and MacOS)

```
mkdir build
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

4. Then in one terminal tab run 

```./tcp-sync-server ```

5. And in another (making sure you are still in build folder)

``` ./tcp-sync-client ```

## Clean
Just remove the build folder by doing ```rm -rf build``` from project root directory.
