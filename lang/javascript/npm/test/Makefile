

ts: testAPI-ts.js 
#testAPI-ts-node.js

testAPI-ts.js : testAPI.ts ../guidoengine.ts ../libGUIDOEngine.d.ts
	tsc --noImplicitAny --jsx react --target es6 --lib es2015,dom --out $@ testAPI.ts


# testAPI-ts-node.js : testAPI.ts ../guidoengine.ts ../libGUIDOEngine.d.ts
# 	tsc --noImplicitAny --jsx react --target es6 --lib es2015 --out $@ testAPI.ts
