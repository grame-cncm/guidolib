# <a id=top></a> Guido Engine - Validation tests

- [Principle of validation](#principle)
- [Requirements](#require)
- [Graphical output generation](#output)
- [Validation](#validation)
- [Typical validation cycle](#cycle)


## <a id=principle></a> Principles of validation

The validation tools included in this folder are based on the Guido Engine graphical output, generated as SVG files. Thus the validation check that the graphical output remains visually the same from one version of the engine to another.
To operate, the system has to be bootstrapped with a reference version. Next and for each set of modifications of the engine, you have to generate a new graphical output, which is then compared to the reference version. Once the new output is validated, it may become the new reference version.

##  <a id=require></a> Requirements
`guido2svg`, `guido2proll` and `guidogettimemap` must be available from the command line. These tools are compiled by the main engine compilation target (i.e. when you run `make` from the `build` folder). They are located in the `build/bin` folder.

Common Unix tools are also required : `make` and `diff`


## <a id=output></a> Graphical output generation  

Simply type:
~~~~~~~~~
> make
~~~~~~~~~
and you'll find a set of svg files in a folder named with the current engine version.  
Note that this version is taken from the `guidoversion.txt` file located in this folder. The version number can be completely arbitrary and you can freely modify `guidoversion.txt` according to your needs.

### Input files
Input files are all the files located in the following folders:
- gmn-examples
- open-issues
- regression-tests

### Output files
Output files are located in the a folder named with the content of the `guidoversion.txt` file (e.g. 1.7.0.0).
The input hierarchy is preserved thus you should find the following folders in your output folder:
- gmn-examples
- open-issues
- regression-tests  

plus 3 additional folders:
- lyrics: contains files generated with lyrics special option.
- maps: contains the whole set of input files generated with graphical mappings
- proll: contains a set of piano roll representations

### Make options
The full output of `make` represents more than 2700 files, which takes a bit time to generate. In a development phase, it is safe to ignore some input files, unless you know that your changes may affect these files.
A specific target is provided for that:
~~~~~~~~~
> make svgshort
~~~~~~~~~
It ignores the graphical mapping files and the piano roll files.

#### Note
You can safely parallelize the run of make e.g.:
~~~~~~~~~
> make -j 8
~~~~~~~~~

And for all make options type:
~~~~~~~~~
> make help
~~~~~~~~~


##  <a id=validation></a> Validation
To validate the current version, run:
~~~~~~~~~
> make validate VERSION=_your_reference_version_
~~~~~~~~~
It will compare all the file from the current version with the reference version (given as argument).
When there is a difference, a message is displayed to open the corresponding files e.g.  
~~~~~~~~~
open 1.7.1.0/regression-tests/issues/g-issue110.svg 1.7.0.0/regression-tests/issues/g-issue110.svg # to check changes
~~~~~~~~~
Depending on your system and settings, running the line will open both files (usually in a browser and in 2 different tabs), allowing you to visually compare the output and possibly detect false positives.

For new files the following message will be displayed:
~~~~~~~~~
open 1.7.6.0/maps/system/regression-tests/issues/g-issue158.svg  # new file
~~~~~~~~~

Note that you can also parallelize the validate target.


## <a id=cycle></a> Typical validation cycle

When you don't have a reference version, start by generating one. To ensure that this version is correct, you must use an unmodified version of the Guido engine. If you have major changes in progress, you can generate this reference version from a fresh checkout of the project ('dev' branch).
Then:
- edit the `guidoversion.txt` file and change the version number: e.g. move 1.7.0.0 to 1.7.0.1
- generate a new output: e.g. `make -j 8` or `make svgshort -j 8` 
- compare with your reference version: e.g. `make validate VERSION=1.7.0.0`
- and carefully check the output of the validate target.

----
[Top](#top)



