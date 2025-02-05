
target := slurhead.gmn slurtail.gmn slurwhole.gmn slur1-2.gmn

all: $(target)

slurhead.gmn : slur.gmn whole.gmn
	guidohead slur.gmn whole.gmn > slurhead.gmn

slurtail.gmn : slur.gmn whole.gmn
	guidotail slur.gmn whole.gmn > slurtail.gmn

slurwhole.gmn : slurhead.gmn slurtail.gmn
	guidoseq slurhead.gmn slurtail.gmn > slurwhole.gmn

slur1-2.gmn : slur1.gmn slur2.gmn
	guidoseq slur1.gmn slur2.gmn > slur1-2.gmn

clean:
	rm -f $(target)
