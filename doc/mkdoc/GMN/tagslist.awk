function add (entry,group) {
	first = substr(entry, 1, 1);
	sub(/ /, "", group); 
	sub(/ /, "", group); 
	a = "[\\" entry "](tags/" group ")";
	if (first == INDEX) {
		LIST = LIST " " a;
	} else {
		print "##" toupper(INDEX) "\n" LIST "\n";
		LIST = a;
		INDEX = first;
	}
}

BEGIN {
	INDEX = "a";
	LIST = "";
	FS = ":";
	print "# Tags index \n";
}

END {
	print "##" toupper(INDEX) "\n" LIST "\n";
}

################# papers generation
/^[a-zA-Z]/ { 
	add($1,$2);
}
