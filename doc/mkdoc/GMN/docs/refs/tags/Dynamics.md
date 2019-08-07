{!tags.css!}

# Dynamics


## \crescendo

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \crescendo | \cresc<br />\crescBegin \crescEnd | R | crescendo sign |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| dx1 | unit | start position displacement | 0 | true |
| dx2 | unit | end position displacement | 0 | true |
| deltaY | unit | control the open end of the hairpins | 3 | true |
| thickness | unit | the line thickness | 0.16 | true |
| autopos | string | when 'on', try to automatically avoid collisions | off | true |

See the [Dynamics](../../../examples/dynamics/) example.





<br />


## \decrescendo

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \decrescendo | \decresc<br />\dimnuendo <br />\dim<br />\crescBegin \crescEnd<br />\diminuendoBegin \diminuendoEnd <br />\dimBegin \dimEnd | R | crescendo sign |




Parameters for decrescendo are the same as for crescendo

See the [Dynamics](../../../examples/dynamics/) example.





<br />


## \intensity

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \intensity | \intens \i | P | dynamic marks |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| type | string | a dynamic string | *none* | false |
| before | string | a string displayed before the dynamic mark | *none* | true |
| after | string | a string displayed after the dynamic mark | *none* | true |
| autopos | boolean | when 'on', try to automatically avoid collisions | off | true |

- supported dynamic strings are "p", "pp", "ppp", "pppp", "f", "ff", "fff", "ffff", "mf", "mp", "sf", "sfz", "rfz" and "fz"

See the [Dynamics](../../../examples/dynamics/) example.



