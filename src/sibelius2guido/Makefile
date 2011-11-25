#
# simple packaging utility
#

VERSION	:= 0.41
TARGET	:= sibelius2guido
DEST	:= sibelius2guido-$(VERSION)

package:
	[ -d $(DEST) ] || mkdir $(DEST)
	cp readme.txt $(TARGET).plg $(DEST)
	zip -ru -x.DS_Store $(DEST).zip $(DEST)
