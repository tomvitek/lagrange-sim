# lagrange-sim

This small project aims to show existence and stability of Lagrange points. 
It should also answer the questing whether the system can be (at least partially)
simulated mechanically by rotating 3d printed Newton potential.

The second part isn't finished yet.

## How to run the project
### C++ requirements
Only requirement of this project is Eigen library (mainpage [here](https://eigen.tuxfamily.org/index.php?title=Main_Page)).
Install it so it is visible to your compiler, or edit `CMakeLists.txt` so it includes the path where you have it installed.

Then it should be as easy as
1. `mkdir build; cd build`
2. `cmake ..` (use `cmake -G Ninja ..` if you have Ninja installed)
3. `cmake --build` (or you can call `make`/`ninja`)
4. Done!

You can now run the programs. See their source code to find out the necessary arguments. You should start in `src/execs` folder.

## Troubleshooting

### Python scripts

If your matplotlib doesn't work as you would expect, try to change your backend in the `matplotlibrc` file.
See [here](https://matplotlib.org/stable/tutorials/introductory/customizing.html#customizing-with-matplotlibrc-files) for details.