{!tags.css!}

# Articulations


## \accent

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \accent | \acc | R | accent sign |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| position | string | above or below | *none* | true |

Accent position is determined by the stem direction: the accent should be placed on the opposite side of the stem. When no stem is present, the accent is placed above.

See the [Articulations](../../../examples/articulations/) example.





<br />


## \breathMark

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \breathMark |  | P | a breath mark |

See the [Articulations](../../../examples/articulations/) example.




<br />


## \fermata

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \fermata |  | RP | fermata signs |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| type | string | short, regular or long | regular | true |
| position | string | above or below | above | true |

See the [Articulations](../../../examples/articulations/) example.





<br />


## \glissando

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \glissando | glissandoBegin glissandoEnd | R | a glissando line between notes |

The **\glissando** tag displays lines between notes but can be applied to chords as well.
In the latter case, a line is drawn between the corresponding notes of each chord.
It is the position of the notes in the GMN code that determines the correspondence



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| dx1 | unit | displacement of the line left anchor point | 0 | true |
| dy1 | unit | displacement of the line left anchor point | 0 | true |
| dx2 | unit | displacement of the line right anchor point | 0 | true |
| dy2 | unit | displacement of the line right anchor point | 0 | true |
| fill | boolean@param | fill space between lines when applied to a chord | false | true |
| thickness | unit | line thickness | 0.3 | true |

See a [glissando](../../../examples/articulations/) example.





<br />


## \marcato

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \marcato |  | RP | marcato sign |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| position | string | above or below | above | true |

Note that depending on the position, the glyph for the marcato sign is not the same.

See the [Articulations](../../../examples/articulations/) example.





<br />


## \pedalOn

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \pedalOn |  | P | a pedal indication |







## \pedalOff

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \pedalOff |  | P | a pedal indication |









<br />


## \pizzicato

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \pizzicato | \pizz | R | pizzicato signs |

The **\pizzicato** tag supports various shapes, intended to different instruments.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| type | string | buzz, snap, bartok, fingernail, lefthand | lefthand | true |
| position | string | above or below | above | true |

Note that snap and bartok types are equivalent.

See the [Articulations](../../../examples/articulations/) example.





<br />


## \slur

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \slur | \sl <br />slurBegin \slurEnd | R | slur |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| curve | string | up or down | *none* | true |
| dx1 | unit | displacement of the left anchor point | 2hs | true |
| dy1 | unit | displacement of the left anchor point | 1hs | true |
| dx2 | unit | displacement of the right anchor point | -2hs | true |
| dy2 | unit | displacement of the right anchor point | 1hs | true |
| r3 | float | displacement of the inflection point | 0.5 | true |
| h | unit | height of the curve | 2hs | true |

The **r3** parameter should be between 0 (leftmost) and 1 (rightmost).

See the [Slurs](../../../examples/slurs/) example.





<br />


## \staccato

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \staccato | \stacc <br /> \staccBegin \staccEnd | R | staccato signs |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| type | string | heavy or regular | regular | true |
| position | string | above or below | *none* | true |

The default staccato position is close to the note head, on the opposite side of the stem.

See the [Articulations](../../../examples/articulations/) example.





<br />


## \tenuto

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \tenuto | \ten | R | tenuto sign |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| type | string | heavy or regular | regular | true |
| position | string | above or below | *none* | true |

The default tenuto position is close to the note head, on the opposite side of the stem.

See the [Articulations](../../../examples/articulations/) example.



