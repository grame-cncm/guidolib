
target := wholehead.gmn wholetail.gmn wholewhole.gmn

all: $(target)

wholehead.gmn : wholex4.gmn whole.gmn
	guidohead wholex4.gmn whole.gmn > wholehead.gmn

wholetail.gmn : wholex4.gmn whole.gmn
	guidotail wholex4.gmn whole.gmn > wholetail.gmn

wholewhole.gmn : wholehead.gmn wholetail.gmn
	guidoseq wholehead.gmn wholetail.gmn > wholewhole.gmn

clean:
	rm -f $(target)
