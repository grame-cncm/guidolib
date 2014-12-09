Note for javascript:
--------------------
To build the javascript version of guido engine, you must have emscripten sdk installed on your computer (see http://kripken.github.io/emscripten-site/).
Go in javascript folder and do :
	make
libGUIDOEngine.js is generated.

You have different samples :
- canvas.html use binary export of GUIDOEngine, parse the binary and draw in html canvas.
- classSvg.html use the SVG export and add the SVG in html page.
- guido.html is a complete test of the javascript GUIDOEngine API.
- webComponent.html is an sample on how to use html 5 component for create a new html tag. A tag guido-viewer is created and integrated in the html page.
- canvasDevice.html use internal javascript device. This device draw on html canvas. To intialize canvas, use jsCanvasDevice.js like in sample (all variable are use in build-in javascript).

The performance of each method (SVG, parse binary or javascript device) are not the same on all browser. SVG is generally faster but on chrome it's the javascript device.

The javascript library is a mapping of the C++ API of guido engine. Most of methods of C++ classes are available in javascript. You can refers to the C++ documentation.
Midi is not yet ported in javascript, so all related methods to midi are not available.
The onDraw methods are not available in javascript, you can use export methods.

A lot of C data structure and enum are available. You can see source file in binding folder.

