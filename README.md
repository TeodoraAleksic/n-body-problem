# n-body-problem

The following project is a comparison of two implementations of the [n-body problem](https://en.wikipedia.org/wiki/N-body_problem): the [direct n-body simulation](https://en.wikipedia.org/wiki/N-body_simulation#Direct_gravitational_N-body_simulations) and the [Barnes-Hut algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation). Both simulations are implemented as single threaded running on the CPU, and multi threaded running on the GPU. GPU parallelisation is done using the [OpenCL](https://www.khronos.org/opencl/) framework. 

## Prerequisites

The project was built and tested on Windows 10 x64 using Microsoft C++ Redistributable 2017. 

To run the OpenCL simulations, a graphics card that supports OpenCL 1.2 is needed. The program was tested and currently only supports NVIDIA, AMD and Intel graphics cards. OpenCL drivers need to be installed/updated from the specific vendor's website.

You can find instructions on how to setup OpenCL in a Visual Studio project [here](http://kode-stuff.blogspot.rs/2012/11/setting-up-opencl-in-visual-studio_1.html).

## Tests

The program usage can be seen bellow.

```
Usage: nbodyproblem.exe (-i INPUT) [-o OUTPUT] [-s STEPS] [-h]

Runs the direct N-body and Barnes-Hut simulations on the CPU and GPU using OpenCL.

Options:
-i INPUT   Input CSV file containing particle data. Required.
-o OUTPUT  Output CSV file containing simulation results.
-s STEPS   Number of simulation steps that will be executed. Default 1.
-h         Prints usage.
```

The program requires an input CSV file with data about the particles used in the simulations to be passed in as the `-i` parameter. The `test` directory in this project contains five CSV files with dummy particle data. They are named `test##.csv`, where `##` indicates the number of particles contained within the file.

**Note**: the single threaded direct n-body simulation running on the CPU has a *O(n<sup>2</sup>)* complexity and thus takes much longer to execute. Avoid running the program with the larger CSV files before testing with the smaller ones.

The program outputs four CSV files, one for each simulation, containing data about the particles during the simulation. Execution times of the simulations will be printed to stdout. If the `-o` parameter is specified, the execution times will be written to a file specified by the parameter.

By default, only one step of the simulation is ran. A custom number of steps can be ran by specifying the `-s` parameter.

An example of running the program would be:
```
nbodyproblem.exe -i test\test100.csv -s 5 -o output.csv
```

**Note**: the `kernel` directory contains kernel function files that are executed on the GPU. The program requires the directory to be located in the same directory as the `exe` file.
