------------------------
 GuidoEngine javascript
------------------------

This is the javascript version of the Guido Engine library.

The javascript library is a mapping of the C++ API of guido engine. Most of methods of C++ classes are available in javascript but there may be some differences. You can refers to the C++ documentation of classes and you have specific documentation in doc folder.
Midi is not yet supported in javascript, so all methods related to midi are not available.
The onDraw methods are not available in javascript, you should use export methods (e.g. rendering in SVG).
The performance of each method (SVG, parse binary or canvas device) are not the same on all browser. SVG is generally faster but on chrome it's the javascript device.

You'll find different samples of use in the examples folder.

A Typescript interface is also provided (see the typescript folder)

