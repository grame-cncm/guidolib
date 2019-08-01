{!references.ref!}

## Voices

A voice is a sequence of notes, chords and tags of the form:
{!BNF/voice.html!}

where tagged-notes is a series of notes, chords, tags, and tagged ranges separated by white space.


#### Remarks:
<ul>
If duration or octave values are omitted for notes within a sequence, they are assumed to be
equal to the last value specified in the current sequence.

Duration and octave values should be specified at the beginning of each sequence; if this is
not done, they are assigned standard default values when this is required in the
context of an application (such as playback or notation software).
</ul>

### Default values

- duration: 1/4
- octave: 1

### Examples
~~~~~~
[ a1*1 ]                     % a single note   
[ c1/4 d e f g a h c2/2 ]    % C major scale   

% short motive with meter, barlines and a slur (see below):
[ \meter<"4/4"> a0/4 \bar c1/2 \slur(e&/4 c) \bar g/1 ]
~~~~~~
{!GMN/voice.html!}

