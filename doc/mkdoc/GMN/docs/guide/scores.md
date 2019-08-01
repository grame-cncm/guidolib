{!references.ref!}

## Scores

A score is a sequence of one or several voices denoted by:   
{!BNF/score.html!}

It is assumed that all voices start simultaneously. 
Normally, the voices should have equal durations; if this is not the case, the
duration of the score is the duration of the longest voice; all other voices
are interpreted as if they were succeeded by additional rests which compensate for the difference
between their duration and that of the segment.


#### Remarks:
<ul>
Octave and duration values should be specified at the beginning of each voice; if this is not
done, default values will be assumed when required in the context of a specific application. In
particular, octave and duration values don't carry over from one voice to the next.
</ul>

#### Note:

A single-voice score may be described as a single [voice](voices.md).


### Examples
~~~~~~
{ 
    [ e1/8. \slur(f/16 e/8) d c h0 a/4 ],   % first voice
    [ c1/4 d e/2 ],                         % second voice
    [ a0/4 h \slur(c1/8 h0) a/4 ]           % third voic
}
~~~~~~
{!GMN/score.html!}
