
target := layout1.gmn layout2.gmn

all: $(target)

layout1.gmn : base.gmn format1.gmn
	guidoseq format1.gmn base.gmn > layout1.gmn

layout2.gmn : base.gmn format2.gmn
	guidoseq format2.gmn base.gmn > layout2.gmn

clean:
	rm -f $(target)
