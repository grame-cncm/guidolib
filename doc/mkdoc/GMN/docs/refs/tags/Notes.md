{!tags.css!}

# Notes


## \cluster

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \cluster |  | R | transform chords into clusters |

The **\cluster** takes the highest and lowest notes of a chord and transforms the range into a cluster



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| hdx | unit | displacement relative to the chord | 0hs | true |
| hdy | unit | displacement relative to the chord | 0hs | true |






<br />


## \cue

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \cue |  | R | cue notes |

Regarding the notes, the **\cue** tag is equivalent to **\noteFormat<color="blue", size=0.6>** tag.
Cue notes are colored in blue by default. You can override that with  [**\noteFormat**](#noteFormat)



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| name | string | the cue description | *none* | true |
Supports [font parameters](../../tagsparams#text-parameters)






<br />


## \displayDuration

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \displayDuration | \dispDur | RP | display an arbitrary duration for a note |

The duration to display is indicated by a rational value. Provision is also made to display dots.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| n | integer | the duration numerator | *none* | false |
| d | integer | the duration denominator | *none* | false |
| ndots | integer | the number of dots | 0 | true |






<br />


## \dotFormat

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \dotFormat |  | RP | control the dots format |

The **\dotFormat** tag supports both a range and a position form.
For the latter, the format is applied up to the next format specification or to the end of the voice.

**\dotFormat** is a way to introduce [common parameters](../../tagsparams#common-parameters) to dots.




<br />


## \grace

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \grace |  | R | display grace notes |





<br />


## \harmonic

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \harmonic |  | R | an harmonic sign |









<br />


## \noteFormat

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \noteFormat |  | RP | control the notes format |

The **\noteFormat** tag supports both a range and a position form.
For the latter, the format is applied up to the next format specification or to the end of the voice.

**\noteFormat** is a way to introduce [common parameters](../../tagsparams#common-parameters) to notes.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| style | string | the notehead style (see below) | standard | true |

The notehead style should be among "diamond", "x", "square", "round", "triangle" and "reversedTriangle".

In addition, each style supports 3 types of variations that consist in enclosing the string into (), <> or [].<br />
For standard notehead, these variations are simply the  "()", "<>" or "[]" strings.





<br />


## \octava

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \octava | \oct | RP | displays an octava sign and transposes the notes accordingly |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| i | integer | a signed number of octaves (up or down) | *none* | false |






<br />


## \restFormat

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \restFormat |  | RP | control the rest format |

The **\restFormat** tag supports both a range and a position form.
For the latter, the format is applied up to the next format specification or to the end of the voice.

**\restFormat** is a way to introduce [common parameters](../../tagsparams#common-parameters) to rest.




<br />


## \headsCenter

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \headsCenter |  | RP | moves the notehead centered on the stem |

The **\headsCenter** tag supports both a range and a position form.
For the latter, it is is applied up to the next *head* specification or to the end of the voice.


## \headsLeft

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \headsLeft |  | RP | force the notehead to the left of the stem |

The **\headsLeft** tag supports both a range and a position form.
For the latter, it is is applied up to the next *head* specification or to the end of the voice.


## \headsRight

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \headsRight |  | RP | force the notehead to the right of the stem |

The **\headsRight** tag supports both a range and a position form.
For the latter, it is is applied up to the next *head* specification or to the end of the voice.


## \headsNormal

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \headsNormal |  | RP | cancel a previous *head* specification and reverts to normal head position. |

The **\headsNormal** tag supports both a range and a position form.
For the latter, it is is applied up to the next *head* specification or to the end of the voice.


## \headsReverse

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \headsReverse |  | RP | moves the notehead on the opposite side of the normal position |

The **\headsReverse** tag supports both a range and a position form.
For the latter, it is is applied up to the next *head* specification or to the end of the voice.




<br />


## \stemsOff

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \stemsOff |  | P | disable stems drawing |



## \stemsAuto

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \stemsAuto |  | P | cancel \stemsOff and enable automatic stems direction |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| length | unit | the stem length | 7.0 | true |




## \stemsDown

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \stemsDown |  | P | force stem direction to down |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| length | unit | the stem length | 7.0 | true |

See the [4 voices](../../../examples/4voices/) example.



## \stemsUp

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \stemsUp |  | P | force stem direction to up |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| length | unit | the stem length | 7.0 | true |

See the [4 voices](../../../examples/4voices/) example.





<br />


## \tie

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \tie | \tieBegin \tieEnd | R | tie between successive notes |

The ties parameters are similar to those of [slurs](../Articulations#slur).
Apart from moving the curve 'up' or 'down', there is no reason to change the other parameters



See the [Tie](../../../examples/space/) example.





<br />


## \tuplet

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \tuplet |  | R | displays an octava sign and transposes the notes accordingly |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| format | string | a format string for shape control | *none* | false |
| position | string | below or above | above | true |
| dy1 | unit | displacement of the left anchor point | 0 | true |
| dy2 | unit | displacement of the right anchor point | 0 | true |
| lineThickness | float | the tuplet line thickness (if any) | 4 | true |
| bold | boolean | to use bold font | false | true |
| textSize | float | the text size | 1 | true |
| dispNote | string | a note duration | *none* | true |

- the **format** string must be in "x", "-x-", "x:y", "-x:y-", or "--"
where 'x' and 'y' are the numbers and '-' denotes the presence of left and right tuplet braces.
- **textSize** is a ratio: 1 is for nominal size
- **dispNote** is similar to [\displayDuration](#displayduration): it forces the notes appearance whatever their duration. The **dispNote** string must be in the form "/n", where n is a number (e.g. "/16" to display sixteenth notes)



