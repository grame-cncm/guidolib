{!tags.css!}

# Ornaments


## \arpeggio

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \arpeggio |  | R | an arpeggio sign |

The **\arpeggio** tag displays arpeggio signs and should be applied to chords only.
It has no effect on single notes.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| direction | string | up or down | *none* | true |

- up: to display an up arrow
- down: to display a down arrow

See the [Articulations](../../../examples/articulations/) example.





<br />


## \mordent

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \mordent | \mord | R | a mordent |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| note | string | a note, part of the ornament | *none* | true |
| type | string | prall, prallprall, inverted or prallinverted | prall | true |
| detune | float | note tuning (for microtonality) | 0.0 | true |
| accidental | string | cautionary or force | *none* | true |
| adx | unit | accidental displacement (if any) | 0hs | true |
| ady | unit | accidental displacement (if any) | 0hs | true |
| position | string | above, below | above | true |

- **note** is intended to indicate accidentals, the note itself is not displayed but the possible accidental does.
- **detune** is applied to the **note** parameter and works similarly as [\alter](../Accidentals#alter)
- **accidental** is intended is intended to control how the accidental is displayed

See the [Articulations](../../../examples/articulations/) example.



## \trill

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \trill | \trillBegin \trillEnd | R | a turn ornament |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| note | string | a note, part of the ornament | *none* | true |
| detune | float | note tuning (for microtonality) | 0.0 | true |
| accidental | string | cautionary or force | *none* | true |
| adx | unit | accidental displacement (if any) | 0hs | true |
| ady | unit | accidental displacement (if any) | 0hs | true |
| begin | boolean | for trill continuations | on | true |
| tr | boolean | show/hide the leading "tr" string | true | true |
| position | string | above, below | above | true |

- **begin** is intended for trill continutation. When false, the trill continues the preceding one (if any). It allows to write new trills with changing accidentals without breaking it.
- **tr** show or hide the leading "tr" string

other parameters have the same effect as for [\mordent](#mordent)

See the [Articulations](../../../examples/articulations/) example.



## \turn

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \turn |  | R | a turn ornament |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| note | string | a note, part of the ornament | *none* | true |
| type | string | inverted or regular | regular | true |
| detune | float | note tuning (for microtonality) | 0.0 | true |
| accidental | string | cautionary or force | *none* | true |
| adx | unit | accidental displacement (if any) | 0hs | true |
| ady | unit | accidental displacement (if any) | 0hs | true |
| position | string | above, below | above | true |

The parameters have the same effect as for [\mordent](#mordent)



