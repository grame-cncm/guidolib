# Source code organisation

The source code of the Guido Engine is located in the `src/engine` folder.
This folder is organised as follows:

| **Name** | description |
|--------|-------------|
| [**abstract**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract) | contains all the [abstract representation (AR) classes](/internals/arclasses) and tags parameters management classes. The file names for AR classes starts with **AR**. The file names for tags management classes starts with **Tag** |
| [**alt-rep**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/alt-rep) | contains classes that implement alternate representations (PianoRoll and Reduced Proportional representations) |
| [**devices**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/devices) | contains the platform independent [graphic devices](/internals/vgdevices) like the SVG or the Abstract devices. Note that platform dependant devices are located in the `platforms`  or in the `environments` folders. |
| [**graphic**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic) | contains all the [graphic representation (GR) classes](/internals/grclasses) and some various utilities. The file names for GR classes starts with **GR**. |
| [**include**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/include) | contains the public header files. |
| [**lib**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/lib) | contains the C++ interface implementation and various utilities.  |
| [**maps**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/maps) | contains the time to graphic mapping implementation |
| [**misc**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/misc) | contains various shared utilities. |
| [**operations**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/operations) | contains operations on AR or GR representations. |
| [**parser**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/parser) | contains the Guido Music Notation parser definition and implementation. |
| [**tools**](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/tools) | various utilities mainly for debug purpose. |

