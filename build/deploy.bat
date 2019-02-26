
echo "Deploying applications"

set cmd="dir /b/s GuidoEditor.exe"
FOR /F "tokens=*" %%i IN (' %cmd% ') DO  windeployqt %%i --compiler-runtime

set cmd="dir /b/s GuidoSceneComposer.exe"
FOR /F "tokens=*" %%i IN (' %cmd% ') DO  windeployqt %%i --compiler-runtime

