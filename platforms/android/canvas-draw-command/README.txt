This project is a library for android application to draw score in android canvas.
You can find an example of utilisation in simpleGuidoEditor application.

To compile this library :

1- Compile jni package. Go to guidolib-code/lang/java and follow instruction. 
	Copy the generated jar (guidoengine.jar and drawcommand.jar) in the ./libs directory of this project.

2- execute this commands : 
  android update project -p .
  ant debug

this jar file genrated is in bin folder and is named canvas-draw-command.jar
