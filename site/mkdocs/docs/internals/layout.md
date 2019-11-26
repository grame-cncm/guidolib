# Global steps of the layout

Building the graphic representation is a complex task that involves spacing and line breaking, which depends on the elements bounding box and determines their horizontal position. Each element has also to compute its specific graphic properties (e.g. a curve for slurs or ties).

## Spacing and line breaking

Spacing algorithm is based on a [spring-rod model](https://github.com/grame-cncm/guidolib/blob/dev/doc/papers/renz-spacing.pdf) and line breaking algorithm is derived from [Donald Knuth](http://www.eprg.org/G53DOC/pdfs/knuth-plass-breaking.pdf).
In practice, the implementation of these algorithms suffers from _old-fashioned_ coding: 

- non-structured methods that are 200 to 500 lines long, 
- lack of convention to differentiate local variables and class fields, 
- non-functional implementation, in the form of a state machine.

All this makes it almost impossible to modify these algorithms although they are perfectly described in [Renz's thesis](https://github.com/grame-cncm/guidolib/blob/dev/doc/papers/kai_renz_diss.pdf). The core of the spacing and line breaking is in charge of the [GRStaffManager](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRStaffManager.cpp).


## Position and bounding box

The common properties of all the graphic elements are their bounding box and their position.
Computing these properties can be complex, in particular due to line breaks that can break an element into several parts, and also because of collisions that can occur with other elements. 
Only range tags are concerned with the former issue but all elements may have to face the latter.

Note that originaly, the Guido Engine didn't made any provision to solve collisions but the Guido language was designed to solve them manually with [common tag parameters](https://guidodoc.grame.fr/refs/tagsparams/#common-parameters) like _dx_ or _dy_, used for relative displacement of an element. The various uses of the Guido engine have gradually led to the inclusion of automatic systems for collision avoidance.

### Range tags
The main method to compute the element graphic properties is **tellPosition** that each object inherits from GObject. The method interface is the following:
~~~~~~~~
virtual void tellPosition( GObject * caller, const NVPoint & position );
~~~~~~~~
**tellPosition** is called twice with the first and last object of the tag range and with the corresponding object position.
In addition and when a tag is split by a line break, the method is called for each segment. In this case, a **[GRSystemStartEndStruct](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRPositionTag.h)** can be used to collect information about the segments. In particular it contains a `startflag` and an `endflag` that indicates the status of the current segment (between LEFTMOST, RIGHTMOST,	OPENLEFT, OPENRIGHT, NOTKNOWN).

The **GRSystemStartEndStruct** must be allocated by the object constructor and added to the object _start-end_ list using `addSystemStartEndStruct`. It can be retrieved using `getSystemStartEndStruct(const GRSystem * grsystem)`.

### Other tags
The horizontal position of non-range tags is automatically set by the engine. The vertical position is to be computed by the object. Computing the bounding box mainly consists in getting glyphs extends.

## Automatic collision avoidance

All the elements that inherits **[GRNotationElement](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/graphic/GRNotationElement.h)** have access to the list of associated GRNotationElement using `getAssociations()`. This list is typically associated to range tags and contains all the elements of the range (e.g. the notes covered by a slur).
Typically, the bounding box of associated elements is used to detect and solve a collision.

Non-range tags have no context and thus, they need to build their own contextual information in order to handle potential collisions.


## Drawing

Each element is responsible to re-implement the `OnDraw` method which interface is the following:
~~~~~~~~
virtual void OnDraw( VGDevice & hdc ) const;
~~~~~~~~
The method is const and thus, all the computation regarding the layout has to be already performed before `OnDraw` is called.
The default behaviour is to draw a music symbol for symbol based elements.

Elements that have been segmented by a line break have access to the current drawing state using their `GRSystemStartEndStruct`.

The method takes a [Virtual Graphic Device](/internals/vgdevice/) as argument.
