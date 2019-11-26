# Abstract classes

Abstract classes describe the music representation at logical level and are used to handle musical tag.
Each [tag](https://guidodoc.grame.fr/refs/tags/) has a corresponding abstract class, which name starts with `AR` and which is implemented in a file that carries the name of the class. For example, the `\meter` tag is described by the `ARMeter` class, which is implemented in the [`ARMeter.cpp`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/ARMeter.cpp) and [`ARMeter.h`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/ARMeter.h) files.


The main purpose of the AR classes is to store and possibly pre-process the tag parameters. 

------------

## Main musical concepts

Almost all the AR classes represent a Guido tag. However, the main musical concepts (like notes or voices) are not based on tags but are also represented using AR classes:

- [ARMusic](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/ARMusic.h): represents the [score](https://guidodoc.grame.fr/guide/scores/), i.e. a set of voices,
- [ARMusicalVoice](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/ARMusicalVoice.h): represents a musical [voice](https://guidodoc.grame.fr/guide/voices/). Note that [AR to AR operations](#ar-to-ar-operations) are implemented by the ARMusicalVoice class,
- [ARNote](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/ARNote.h): represents notes.

------------

## Utilities

- [ARFactory](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/ARFactory.h): this class is in charge of the creation of all the musical elements (voices, notes, tags,...). It is used either by the parser of by the [GUIDOFactory API](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/include/GUIDOFactory.h).  
- [ARMusicalVoiceState](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/ARMusicalVoiceState.h): used to maintain the current voice state at a given location when traversing the voice. For example, a voice state contains the list of opened tags at a given voice position.

------------

## AR to AR operations

AR to AR operations enrich the abstract representation with elements that can be automatically computed from the current descrption (e.g. like bars). They consists also in a set of logical layout operations (e.g. compute the beam direction when not set). Theses operations are implemented in [ARMusicalVoice](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/ARMusicalVoice.h) by a set of methods named `doAuto...`:

- doAutoKeys(): traverses the voice and look for key changes. Whenever a key-change occurs, the previous key needs to be 'naturalized',
- doAutoDispatchLyrics(): converts lyrics tags into individual text-tags for all events within the range,
- doAutoTies(): traverses the voice and split ties so that there are always TWO events within a tie-Range,
- doAutoCheckStaffStateTags(): makes sure that clef/key/meter-information is set,
- doAutoDisplayCheck(): checks whether the events in the voice can be displayed as single graphical objects, 
- doAutoBarlines(): add automatic barlines, breaks notes if needed, 
- doAutoMeasuresNumbering(): add mesaures numbering,
- doAutoEndBar(): add end bar (unless prevented with an [`auto`](https://guidodoc.grame.fr/refs/tags/Miscellaneous/) tag),
- doAutoBeaming(): add automatic beaming tags, 
- doAutoGlissando(): post-processing for the [`glissando`](https://guidodoc.grame.fr/refs/tags/Articulations/#glissando) tags,
- doAutoFeatheredBeam(): post-processing for the [`fbeam`](https://guidodoc.grame.fr/refs/tags/Beaming/#fbeam) tags.

