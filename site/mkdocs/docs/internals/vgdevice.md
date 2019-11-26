# Virtual Graphic Devices

The core of the Guido Engine is platform independent. 
Only the graphic operations and font related operations depend on the host operating system and graphic layers.
In order to cope with each specific platform, the Guido library provides a Virtual Graphic Device layer which is actually a set of pure C++ abstract classes that isolates the engine from the platform dependencies. 

The main advantage is that Virtual Graphic Devices can implement any kind of graphic output: on-screen (platform specific, OpenGL), off-screen (raw bitmaps), files (pdf, svg, postscript), network streams, etc.

## The Virtual Graphic classes

The VG classes are located in the [`src/engine/include`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/include) folder and are the following:

- [VGDevice](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/include/VGDevice.h): declares all methods required by the
Guido Graphic Representation (GGR) to communicate their graphical operations. Implementations of VGDevices are provided by clients applications using derived classes so that neither GGR objects nor any part of the GUIDO Engine depends on a particular 
graphical implementation. A VGDevice derived classe must provide standard graphic functions (Lines, 
Arcs, Boxes,Polygons, Text), coordinate transformations (zoom / scaling), and symbolic music symbols handlers (DrawSymbol method). VGDevice design makes a clear distinction between text characters and music symbols (although music symbols are generally glyphs in a music font). Music symbols are identified by font-independent constants defined in [MusicalSymbols.h](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/lib/MusicalSymbols.h).
- [VGFont](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/include/VGFont.h): provides font related information like glyphs metrics,
- [VGSystem](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/include/VGSystem.h): an abstract layer providing VGDevice and VGFont allocation.

In addition, two utilities are provided: VGColor and VGPen covering respectively colors and pen representations.


## Implementations

### Platform specific

Platform specific implementations of VGDevice are located in the `platforms` folder:

- linux/src: implements a Cairo based device,
- macos/src: implements a Quartz based device,
- win32/src: contains several implementations:
    - a GDI based device
    - a GDI Plus based device
    - a Direct 2D based device

### Platform independent

Platform independent implementations of VGDevice are located in the `src/engine/devices` folder:

- SVG device: produces SVG output
- Abstract device: produces VGDevice commands on output. Actually prints the VGDevice methods followed by their parameters. This output can next be parsed by another component implementing the VGDevice interface, to execute the enclosed methods. This splits the graphics rendering into two distinct parts. This is useful, for example, under Android to communicate between the native layer that calculates the partition and the Java layer that draws the result of this calculation.
- Binary device: similar to the Abstract device but in a more compact binary form.

### Environment dependent

A set of VGDevices are implemented over already platform independent layers and provide compatibility with various high level frameworks. They are located in the `environments` folder:

- [Juce](https://juce.com/): implements a device based on the Juce framework,
- [OpenFrameworks](https://openframeworks.cc/): implements a device over OpenFramework
- [OpenGL](https://www.opengl.org/): implements a device based on OpenGl. Requires also freetype2 and FTGL (OpenGL FreeType fonts) libraries,
- [Qt](https://www.qt.io/): implements a device based on the Qt framework.

## Unsupported implementations
- GDeviceWin2000:         Windows 2000 / XP(gdi+)
- GDeviceGTK:             Linux GTK implementation
- GDevicePostScript:      EPS files (encapsulated postscript)
