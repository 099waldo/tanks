# Tanks

A remake of Wii Tanks in C++ and SFML. 

## Building

At the moment the only dependency is SFML. You can install that here: https://www.sfml-dev.org/download.php

We use premake for generating the build files so you will need that to generate the project files. 

Right now we only support linux. Feel free to submit a pull request adding other platforms. 

### Linux using Makefiles

./premake5 gmake

make all