# The Guido Project

![score](rsrc/score.png)

The Guido Project is an open source project hosted on [github](https://github.com/grame-cncm/guidolib) that encompasses a music notation format, a score rendering engine and various music score utilities. The [Guido Music Notation Format](https://guidodoc.grame.fr) is a general purpose formal language for representing score-level music in a platform-independent plain text and human readable way. The format comes with various software components for music score rendering and manipulation. The main of these components is the Guido engine, a library that can be embedded on different platforms and using different programming languages.

The Guido Library is an efficient solution to embed music notation in a standalone application or in a web page. Interfaces are available for various programming languages:

- C/C++
- Java
- Javascript
- Python 

It supports different frameworks for graphic rendering:

- Qt
- Juce
- Open Framework
- OpenGL

It runs on the main operating systems and provides native graphic devices:

- Android
- iOS
- Linux
- MacOSX
- Windows

The Guido tools include a Qt editor, various command line utilities and an [online editor](https://guidoeditor.grame.fr/) based on the Web Assembly flavor of the Guido Library.

Compared to existing solutions in the field of music score compilation, the notable feature of the Guido engine is its efficiency and its ability to compute scores in real time.

---------

## Targets

The present web site is intended for: 

- applications **developers** who want to embed symbolic music notation: the Guido API section documents the Guido Engine API and operations;
- **contributors** who want to extend the engine or fix some bug: the Internals section gives some insights into the engine design and implementation. See also the [Contributing](https://github.com/grame-cncm/guidolib/wiki/Contributing) section on the guidolib wiki. 

Note that [Kai Renz's PHD thesis](https://github.com/grame-cncm/guidolib/blob/dev/doc/papers/kai_renz_diss.pdf) constitutes still the most detailled description of the engine design and operations.

The Guido project is an open source project hosted on [github](https://github.com/grame-cncm/guidolib).

---------

## History

The Guido Project originates from the GUIDO NoteViewer project started in the 90th at the Darmstadt University of Technology by 3 PHD students: Holger H.Hoos, Jürgen Kilian and Kai Renz. 
Keith Hamel, the author of [Notability](http://debussy.music.ubc.ca/NoteAbility/) has also significantly contributed to the project.   

The GUIDO NoteViewer became an open source library in December 2002 at the initiative of [Grame-CNCM](http://www.grame.fr). 
