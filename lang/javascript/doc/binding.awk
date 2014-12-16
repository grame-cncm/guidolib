BEGIN { 
print "Detail of mapping C/C++ to javascript."
print "You can refer to C/C++ documentation for each mapped object.";
print "All mapped classes without method are just mapped to be able to pass argument pointer of this class.";
print "";
print "You can create a class object with :"
print "	var myObject = new Module.Myclass;"
print "You have to delete it with "
print "	delete myObject;"
print ""
print "C structure are json object and can be access and construct like it."
print "Example to create GuidoLayoutSettings :"
print ""
print "	var settings= {"
print "		systemsDistance : 75,"
print "		systemsDistribution : 1,"
print "		systemsDistribLimit : 0.25,"
print "		force : 600,"
print "		spring : 1.7,"
print "		neighborhoodSpacing : 0,"
print "		optimalPageFill : 1,"
print "		resizePage2Music : 1,"
print "		proportionalRenderingForceMultiplicator : 0"
print "	};"
print ""
print "Enum can be access with Module.enumName.enumValue;"
print "Constants can be access with Module.constantName;"
print "";
print "You can find more details in embind module of emscripten documentation";
print "";
}

/emscripten::value_object<.*>/ { 
	match($0, /<(.*)>\("(.*)"\)/, a); 
	print "The C/C++ structure " a[1] " is map to javascript with the name : " a[2];
	previous = "valueobject";
 }
/emscripten::enum_<.*>/ { 
	match($0, /<(.*)>\("(.*)"\)/, a); 
	if (previous != "enum") print "";
	print "The C/C++ enum " a[1] " is map to javascript with the name : " a[2];
	previous = "enum";	
 }
/emscripten::constant/ { 
	match($0, /.*\("(.*)",\s*(.*)\)/, a); 
	if (previous != "constant") print "";
	print "The C/C++ constant " a[2] " is map to javascript with the name : " a[1];
	previous = "constant";
}
/emscripten::class/ { 
	match($0, /<(.*)>\("(.*)"\)/, a); 
	print "";
	print "The C/C++ class " a[1] " is map to javascript with the name : " a[2];
 }
/\.function/ {
	match($0, /"(.*)".*::(.*)/, a);
	print "	The method a[2] is map to javascript with the name : " a[1];
}
END { print ""; }
