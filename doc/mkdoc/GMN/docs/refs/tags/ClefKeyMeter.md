{!tags.css!}

# Clef Key Meter


## \clef

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \clef |  | P | clef signs |




### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| type | string | bass, basso, f, f5, f4, f3, f2, f1, <br />tenor, c, c5, c4, c3, c2, c1, c0, <br />alto, violin, treble, <br />g, g5, g4, g3, g2, g1, <br />gg, gg5, gg4, gg3, gg2, gg1, <br />perc, perc5, perc4, perc3, perc2, perc1, <br />none or off | treble | false |

- bass, basso, f, f4 are equivalent and represent a standard F clef on the 4th staff line
- f1, f2, f3, f5 represent a f clef on the 1st, 2nd and 5th staff line
- alto, c, c3 are equivalent and represent a bratsche clef on the 3rd staff line
- c4 and tenor are equivalent and represent a bratsche clef on the 4th staff line
- c1, c2, c3, c5 represent a bratsche clef on the 1st, 2nd, 3rd and 5th staff line
- c0 represent a bratsche clef on the 1st ledger line under the staff
- g, g2, treble, violin are equivalent and represent a G clef on the 2nd staff line
- g1, g3, g4, g5 represent a G clef on the 1st, 3rd, 4th and 5th staff line
- gg, gg2 are equivalent and represent a double G clef on the 2nd staff line
- gg1, gg3, gg4, gg5 represent a double G clef on the 1st, 3rd, 4th and 5th staff line
- perc, perc3 are equivalent and represent a percussion clef on the 3rd staff line
- perc1, perc2, perc4, perc5 represent a percussion clef on the 1st, 2nd, 4th and 5th staff line
- none, off prevents the clef to be displayed

See the [Clefs](../../../examples/clefkeymeter/) example.





<br />


## \key

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \key |  | P | key signatures |

The **\key** tag displays a key signature specified as tonality or as a number of sharps or flats. It supports also arbitrary keys and micro-tonality.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| key | string\|integer | F, C, G, D, A, E, B, H or a signed integer<br />a free key string | *none* | false |
| hideNaturals | string | a boolean value to prevent naturals on key change | false | true |
| free | string | a free key string | *none* | true |

- when specified with a signed integer, positive values are for sharps and negative values for flat
- F is equivalent to -1
- C, G, D, A, E are respectively equivalent to 0, 1, 2, 3 and 4
- B and H are equivalent to 5
- a **free key** string is an arbitrary list of notes with accidentals separated by spaces. When not indicated, an accidental takes the previous value, Accidentals support micro-tonality using signed floating point numbers inside brackets. Example: "g#d&" denotes a key signature with a 'g' sharp and a 'd' flat; "f[0.5]c" denotes a key with a quarter tone sharp on 'f' and 'c'.

See the [Key](../../../examples/clefkeymeter/) example.





<br />


## \meter

-------

### Description

| Name | Variants | Type | Notation element |
| :----| :--------| :----| :----------------|
| \meter |  | P | time signatures |

The **\meter** tag displays a time signature.



### Parameters

| Name        	| Type   | Description    | Default value  | Optional |
| :------------ |:-------| :--------------| :------------- | :--------| 
| type | string | C, c, C/, c/<br />a rational string<br />a complex time signature string | 4/4 | false |
| autoBarlines | boolean | control automatic barlines | on | true |
| autoMeasuresNum | string | on, off, system, page | off | true |
| group | boolean | control complex meter appearance | off | true |

- C, c, C/, c/ : a C or a crossed C meter.
- complex time strings are in on of the following forms:
	- a) "a+b+...+n/d" a rational with a sum as numerator
	- b) "a/b + c/d" a sum of rationals
	- c) a combination of a) and b)
- **autoMeasuresNum** activates automatic measures numbering and supports 3 modes:
	- on: the measure number is displayed at every bar
	- system: the measure number is displayed on every system only
	- page: the measure number is displayed on every page only
- **group** applies to complex time signatures and control how the a) form encountered in a c) form is displayed: when **group** if off, the a) form is transformed in a b) form

See the [Meter](../../../examples/clefkeymeter/) example.



