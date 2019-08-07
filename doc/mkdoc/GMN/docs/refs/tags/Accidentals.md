{!tags.css!}

# Accidentals


## \accidental

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \accidental | \acc | R | sharp, flat, natural, cautionary accidental |

The **\accidental** tag may be used for more precise control over accidentals. It supports common tag parameters.
Used without parameter, it forces the display of an accidental (flat, sharp or natural).



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| style | string | "cautionary" to enforce a cautionary accidental<br/>"none" to prevent an accidental display | *none* | true |

See the [Accidentals](../../../examples/accidentals/) example.





<br />


## \alter

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \alter |  | RP | micro-tonal accidentals |

The **\alter** tag may be used for micro-tonal accidentals. It supports common tag parameters.
It can be used with or without a range: in the latter, it takes effect until the next **\alter** tag; if a ranged alter is encountered in between, it is applied locally and the non-ranged alter continues to apply after that.
 
**Note**: the **\alter** tag has a cumulative effect: it is added to existing accidentals.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| detune | float | a signed floating point value representing semi-tones | 0 | false |

See the [Accidentals](../../../examples/accidentals/) example.



