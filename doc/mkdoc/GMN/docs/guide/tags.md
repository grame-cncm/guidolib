{!references.ref!}

## Tags and Tagged Ranges

Tags are used to represent additional musical information, such as slurs, clefs, keys, etc. A tag has
one of the forms

1. `\id`
2. `\id<param-list>`
3. `\id( note-series )`
4. `\id<param-list> ( note-series )`

where  

- `id` is one of a number of predefined tag-names,
- `param-list` is a list of string or numerical arguments, separated by commas (','),
- `note-series` is a series of [note](notes.md) descriptions or [chord](chords.md) descriptions and tags separated by white space (spaces, newlines, and/or tabs).

The general syntax for tags is:

{!BNF/tags.html!}


#### Remarks:
<ul>
In general, tag-structures can be arbitrarily nested, since the note-series in (3) and (4) may
contain other tags.

The tags which are defined in Basic GUIDO are described in detail in Section 2.7 (see below).
Tags of the from (1) and (2) are used for elements of CMN like clefs, meter, key, etc.   
Tags of the from (3) and (4) are called tagged ranges, they define properties of series of notes
such as slurs, beams, etc.  

Generally, tags have no duration; however, tagged ranges can be thought of extending over the
duration of the note series they are associated with.
</ul>

### Examples
~~~~~~
\bar                                % barline
\clef<"g2">                         % treble clef
\slur(c1/4 d e)                     % slurred group
\text<"Motive A">(g1/2 f#/4 g g#/2) % text
~~~~~~

