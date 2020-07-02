
# Ember Editor [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![Build status](https://ci.appveyor.com/api/projects/status/47kvcru66wge4vas/branch/develop?svg=true)](https://ci.appveyor.com/project/L4ZZA/pyro/branch/develop)

> A small piece of burning or glowing coal or wood in a dying fire.

Real-time 2D level editor using Pyro engine.

### How to build code

- Clone the repository using `git clone --recurse-submodules -j8`
    - `recurse-submodules` will automatically initialize and update each submodule.
    - `j8` tells git to use different _jobs_ to download and updated the submodules, hence makes it faster.
- Generate the solution running `scripts/generate_solution_*.bat` [double-click or run from cmd].
- Open `pyro.sln` solution just generated and build usin Visual Studio [other build systems are not supported yet].
- Run the solution (press F5).
[WARNING: if you want to run the application from the output folder you'll need to copy the assets folder from the project directory and paste in the same folder of the executable]

### How to build the installer

> **Requirements**
> - Install [NSIS: Nullsoft Scriptable Install System](https://sourceforge.net/projects/nsis/) to its default location `C:\Program Files (x86)\NSIS`
> - Add the install directory to your system path like so:
    ![image](https://i.imgur.com/cTZquD7.png)

- Run the `create_installer.bat`, thie will generate the installer in the bin directory.
- go to `bin\Release-windows-x86_64\installers` and run `LevelEditorSetup.exe`.
