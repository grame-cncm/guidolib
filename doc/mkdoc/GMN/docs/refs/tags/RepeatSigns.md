{!tags.css!}

# Repeat Signs


## \coda

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \coda |  | P | the code sign |





<br />


## \daCapo

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \daCapo |  | P | Da Capo |



## \daCapoAlFine

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \daCapoAlFine |  | P | Da Capo al Fine |





<br />


## \daCoda

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \daCoda |  | P | Da Coda |





<br />


## \dalSegno

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \dalSegno |  | P | Dal Segno |



## \dalSegnoAlFine

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \dalSegnoAlFine |  | P | Dal Segno al Fine |





<br />


## \fine

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \fine |  | P | Fine |





<br />


## \repeatBegin

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \repeatBegin |  | P | A begin repeat bar |





<br />


## \repeatEnd

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \repeatEnd |  | P | An end repeat bar |





<br />


## \segno

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \segno |  | P | Segno |





<br />


## \tremolo

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \tremolo | \trem <br />tremBegin tremEnd <br />tremoloBegin tremoloEnd | R | displays a tremolo |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| style | string | a strokes string | /// | true |
| speed | integer | the tremolo speed | 32 | true |
| pitch | string | the pitch of the other note | *none* | true |
| thickness | unit | the stroke thickness | 0.75 | true |
| text | string | a string displayed over the strokes | *none* | true |






<br />


## \volta

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \volta | \voltaBegin \voltaEnd | R | support for multiple endings in repreated sections |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| mark | string | the section mark, typically the ending number | *none* | false |
| format | string | the volta format | *none* | true |

- the **format** is a string in the form "|-|", "|-", "-|" or "-", that describes the shape of the line over the repeated section.



