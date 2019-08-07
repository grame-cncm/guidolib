{!tags.css!}

# Beaming


## \beam

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \beam | \bm<br/>\b<br/>\beamBegin beamEnd | R | notes beaming |

Beaming is automatically computed according to the current meter. Manual beaming is always possible.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| dy1 | unit | adjust the left beaming position | *none* | true |
| dy2 | unit | adjust the right beaming position | *none* | true |
| dy | unit | similar to dy1=dy and dy2=dy | *none* | true |

*dy* parameters adjust the position of the beams relative to the note head.

Known issue: *dy* fails when the stems are down.

See the [Beaming](../../../examples/beaming/) example.






<br />


## \beamsAuto

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \beamsAuto |  | P | set automatic beaming on |



## \beamsOff

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \beamsOff |  | P | set automatic beaming off |



## \beamsFull

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \beamsFull |  | P | set full automatic beaming on |

A rest inside a group of notes prevents automatic beaming unless full beaming is on.





<br />


## \fBeam

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \fBeam | \fbeamBegin fbeamEnd | R | feathered beaming |

Feathered beaming is provided to support contemporary notation.

Notes grouped under a feathered beam should express their duration normally.
The spacing algorithm will take these durations into account to position them like any other note.
The number of beams is computed according to the first and last notes durations.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| durations | string | used to control the number of beams | *none* | true |
| drawDuration | boolean | displays the effective duration of the beamed group | false | true |

- **duration** may be used to override the default strategy to compute the number of beams.
It must be a string in the form "n/d,m/e" that express the number of beams for the begin and end of the group,
under the form of two rationals denoting durations. For example, "1/8,1/32" indicates that the group starts with
- **drawDuration** is used to display the actual total duration of the beamed group under the form of a rational value.

See the [Beaming](../../../examples/beaming/) example.



