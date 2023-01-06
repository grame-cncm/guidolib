Testing the GUIDOEngine Wasm library
======================================================================

This folder provides tools to test the engine API. There are two ways to test: in a web page and using node. Both are equivalent and are performing similar tests, only the context is different.

## Testing in a web page

- run a local web server from the npm folder
- in browser load the testAPI.html or the testAPI-TS.html file

Note :
- testAPI.html is using the `testApi.js` file.
- testAPI-TS.html is using the `testAPI-ts.js` file, which has to be first generated from the testAPI.ts file. The enclosed Makefile is provided for that. 


## Testing using node

Just run:
~~~~~~~~~~~
node testAPI.node.js
~~~~~~~~~~~


