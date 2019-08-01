{!references.ref!}

## Chords

A chord is a list of notes and/or tagged notes, separated by commas and enclosed in braces.

{!BNF/chord.html!}


#### Remarks:
<ul>
All notes of a chord should have the same duration.   
If this is not the case, the duration of the chord is the duration of the longest note within the
chord; all other notes are interpreted as being succeeded by rests which compensate for the
difference between their duration and that of the chord.

The notes within a chords do not have to be ordered with respect to their pitch; however, it is
suggested to consistently write chords in either an ascending or descending order.

If for some note of a chord, duration or octave values are omitted, these are assumed to be
equal to the last respective value specified in the current sequence; this applies also to the case
where this value was specified for a preceeding note of the same chord.
</ul>

### Examples
~~~~~~
{c1/4., e&1/4., g1/4.}   % c minor triad , ascending order   
{c1/4., e&,g}            % same as above, abbreviated notation   
{c#1/4, e#, g#}          % C-sharp major triad, ascending order   
{c2, dis1, a0, fis}      % four-voice chord, descending order   
~~~~~~
{!GMN/chords.html!}

<!-- 
### Chord Sequences
A chord sequence is a generalized note sequence, where instead of individual notes, chords may also
appear.

Remarks:

Chord sequences may contain chords as well as notes and rests.
The chords within a sequence need not all have the same number of notes.

Examples:
\[ {c1/4,e,g} {c,f,a} {d,f,h} {c1/2,g1,c2} ] % progression of triads   
\[ _/8 \slur(c1 d e) {fis/2, ais} e/2 ]  % mixed notes and chords
 -->

