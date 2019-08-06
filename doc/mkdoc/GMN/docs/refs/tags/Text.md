{!tags.css!}

# Text


## \fingering

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \fingering | \fing | P | fingering |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| position | string | above or below | *none* | true |
Supports [font parameters](/refs/tagsparams/#text-parameters)

The default position for fingering is close to the note head. When **position** is specified, the fingering is placed above or below the note, avoiding collisions with the stems and always outside the staff.





<br />


## \instrument

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \instrument | \instr | P | instrument marking |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| name | string | the instrument name | *none* | false |
| transp | string | a transposition string | *none* | true |
| autopos | boolean | automatic position control | off | true |
Supports [font parameters](/refs/tagsparams/#text-parameters)






<br />


## \lyrics

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \lyrics |  | R | lyrics |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| text | string | the lyrics | *none* | false |
| autopos | string |  | off | true |
Supports [font parameters](/refs/tagsparams/#text-parameters)






<br />


## \mark

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \mark |  | P | marks |

The **\text** tag is both a range and a position tag. When a range is specified, the text doesn't take place: it is placed over the range.
With no range, the text is inserted at its position.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| text | string | the mark | *none* | false |
| enclosure | string | none, square, rectangle, oval, circle, bracket, triangle, diamond | none | true |
Supports [font parameters](/refs/tagsparams/#text-parameters)






<br />


## \text

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \text | \label <br/> \t | RP | textual elements |

The **\text** tag is both a range and a position tag. When a range is specified, the text doesn't take place: it is placed over the range.
With no range, the text is inserted at its position.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| text | string | the text | *none* | false |
Supports [font parameters](/refs/tagsparams/#text-parameters)






<br />


## \harmony

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \harmony |  | P | harmony marking |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| text | string | the harmony string (e.g. C7) | *none* | false |
Supports [font parameters](/refs/tagsparams/#text-parameters)




