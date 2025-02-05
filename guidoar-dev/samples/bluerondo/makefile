
target := bluerondo
out	   := $(target).gmn $(target).mid $(target).pdf

all: $(out)

$(target).gmn :
	sh bluerondo > $(target).gmn


$(target).mid : $(target).gmn
	guido2midi $(target).gmn $(target).mid

$(target).pdf : $(target).gmn
	guido2image -f $(target).gmn -t pdf

clean:
	rm -f $(out)
