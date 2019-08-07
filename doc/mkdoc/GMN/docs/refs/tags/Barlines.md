{!tags.css!}

# Barlines


## \bar

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \bar | \| | P | a simple bar line |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| displayMeasNum | boolean | displays measure number | false | true |
| numDx | unit | measure number displacement | 0 | true |
| numDy | unit | measure number displacement | 0 | true |

Measure numbering is attached to barlines. The **displayMeasNum** parameter activates or inhibits measures numbering.

Measure numbering may also be activated at [\meter](../ClefKeyMeter) level.

See the [Barlines](../../../examples/barlines/) example.





<br />


## \barFormat

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \barFormat |  | P | describes the span of a barline. |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| style | string | system or staff | staff | true |
| range | string | only for system style - indicates the range of staves covered by the bar | *none* | true |

- the **range** string must be in the form *"n-m"* where 'n' and 'm' are integer numbers denoting staff indexes (starting from 1)

See the [Barlines](../../../examples/barlines/) example.





<br />


## \doubleBar

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \doubleBar |  | P | a double bar line |




The **\doubleBar** tag supports the same parameters than the **\bar** tag.

See the [Barlines](../../../examples/barlines/) example.





<br />


## \endBar

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \endBar |  | P | an end bar line |

A finish bar is automatically inserted at the end of a piece, unless otherwise specified.
See also the [auto](../Miscellaneous) tag.






