


## Score 
-----------

~~~~~~
score		: '{' '}'
			| '{' voicelist '}'
			| voice	
			;

voicelist	: voice
			| voicelist ',' voice	
			;

voice		: '[' symbols ']' ;

symbols		:
			| symbols music	
			| symbols tag
			| symbols chord	
			;
~~~~~~

## Tags 
-----------

~~~~~~
tag			: positiontag
			| rangetag
			;

positiontag	: tagid
			| tagid '<' tagparams '>'
			;

rangetag	: positiontag '(' symbols ')' ;

tagid		: tagname
			| tagname : integer
			;

tagname		: \ string


tagparams	: tagparam
			| tagparams ',' tagparam
			;

tagparam	: tagarg
			| id '=' tagarg
			;

tagarg		: number
			| number UNIT
			| string
			;

number		: integer | float ;
UNIT		: 'm' | 'cm' | 'mm' | 'in' | 'pt' | 'pc' | 'hs' ;
string      : [a-z_A-Z][a-z_A-Z0-9]*
~~~~~~

## Chord 
-----------

~~~~~~
chord		: '{' chordsymbols '}' ;

chordsymbols: taggedchordsymbol
			| chordsymbols , taggedchordsymbol
			;

taggedchordsymbol: chordsymbol
			| taglist chordsymbol
			| chordsymbol taglist
			| taglist chordsymbol taglist
			;

chordsymbol	: music	
			| rangechordtag	
			;

rangechordtag : positiontag  '(' taggedchordsymbol ')'
			;

taglist		: positiontag
			| taglist positiontag
			;
~~~~~~

## Notes 
-----------

~~~~~~
music		: note | rest ;

rest		: '_' duration dots
			| '_' '<' integer '>' duration dots
			;

note		: noteid octave duration dots
			| noteid accidentals octave duration dots
			;

noteid		: notename
			| notename '<' integer '>'
			;
			
notename	: DIATONIC
			| CHROMATIC
			| SOLFEGE
			| "empty"
			;

DIATONIC    : 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' ;
CHROMATIC   : 'cis' | 'dis' | 'fis' | 'gis' | 'ais' ;
SOLFEGE     : 'do' | 're' | 'mi' | 'fa' | 'sol' | 'la' | 'si' | 'ti'

accidentals	: accidental | accidentals accidental ;
accidental	: '#' | '&' ;
octave		: integer ;

duration	:
			| '*' number '/' number
			| '*' number
			| '*' number 'ms'
			| '/' number
			;

dots		: '.' | '..' | '...' ;
~~~~~~
