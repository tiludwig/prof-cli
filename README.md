# Profiler command line interface

## Eclipse setup

1. Create a new managed C/C++ project
2. Get the files into your project folder:
```
cd eclipse-project-folder
git init
git remote add origin git@github.com:tiludwig/prof-cli.git
git pull -t origin master
```
3. In eclipse click refresh in your created project

Because the program uses dynamic libraries, you need to link against the dl library:
Project Settings->C/C++ Build->Settings->C/C++ Linker->Libraries add **dl**
		
You also need to configure the include path for the project:
Project Settings->C++ Build->Settings->GCC C++ Compiler->Includes add **${workspace_loc:/${ProjName}}**

Now you should be able to compile the project. To run the program, you need to add a config.ini file to the directory, where the executable is located.
The content of the configuration file might look something like this:
```
[core]
	com-driver: serial
	driver-args: /dev/ttyUSB0 9600
[test]
	testname = "Baseline"
	target-name = generic_target
	iterations = 20000
	
	input-method = libgeneric-im.so
	input-args = baseline
[test]
	testname = "Genetic Input"
	target-name = generic_target
	iterations = 100000
	
	input-method = libgeneric-im.so
	input-args = genetic
```