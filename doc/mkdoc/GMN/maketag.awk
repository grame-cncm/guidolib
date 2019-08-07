
function file_exists(file) {
  if ((getline _ < file) >= 0) return 1;
  return 0;
}

function taginit() {
	NAME  = "";
	ALIAS = "";
	NOTATION = "";
	DESC = "";

	PNAME = "";
	PTYPE = "";
	PDESC = "";
	PDEFT = "";
	POPT = "";
	PNOTE = "";
}
function init() {
	FILE = "";
	GROUP = "";
	taginit();
}

function start(group) {
	return "{!tags.css!}\n\n# "group"\n\n";
}

function tagname (name, variant, type, elt, desc ) {
return "## "name"\n\n-------\n\n\
### Description\n\n\
| Name | Variants | Type | Notation element |\n\
| :----| :--------| :----| :----------------|\n\
| " name " | " variant " | " type " | " elt " |\n\n" desc "\n";
}

function startparam () {
return "\n### Parameters\n\n\
| Name        	| Type   | Description    | Default value  | Optional |\n\
| :------------ |:-------| :--------------| :------------- | :--------| "
}

function fontparams () {
return "Supports [font parameters](../../tagsparams#text-parameters)"
}


function param (name, type, desc, def, opt ) {
	return "| " name " | " type " | " desc " | " def " | " opt " |" ;
}

function add_desc (content) {
	if (INDOC) {
		gsub(/@EXAMPLES/, "../../../examples", content)

		if (INDESC) {
			DESC = DESC content "\n";
		}
		if (INPDESC) {
			PNOTE = PNOTE content "\n";
		}
	}
}


BEGIN {
	FS = ":";

# 	OUT = "tags";
	init();

	INDOC = 0;
	INDESC = 0;
}

END {
}

################# generates tags doc for mkdoc
/^[^@]/  	{ add_desc($0); }
/^$/ 		{ add_desc($0); }


/^\/\*[ 	]*@mkdoc/ {
	INDOC = 1;
	init()
}

/^[ 	]*\*\// 	{  INDOC = 0; }

/^@group/ {
	if (INDOC) {
		GROUP = $2;
		FILE = OUT "/" GROUP ".md"
		gsub(/ /, "", FILE)
		if ( file_exists(FILE)) {
			print "\n\n<br />\n\n" >> FILE;
		}
		else {
			print "New group " GROUP; 
			print start(GROUP) > FILE
		}
	}
}

/^@tagname/ 		{ taginit(); NAME = $2; }
/^@tagalias/		{ ALIAS= $2; }
/^@tagtype/			{ TYPE = $2; }
/^@tagnotation/		{ NOTATION = $2; }
/^@tagdesc/			{ INDESC = 1; }
/^@tagend/			{ 
	INDESC = 0; 
	if (INDOC) {
		print "Create " NAME " in group " GROUP; 
	 	print tagname(NAME, ALIAS, TYPE, NOTATION, DESC) >> FILE;
	 }
}

/^@params:/			{ if (INDOC) print startparam()  >> FILE; }
/^@param:/			{ if (INDOC) print param($2, $3, $4, $5, $6)  >> FILE; }
/^@fontparams:/		{ if (INDOC) print fontparams()  >> FILE; }
/^@paramdesc/		{ INPDESC = 1; }
/^@paramend/		{ 
	INPDESC = 0; 
	if (INDOC) print "\n" PNOTE "\n\n"  >> FILE;
}
