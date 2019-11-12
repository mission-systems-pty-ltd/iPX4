# MOOS-iPX4

An interface between MOOS and the PX4 flight controller enabling telemetry translation and off-board control.

## Installation
The MOOS-iPX4 package requires the following dependencies to run:
- MOOS-IvP (see below)
- Mavlink (see below)
- Boost [boost]
- pThreads library [pthread]
- C Math library [m]

### MOOS-IvP Installation
Check if MOOS-IvP is installed (e.g. `which pAntler` should return output similar to `/usr/local/bin/pAntler`). If MOOS-IvP is already installed, skip this step.

The following instruction set should be adequate to install MOOS-IvP on most Linux distributions:
```shell
$ svn co https://oceanai.mit.edu/svn/moos-ivp-aro/releases/moos-ivp-17.7.2 moos-ivp
```
(Note: at time of writing, moos-ivp 17.7.2 was the most recent stable release, check http://oceanai.mit.edu/moos-ivp/pmwiki/pmwiki.php?n=Site.Download for the most stable release, and see other information about the MOOS-IvP software)

Now build MOOS-IvP using the following commands (`sudo` priviledges may be required):
```shell
$ cd moos-ivp
$ sudo ./build.sh
```
(Check successful installation using `which pAntler`, which should return output similar to `/usr/local/bin/pAntler`)

### Mavlink Installation
The following is the instruction set for Linux (Ubuntu) installation. Mavlink requires python 2.7+ or 3.3+, python-future module and (for the GUI) python's TkInter module, which have the following installation instructions.

The recommendation from the Mavlink website is to use python pip to install future (even with changes to PATH), however this did not work on Ubuntu 18.04, however `apt` did:
```shell
 $ sudo apt install python-future
```
(Alternative as listed on Mavlink installation instructions:)
```shell
 $ sudo apt-get install python3-pip
 $ pip install --user future
```

To install TkInter, use the following:
```shell
 $ sudo apt-get install python-tk
```

Clone Mavlink recursively:
```shell
 $ git clone https://github.com/mavlink/mavlink.git --recursive
```
(Alternatively this can be done in two lines:)
```shell
 $ git clone https://github.com/mavlink/mavlink.git
 $ git submodule update --init --recursive
```

Set PYTHON_PATH to the directory path containing your mavlink repository. Do this by adding this line to your `~/.bashrc` file:
`PYTHONPATH=${PYTHONPATH}:~/your/path/to/mavlink`

Now you must generate the Mavlink library. Inside the `mavlink` directory:
```shell
 $ mkdir include
 $ python -m mavgenerate
```
This launches the simple mavgenerate GUI (see Mavlink website if you do not wish to do this). Select the following options in the GUI:

* choose `.xml` file, recommended: `.../mavlink/message_definitions/1.0/common.xml` (if you choose a different .xml file, then the CMakeLists.txt will not compile correctly)

* choose output location, `.../mavlink/include` which you just created

* choose language, C++

* choose version, 2.0

* tick both "validate" and "validate units"

* click generate


Ensure all `.h` files were generated correctly into the include directory.

### Install iPX4
Clone the directory and move to that directory.

Fun the following commands to build the application:
```shell
 $ mkdir build && cd build
 $ cmake ..
 $ make
```

Environmental setup requires three environment variables to be set, by adding `export VARIABLE_NAME=/path/to/directory` to your `~/.bashrc` file:
- `$ENV{MOOSIVP_SOURCE_TREE_BASE}` which should point to your moos-ivp directory
- `$ENV{MAVLINK_HOME}` which should point to your mavlink directory
- `$ENV{BIN_HOME}` which should point to where you would like your `iPX4` executable to be built to

## Usage
To see how the application is run, use the following commands:
```shell
 $ ./iPX4 -h
```
(running `./iPX4` or `.iPX4 --help` has the same display)

Basic usage of the MOOSApp is as follows:
```shell
 $ ./iPX4 file [OPTIONS]
```
