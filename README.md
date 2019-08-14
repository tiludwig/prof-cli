# Profiler command line interface

## Eclipse setup

1. Create a new managed C/C++ project
2. Get the repository into the project folder:
```
git init
git remote add origin git@github.com:tiludwig/prof-cli.git
git pull -t origin master
```
3. In eclipse click refresh in your create project

Because the program uses dynamic libraries, you need to link against the dl library
		Project Settings->C/C++ Build->Settings->C/C++ Linker->Libraries add **dl**
		
You also need to configure the include path for the project
		Project Settings->C++ Build->Settings->GCC C++ Compiler->Includes add **${workspace_loc:/${ProjName}}**
		