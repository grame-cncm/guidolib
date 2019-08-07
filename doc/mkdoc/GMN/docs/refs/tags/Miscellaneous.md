{!tags.css!}

# Miscellaneous


## \auto

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \auto | \set | P | sets voice level global settings |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| autoEndBar | string | automatic end bar | on | true |
| endBar | string | variant for autoEndBar | on | true |
| autoPageBreak | string | automatic page break | on | true |
| pageBreak | string | variant for autoPageBreak | on | true |
| autoSystemBreak | string | automatic system break | on | true |
| systemBreak | string | variant for autoSystemBreak | on | true |
| autoClefKeyMeterOrder | string | automatic reordering of clef, key and meter | on | true |
| clefKeyMeterOrder | string | variant for autoClefKeyMeterOrder | on | true |
| autoLyricsPos | string | automatic collision avoidance for \lyrics | off | true |
| lyricsAutoPos | string | ariant for autoLyricsPos | off | true |
| autoInstrPos | string | automatic positionning for \instr | off | true |
| instrAutoPos | string | variant for autoInstrPos | off | true |
| autoIntensPos | string | automatic collision avoidance for \intens | off | true |
| intensAutoPos | string | variant for autoIntensPos | off | true |

Collision management strategies can produce unexpected results or even create new collisions.
In this case, you should switch to manual layout using the 'dx' and 'dy' [common parameters](../../tagsparams#common-parameters).





<br />


## \space

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \space |  | P | insert space at any position. |

The **\space** tag can be used to save space for notation elements that do not take up space.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| dd | unit | the amount of space | 0 | false |

See the [Space](../../../examples/space/) example.





<br />


## \special

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \special |  | P | insert a musical glyph |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| char | string | the character to display | *none* | false |

**char** may be specified as:

- a litteral character
- an hexadecimal number i.e. a number prefixed with "\x" (e.g. "\xa0")
- an octal number i.e. a number prefixed with "\o" (e.g. "\o130")
- a decimal number i.e. a number prefixed with "\"

See the [Special](../../../examples/space/) example.



