# Multiplatform Chili Framework 
This is still the Chili Framework used in the world-famous, erection-inducing, cancer-curing Planet Chili C++ Tutorials. However, this one has its internals changed to OpenGL, glfw and glad, therefore it can run even on **Linux**! 😎 https://www.planetchili.net

# How to build this beast
## Windows
* You'll need git, CMake and Visual Studio 2017. After choosing your favourite working directory and navigating into it, you'll have to clone this amazing repository, **don't forget to download the submodules too with the extra parameter**:
```
C:\Users\Kappa>git clone --recurse-submodules https://github.com/FinalL/chili_framework
```
* After that, you can open up your cmake-gui and you should be able to select the cloned repository folder as source and prepare a build directory for the project. Then proceed to `Configure`. You should not need to change any settings and you should be able to proceed to `Generate`. When that is complete, `Open Project` will bring you to a prepared solution for building and making wonders. Right click on the `ALL_BUILD` project and `Build` should succeed after couple of seconds.
* The last thing remaining is to set the `Engine` project as StartUp project. You can do that by right-clicking the `Engine` project, and choosing the `⚙️ Set as StartUp project` option.

## Linux
* It's quite simple process, you'll need `cmake`, `git` and a compiler of choice, but we'll stick with `build-essentials` pack, since that makes building the cmake project trivial. On Debian derivative distributions of GNU/Linux, we'll also need to install glfw dependency `xorg-dev`.
```
user@machine:~$ sudo apt-get install cmake git build-essentials xorg-dev
```
* Clone the repository, **don't forget to download the submodules too with the extra parameter**:
```
user@machine:~$ git clone --recurse-submodules https://github.com/FinalL/chili_framework
```
* Enter the repository and make a build directory and enter it:
```
user@machine:~/chili_framework$ mkdir build && cd build
```
* Invoke `cmake` on the source:
```
user@machine:~/chili_framework/build$ cmake ..
```
* Build the project:
```
user@machine:~/chili_framework/build$ make
```
* If the last step produced no errors, you can proceed to launch the engine by:
```
user@machine:~/chili_framework/build$ Engine/Engine
```
