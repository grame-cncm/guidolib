filename = 'guidohttpserver.xcodeproj/project.pbxproj'
infi = file(filename,'r')
lines = infi.read()
infi.close()

lines = lines.replace('isa = PBXFrameworkReference;','lastKnownFileType = wrapper.framework;\n\t\t\tisa = PBXFileReference;')

outfi = file(filename, 'w')
outfi.write(lines)
outfi.close()
