coms229-project2
Ryan Scheel - rascheel@iastate.edu - 971814844
================
OVERVIEW

All source code is in ./src, sim-gui and the gui code is in src/sim-gui/

After calling make the executables will be placed in the ./bin 
directory and .o files will be placed in the ./obj directory.
The ./bin and ./obj directories will be deleted after running a 
make clean.

make - builds everything
make clean - cleans all generated files
make test - runs some rudimentary tests
make tarball - builds tarball for final submission

================
FILE DESCRIPTIONS

common.h/common.cpp - These contain some structs useful structs that are used all over,
                      and some common functions that are useful like an exitWithError function.
                      Also contains some global constants and enums that are used to increase
                      code clarity.

terrain.h/terrain.cpp - This defines the terrain class which is the major logic portion of this project.
                        It stores the cell state data and reads aut files and outputs as an aut file.
                        It also contains an abstract subclass simulator which is used to implement different
                        types of simulations at compile time.

conwaysGOL.h conwaysGOL.cpp - Has the conwaysGOL class which inherits from the abstract simulator class to allow
                              conways game of life simulation at run time.

briansBrain.h briansBrain.cpp - Has the briansBrain class which inherits from the abstract simulator class to allow
                                brians brain simulation at run time.

wireWorld.h wireWorld.cpp - Has the wireWorld class which inherits from the abstract simulator class to allow
                            wire world simulation at run time.

langtonsAnts.h langtonsAnts.cpp - Has the langtonsAnts class which inherits from the abstract simulator class to allow
                                  langtons ants simulation at run time.

showgen.cpp - contains the main function for showgen and the argument parsing logic.

sim-tui.cpp - contains the main function for sim-tui, ncurses implementation, and the argument parsing logic

grid.cpp grid.h - Has the Qt code for implenting the graphical grid with colors in sim-gui.

optionsDialog.cpp optionsDialog.h - Has the Qt code for implementing the options menu in sim-gui.

sim-gui.cpp - has the main function for sim-gui, and implements the argument parsing logic.
