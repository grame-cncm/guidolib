
{!references.ref!}

## Notes and Rests
The general syntax for representing notes is:

{!BNF/notes.html!} 

The corresponding representation for rests is:

{!BNF/rests.html!}


Both notes and rests are instances of GUIDO events; in particular, rests are treated as special notes,
which have a duration but neither accidentals nor an octave value.
Octave and duration can be omitted; if so, they are inferred from preceeding notes (or rests) within
the same sequence or chord or assumed to have standard values when required (such as for musical playback or notation).


### Note names
There are different systems of note names:

- diatonic: `c d e f g a h/b`
- chromatic: `c cis d dis e f fis g gis a ais h/b` 
- solfege: `do re me fa sol la si/ti`

####Remarks:
<ul>
Arbitrarily mixing these systems is possible, but discouraged.  
`h` and `b` denote the same pitch-class (`b` being used in the international system, `h` in the german
system).  
`si` and `ti` denote the same pitch-class (`si` being used in the french and italian, `ti` in the english
solfege system).
</ul>

### Accidentals
Accidentals is an arbitrary sequence of the symbols `#` and `& (for sharp and flat); in particular,

`##` is a double sharp, `&&` a double flat.

####Remarks:
<ul>
Accidentals cannot be used with rests.   
Accidentals are valid only for the note in the description of which the appear.   
The usage of arbitrary sequences of accidentals without a reasonable interpretation in CMN
(such as `#&#`) is discouraged.   
Chromatic pitch-classes are assumed to be different from alterated diatonic ones, i.e., `c# != cis`.
</ul>

### Octave

Octave is an integer number indicating the octave of the note, where `a1` is the 440Hz a. All octaves
start with the pitch-class c, so c1 is the c just below a1.

####Remarks:
<ul>
The usage of octaves beyond the range of -3..+5 is discouraged.   
If octave is omitted from a note description, it is assumed to be identical to the last octave
specified before in the current sequence.   
Octave should always be specified at the beginning of each sequence; if this is not done, but an
octave is required in the context of an application, a standard value of +1 is to be assumed.
</ul>


### Duration

Duration is specified in one of the following forms:

{!BNF/duration.html!}

where enum and denom are positive integers and dots is either empty, '.', or '..'.   
When no dots are present, the relative duration of the notes is assumed to be

- enum/denom beats in case (1);
- enum/1 beats in case (2);
- 1/denom in case (3).

Succeeding the duration by

- one dot '.' is equivalent to multiplying it by 3/2
- two dots '..' is equivalent to multiplying it by 7/4

#### Remarks:
<ul>
Triplets are represented as \*1/3, \*1/6, ...; quintuplets, septuplets and so on are handled analogously.
Durations which are specified by fractions that are equal, such as \*2/4 and \*1/2, are considered
to be different.

Although dotted durations have equivalent dot-less durations, these are considered to be different (\*1/4. is not identical to \*3/8).
If duration is omitted from a note description, it is assumed to be identical to the last duration specified before in the current sequence.
Duration should always be specified at the beginning of each sequence; if this is not done, but a
duration is required in the context of an application, a standard value of 1/4 is to be assumed.
</ul>

#### Note:
Duration may also be specified in milliseconds. The corresponding form is   
`*number'ms'`   
In this case, the actual musical duration is computed by assuming that the tempo is 60 quarter notes per minutes and thus,  1000ms are converted to a quarter note (*1/4).


#### General remarks
<ul>
No white space (blank, newline or tab) are allowed within note descriptions.
</ul>

### Examples
~~~~~~
d1*3/4  c#0/8 h&/6  _/4  c&&/2. cis/4.. fa##1 sol&0 
~~~~~~
{!GMN/notes.html!}
