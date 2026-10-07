# Pash
## The best software for micro computer formatting/migration.
Pash is a software for Linux wich you can install all your programs installed by your main machine on other machines, just having the same package managers.
### About the name:
Pash is the mix of Package with Bash, it means that it makes a Bash script with installs all your programs installed with package managers
### Supported package managers:
apt
**renember comming here latter, this program evolues fast, lately will have more package managers available!**
### How to use:
you can Just exec without flags, for make a universal installer in the Pash Executable directory.
#### Flags:
**--split** - splits the installation: every package manager has your own Bash script.

**--dir** - choose a dir for putting the shell scripts
### Build
#### Dependencies 

**1- c++ 1 compiler7**

**2- CMake 3.16 or later**

**3- QT6 core**

#### Build
for build, open your linux terminal **(on windows use WSL)** and run this commands:

```
chmod +x build.sh
./build.sh

```
## How to add support to new package managers

If you want to contribute by adding support for a new package manager, create two files:

```text
[package manager]Split.inc
[package manager]NonSplit.inc
```

Put the implementation of the package manager inside the corresponding file.

### Split mode

The `[packate manager]Split.inc` file is responsible for creating **two files**:

1. A temporary file containing the list of packages installed by the user.
2. A Bash script containing the commands required to install those packages.

For example, for APT:

```
APTLIST.tmp
apt.sh
```

The package manager command should list the packages installed manually by the user.

For APT, this command is:

```
apt-mark showmanual
```

The output should be redirected to the list file:

```
apt-mark showmanual > APTLIST.tmp
```

The implementation should create the paths using `std::filesystem::path`:

```
std::filesystem::path listPath =
    std::filesystem::path(pathstr) / "APTLIST.tmp";

std::filesystem::path scriptPath =
    std::filesystem::path(pathstr) / "apt.sh";
```

Then execute the package manager's command:

```
std::string cmd =
    "apt-mark showmanual > '" + listPath.string() + "'";

if (std::system(cmd.c_str()) != 0) {
    qDebug() << "apt-mark failed";
}
```

After the list has been generated, open both files:

```
std::ifstream listFile(listPath);
std::ofstream aptList(scriptPath);
```

The script must start with the Bash shebang:

```
aptList << "#!/bin/bash\n";
```

Then read the package list line by line using `std::getline`:

```
std::string line;

while (std::getline(listFile, line)) {
    aptList << "sudo apt install -y " << line << '\n';
}
```

The result will be a script containing one installation command for each package.

For example, if the list contains:

```
cmake
g++
qt6-base-dev
```

the generated script will contain:

```
#!/bin/bash
sudo apt install -y cmake
sudo apt install -y g++
sudo apt install -y qt6-base-dev
```

Finally, close the files and remove the temporary package list:

```
listFile.close();
aptList.close();

std::filesystem::remove(listPath);
```

### Split implementation requirements

A new `Split.inc` implementation should therefore:

1. Determine how to list packages installed by the user.
2. Create a temporary list file.
3. Run the package manager's list command.
4. Open the list file.
5. Create the installation script.
6. Write `#!/bin/bash` as the first line.
7. Read the package list using `std::getline`.
8. Generate the appropriate installation command for each package.
9. Close both files.
10. Remove the temporary list file.

The generated installation command depends on the package manager.

For example:

```
APT     → sudo apt install -y <package>
Pacman  → sudo pacman -S <package>
DNF     → sudo dnf install <package>
```

### Non-split mode

The `[package manager]NonSplit.inc` file is used when the `--split` option is **not** enabled.

Unlike split mode, the implementation should generate a single automation script containing the required commands for the package manager.

The exact implementation depends on how the package manager handles package lists and installation.

### File naming

Use the package manager name when naming the files.

For example:

```
aptSplit.inc
aptNonSplit.inc

pacmanSplit.inc
pacmanNonSplit.inc

dnfSplit.inc
dnfNonSplit.inc
```

Keep the implementation specific to the package manager inside these files. This keeps the main Pash source cleaner and makes adding support for new package managers easier.


