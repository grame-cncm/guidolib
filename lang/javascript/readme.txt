Note for javascript:
--------------------
To build the javascript version of guido engine, you must have emscripten sdk installed on your computer (see http://kripken.github.io/emscripten-site/).
Go in javascript folder and do :
	make
libGUIDOEngine.js is generated.

You have different samples in examples folder.
The performance of each method (SVG, parse binary or canvas device) are not the same on all browser. SVG is generally faster but on chrome it's the javascript device.

The javascript library is a mapping of the C++ API of guido engine. Most of methods of C++ classes are available in javascript but there may be some differences. You can refers to the C++ documentation of classes and you have specific documentation in doc folder.
Midi is not yet ported in javascript, so all related methods to midi are not available.
The onDraw methods are not available in javascript, you can use export methods.
GUIDOPianoRollAdapter.getMap is ported to javascript in GuidoScoreMap.getPianoRollMap to return json string.

A lot of C data structure and enum are available. You can see source file in src/binding folder and html file in examples folder.
Embind module of emscripten is used. You can refer to embind emscripten documentation on how to use mapped classes structures and enum.

