GuidoInitWithIndependentSVG = Module.cwrap('GuidoInitWithIndependentSVG', 'number', ['number']);
GuidoOpenParser = Module.cwrap('GuidoOpenParser', 'number', []);
GuidoCloseParser = Module.cwrap('GuidoCloseParser', 'number', ['number']);
GuidoString2AR = Module.cwrap('GuidoString2AR', 'number', ['number', 'string']);
GuidoARretGR = Module.cwrap('GuidoARretGR', 'number', ['number', 'number']);
GuidoSVGExportWithFontSpec_retCString = Module.cwrap('GuidoSVGExportWithFontSpec_retCString', 'number', ['number', 'number']);
GuidoReleaseCString = Module.cwrap('GuidoReleaseCString', '', ['number']);
