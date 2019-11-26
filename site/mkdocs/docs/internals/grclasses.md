# Graphic classes

Graphic classes are graphic instances of the [abstract classes](/internals/arclasses), i.e. notation elements with concrete graphic attributes (position, size, etc.). 
Almost each abstract class has a corresponding graphic classe which is implemented in a file that starts with 'GR' and that carries the name of the class. For example, the [`GRMeter.cpp`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRMeter.cpp) file implements the graphic instance of the `ARMeter` class, that handles the `\meter` tag.

## Structure of the GR

- **Class [GRMusic](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRMusic.h)**: one instance of this class is created for every score. GRMusic contains one or more instances of class GRPage, which represent the pages of the score. To create the pages and lines of a score, GRMusic uses the GRStaffManager class.
- **Class [GRPage](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRPage.h)** contains one or more instances of class GRSystem. An instance of class GRPage knows about its height and width and also about the size of its margins. Some graphical elements of the score, like for instance the visible title and composer of a piece are also controlled by class GRPage.
- **Class [GRSystem](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRSystem.h)** represents a line of music in the score. It is made up of one or more instances of class GRSystemSlice. When the line breaking algorithm has decided which slices belong to a system, class GRSystem must adjust the spacing of the notation elements, and must also adjust its extent to match the desired line width.
- **Class [GRSystemSlice](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRSystemSlice.h)** is a part of a single line of music. A set of GRSystemSlices build a GRSystem. It is convenient to think of system slices as measures of a score. Class GRSystemSlice contains one or more instances of class GRStaff. GRSystemSlice is also responsible for graphical elements that belong to several staves; this might be, for example, a beam that begins and ends in different staves.
- **Class [GRStaff](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRStaff.h)** represents a part of a single staff of a line of music. The time position and duration of the part being represented by an instance of class GRStaff is directly determined by the containing GRSystemSlice. Class GRStaff directly inherits from class GRCompositeNotationElement, which is capable of storing an arbitrary number of instances of class GRNotationElement. Using this storage, class GRStaff stores graphical elements being placed upon it. This might be musical markup, like for example an instance of class GRClef or a musical event like an instance of class GRNote.

The main classes involved in the transformation of AR to GR are the 
[`GRStaffManager.cpp`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRStaffManager.cpp) and the 
[`GRVoiceManager.cpp`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRVoiceManager.cpp)


## Main classes

- **Class [GRTag](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRTag.h)**: the base class for all GUIDO-tags. 
- **Class [GRPositionTag](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRPositionTag.h)**: the base class for representing [range tags](https://guidodoc.grame.fr/refs/tags/) (i.e. tags applied to a series of notes, e.g. \slur(a b c). The issue of line breaking is crucial when dealing with range tags: if both the begin- and end-event of a range tag are located on one line of music, the graphical object (e.g. a slur) can be created directly. If the begin- and end-event of a range-tag are located on different lines, then the graphical object must be split into several graphical objects. A mechanism within class GRPositionTag has been implemented to deal with this case.
- **Class [GNotationElement](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GNotationElement.h)**: the base class for all graphical elements. It covers elements that corresponds to an AR object with the derived GARNotationElement class, as well as elements that have no explicit description (like beams, stems, notehead, etc.)
- **Class [GRARNotationElement](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRARNotationElement.h)**: the base class for all graphical objects that have a direct counterpart in the Abstract Representation (which is shown by using “AR” as part of the name). Consider, for example, class GRClef, which inherits indirectly from class GRTag and from class GRARNotationElement. This reflects, that the graphical “clef ” object has a direct counterpart in the AR (which obviously is an instance of the class ARClef).

### Note
Within the GR representation, all references to an AR object are const. Thus a GR object can't modify the AR representation.