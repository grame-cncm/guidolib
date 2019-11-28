function checkclass() {
	if (inclass == 1)
		print "</div>";	
	inclass = 0;
	waitfunc = 0;
}

function checkwait() {
	if (waitfunc == 1) {
		print "<div id='functions'>Functions: </div>";
		print "<div id='list'>";
	}
	waitfunc = 0;
}

function getClass(name) {
	if (name == "GUIDOScoreMap")
		return doc"/classGuidoScoreMapAdapter.html";
	return doc"/class"name".html";
}

function opaque(name) {
	if (name == "GuidoParser") return 1;
	if (name == "NodeAR") return 1;
	if (name == "NodeGR") return 1;
	if (name == "GuidoStream") return 1;
	if (name == "PianoRoll") return 1;
	return 0;
}

###################################
BEGIN { 
	FS = "\"";
	inclass=0;
	doc="api";
	system("cat rsrc/header.html");
}

END { 
	checkclass();
	system("cat rsrc/footer.html");
}


###################################
/^	class_</ { 
	checkclass();
	print "<br />";
	if (opaque($2))
		print "<div id='module'>" $2 "<span class='opaque'> Opaque pointer</span></div>";
	else
		print "<div id='module'>Module: <a href='"getClass($2)"'>" $2 "</a></div>";
	inclass=1;
	waitfunc=1;
}

/^			.function/ {
	checkwait();
	print "<span class='fname'>" $2 "</span><br />";
}
