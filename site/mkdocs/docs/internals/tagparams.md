# Tag Parameters

All the tags parameters are declared in the [`src/engine/abstract/TagParameterStrings.cpp`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/TagParameterStrings.cpp) file. It consists in a set of constant strings, consistently named according to the AR class that uses the parameters. For example: tag parameters for the `ARMeter` class is named `kARMeterParams`.

----

## Tag parameters string
A tag parameters string is a list of semicolon separated parameters.
Each parameter is a string in the following format:
~~~~~~~~~~~
"TYPE,NAME,DEFVALUE,REQUIRED"
~~~~~~~~~~~
where:

- TYPE is the parameter type and must be in:
    - 'U' : a unit parameter i.e. a numeric value with an optional unit ('cm', 'hs',...). When omitted, the default unit is used.
    - 'S' : a string parameter.
    - 'I' : an integer parameter.
    - 'F' : a float parameter.
- NAME is the parameter name.
- DEFVALUE is the parameter default value.
- REQUIRED is either 'r' or 'o' for required or optional parameters.

### Example:
The ARMeter tag parameters are described with:
~~~~~~~~~~~
"S,type,4/4,r;S,autoBarlines,on,o;S,autoMeasuresNum,off,o;S,group,off,o"
~~~~~~~~~~~
see the [\meter documentation](https://guidodoc.grame.fr/refs/tags/ClefKeyMeter/#meter) for the parameters description.

----

## Shared Parameters

The following parameters are shared by all the AR classes: 
~~~~~~~~~~~
const char* kCommonParams		= "S,color,black,o;U,dx,0,o;U,dy,0,o;F,size,1.0,o";
~~~~~~~~~~~

The following parameters are shared by all the ARFontAble derived classes: 
~~~~~~~~~~~
const char* kARFontAbleParams	= "S,textformat,rc,o;S,font,Times,o;U,fsize,9pt,o;S,fattrib,,o";
~~~~~~~~~~~

----

## Initialisation

All the tag parameters strings are stored in a [`TagParametersMaps`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/TagParametersMaps.h) structure that is initialised by `GuidoInit()`and released by `GuidoShutdown()`. 

Each AR class is responsible to:

- setup its parameters at construction time using `setupTagParameters (_a_param_map_)`
- handle the current parameters by overriding the `setTagParameters`method

For example, see the [`ARFermata`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/ARFermata.cpp) implementation.

----

## Creating a new parameters string
It requires the following steps:

- implement the new parameter string in [`TagParameterStrings.cpp`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/TagParameterStrings.cpp)
- declare the new string in [`TagParameterStrings.h`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/TagParameterStrings.h)
- add a new parameters map in [`TagParametersMaps.h`](https://github.com/grame-cncm/guidolib/blob/dev/src/engine/abstract/TagParametersMaps.h)
