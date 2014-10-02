GuidoSVGExportWithFontSpec_retCString = Module.cwrap('GuidoSVGExportWithFontSpec_retCString', 'number', ['number', 'number']);
GuidoAbstractExport_retCString = Module.cwrap('GuidoAbstractExport_retCString', 'number', ['number', 'number']);
GuidoBinaryExport_retCString = Module.cwrap('GuidoBinaryExport_retCString', 'number', ['number', 'number']);

GuidoInitWithIndependentSVG = Module.cwrap('GuidoInitWithIndependentSVG', 'number', ['number']);
GuidoOpenParser = Module.cwrap('GuidoOpenParser', 'number', []);
GuidoString2AR = Module.cwrap('GuidoString2AR', 'number', ['number', 'string']);
GuidoCloseParser = Module.cwrap('GuidoCloseParser', 'number', ['number']);
GuidoReleaseCString = Module.cwrap('GuidoReleaseCString', '', ['number']);

GuidoCountVoices = Module.cwrap('GuidoCountVoices','number', ['number']);
GuidoGetPageCount = Module.cwrap('GuidoGetPageCount','number', ['number']);
GuidoGetSystemCount = Module.cwrap('GuidoGetSystemCount','number', ['number', 'number']);

GuidoMakeDate = Module.cwrap('GuidoMakeDate','number', ['number', 'number']);
GuidoGetDateNum = Module.cwrap('GuidoGetDateNum','number', ['number']);
GuidoGetDateDenom = Module.cwrap('GuidoGetDateDenom','number', ['number']);
GuidoFreeDate = Module.cwrap('GuidoFreeDate','', ['number']);

GuidoFreeAR = Module.cwrap('GuidoFreeAR','number', ['number']);
GuidoFreeGR = Module.cwrap('GuidoFreeGR','number', ['number']);

GuidoDuration_retDate = Module.cwrap('GuidoDuration_retDate','number', ['number']);

GuidoFindEventPage_p = Module.cwrap('GuidoFindEventPage_p','number', ['number','number']);
GuidoFindPageAt_p = Module.cwrap('GuidoFindPageAt_p','number', ['number','number']);
GuidoGetPageDate_retDate = Module.cwrap('GuidoGetPageDate_retDate','number', ['number','number']);

GuidoARretGR = Module.cwrap('GuidoARretGR', 'number', ['number', 'number']);
