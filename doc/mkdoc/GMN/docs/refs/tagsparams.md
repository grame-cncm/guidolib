{!references.ref!}
{!tags.css!}

# Tag Parameters 

This section describes the general syntax for tag parameters and parameters common to a set of tags as well.
The parameters specific to a given tag are described with the corresponding tag.

## Syntax

The general syntax for tag parameters is the following:
{!BNF/tagsparam.html!}

where `value` is among:
{!BNF/value.html!}



## Values

Values may be of different types among the following:

- **string**: a double quoted string
- **float**: a floating point or integer number
- **unit**: a number followed by a unit string
- **boolean**: a special string among:
    - "true", "on", "yes" 
    - "false", "off", "no" 

Note that boolean values are not case sensitive. Also note that a string that is not in the *true* set is considered as false.



## Units

Units may be used with numbers, generaly for for displacements (dx, dy).   
The supported units are:

| Unit        | Description    |
| ------------- |:-------------| 
| m       | meter |
| cm      | centimeter |
| mm      | millimeter |
| in      | inch  (2.54cm)|
| pt      | point ( ~0,353 mm) |
| pc      | pica  ( ~4.2333 mm) |
| hs      | half space of the current staff |

When not specified, the default unit is `hs`.

### Example
~~~~~~
{
	[ \meter<"4/4"> \i<"p", dx=0.01m> d/2 e/4 f ],
	[ \meter<"4/4"> \i<"p", dx=1cm>   d/2 e/4 f ],
	[ \meter<"4/4"> \i<"p", dx=10mm>  d/2 e/4 f ],
	[ \meter<"4/4"> \i<"p", dx=0.3937in> d/2 e/4 f ],
	[ \meter<"4/4"> \i<"p", dx=28.32pt>  d/2 e/4 f ],
	[ \meter<"4/4"> \i<"p", dx=2.362pc>  d/2 e/4 f ],
	[ \meter<"4/4"> \i<"p", dx=1cm, dy=2hs> d/2 e/4 f ]
}
~~~~~~
{!GMN/units.html!}


## Colors

Color is part of the common tag parameters. A color may be specified using:

- an [html color name](https://www.w3schools.com/colors/colors_names.asp)
- an RGB value in the form 0xrrggbb
- an RGBA value in the form 0xrrggbbaa
  
### Examples
~~~~~~
[ \clef<"g", color="blue"> 	\key<4, color="0xaaaaaa"> 	\meter<"4/4", color="0xdd000050"> ]
~~~~~~
{!GMN/colors.html!}



## Common parameters
Common parameters are tag parameters supported by all tags.

| Name        	| Type   | Description    | Default value  | Optional |
| ------------- |:-------| :--------------| :------------- | :--------| 
| dx       		| unit   | displacement on the horizontal axis | 0 | true |
| dy      		| unit   | displacement on the vertical axis | 0 | true |
| color      	| string | a color indication (see [above](#colors))| black | true |
| size      	| float  | a size specification | 1.0 | true |



## Text parameters

Text parameters are tag parameters supported by text based tags such as `\text`, `\lirycs`.

| Name        	| Type   | Description    | Default value  | Optional |
| ------------- |:-------| :--------------| :------------- | :--------|  
| textformat    | string | a format string (see below)  | rc | true |
| font      	| string | a font name | Times | true |
| fsize      	| unit   | the font size | 9pt | true |
| fattrib      	| string | the font attributes (see below) | *none* | true |

### Format string
A format string is in the form 'hv' where :

- 'h' must be in 
	- 'l' : for horizontal left alignment
	- 'c' : for horizontal centering
	- 'r' : for horizontal right alignment
- 'v' must be in 
	- 't' : for vertical top alignment
	- 'c' : for vertical centering
	- 'b' : for vertical bottom alignment

### Font attributes
A combination of the following characters:

- 'b' for bold 
- 'i' for italic 
- 'u' for underline 
  
### Examples
~~~~~~
[ \clef<"f"> \text<"hello world", fsize=6pt, fattrib="bi", dy=3.9> ]
~~~~~~
{!GMN/font.html!}



