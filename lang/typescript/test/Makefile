
TS = $(wildcard *.ts)

run : test.js
	node test.js

test.js : $(TS) Makefile
	tsc --jsx react --target es6 --lib es2015,dom --out test.js $(TS)

clean:
	rm -f test.js