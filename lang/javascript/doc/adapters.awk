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


/^	emscripten::class_/ { 
	checkclass();
	print "<br />";
	print "<div id='module'>Module: <a href='"doc"/class"$2".html'>" $2 "</a></div>";
	inclass=1;
	waitfunc=1;
}

/^			.function/ {
	checkwait();
	print "<span class='fname'>" $2 "</span><br />";
}
