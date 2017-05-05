# LayersLevelGenerator

This project is written for CTU in Prague, Faculty of Information Technology within subject BI-SP1 (Software Project 1) by Danil Luzin, Constantin Grosu and Artem Kandaurov.

LayersLevelGenerator is a program for generating levels for game Layers.

The project is written in C++ language.

== Requirement

You need just a bash terminal for compile file:

== Generating

- Download the generator from the available source https://github.com/grosucon/LayersLevelGenerator
- File lvl.info describe the generated level.
- All levels will be generated in the same directory where is located your main.cpp in the 1.lvl file.
- All settings of one level you'll find on the top of the file main.cpp, all you need to change is just DEFINE methods to simulate your own level type.
- After all changes, compile the file using command g++ main.cpp -std=c++11 in a terminal.
- Now run ./a.out and enjoy your levels ;)

== Unit Tests

All unit tests are in file main.cpp.

== Realted Repositories

The Layers project.

https://github.com/kandaart/Layers
