You have 4 samples. Before open an example in your browser, be sure you have the library (libGUIDOEngine.js) in parent directory of html file. You have to copy it in package distribution.

- binaryExport use binary export of GUIDOEngine, parse the binary and draw in html canvas. It use jsCanvasDeviceFromBinary.js to initailize html canvas and jsBinaryDeviceParser.js to execute draw commands.
- svgExport use the SVG export and add the SVG xml code in html page.
- canvasDevice use internal javascript device. This device draw on html canvas. To intialize canvas, use jsCanvasDevice.js like in sample (all variables initialized in jsCanvasDevice.js are use in build-in javascript).
- webComponent is a sample on how to use html5 component for create a new html tag to have a easy integration of guido in html page. A tag guido-viewer is created and integrated in the html page. The draw method used in export svg.

