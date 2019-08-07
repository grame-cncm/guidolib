{!tags.css!}


# Tags 

## Tags variants

Tags may be indicated using shortcuts or alternate names (e.g. \t for \text). These variants are listed in the 'Variants' column of the tag description.

## Tags types
We make the distinction between:

- *range tags*: tags applied to a series of notes (e.g. \slur(a b c)). They have a time position and we can consider that their duration is the duration of the enclosed events.
- *position tags*: tags that have a time position but no explicit time extent (e.g. \meter<"4/4">). These tags can be seen as lasting until otherwise indicated or until the end of the current voice.

The type of a tag (range or position) is indicated in the 'Type' column of the tag description with the letter 'R' for range tags and 'P' for position tags. Note that some tags support both forms; the corresponding type is 'RP'.

## Range tags forms
Almost all the range tags support an alternate *begin_end* form. Let's consider a tag name \xxx, when this form is supported you can write either:

- \xxx ( a f g )
- \xxxBegin a f g \xxxEnd

both forms are equivalent.

The  *begin_end* form supports identifiers as suffixes, used to disambiguate nested or interleaved tags. The corresponding form is:
~~~~~~
\xxxBegin:n .... \xxxEnd:n
~~~~~~
where 'n' is an integer.   
See the [Slurs](/examples/slurs/) example.


## Tags list by section

- [Accidentals](tags/Accidentals.md)
- [Articulations](tags/Articulations.md)
- [Barlines](tags/Barlines.md)
- [Beaming](tags/Beaming.md)
- [Clef Key Meter](tags/ClefKeyMeter.md)
- [Dynamics](tags/Dynamics.md)
- [Header](tags/Header.md)
- [Layout](tags/Layout.md)
- [Miscellaneous](tags/Miscellaneous.md)
- [Notes](tags/Notes.md)
- [Ornaments](tags/Ornaments.md)
- [Repeat Signs](tags/RepeatSigns.md)
- [Tempo](tags/Tempo.md)
- [Text](tags/Text.md)
