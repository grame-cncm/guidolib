
TS = $(wildcard *.ts)

run : test.js
	node test.js

test.js : $(TS)
	tsc --out test.js $(TS)
