
target := repeathead.gmn repeattail.gmn repeatwhole.gmn

all: $(target)

repeathead.gmn : repeats.gmn wholex2.gmn
	guidohead repeats.gmn wholex2.gmn > repeathead.gmn

repeattail.gmn : repeats.gmn wholex2.gmn
	guidotail repeats.gmn wholex2.gmn > repeattail.gmn

repeatwhole.gmn : repeathead.gmn repeattail.gmn
	guidoseq repeathead.gmn repeattail.gmn > repeatwhole.gmn

clean:
	rm -f $(target)
