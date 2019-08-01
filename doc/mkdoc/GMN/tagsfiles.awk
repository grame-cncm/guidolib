
BEGIN {
	FILE = "";
	FS = " ";
}

END {
}

################# papers generation
/^[a-zA-Z]* *=/ { 
	if ($1 != "")
		FILE="params/" $1 ".txt"
}

/^| / { 
	if (FILE != "")
		print $0 > FILE;
}
