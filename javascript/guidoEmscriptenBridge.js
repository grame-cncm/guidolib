GuidoGetSVGExportWithFontSpec = Module.cwrap('GuidoGetSVGExportWithFontSpec', 'number', ['number', 'number']);
GuidoGetAbstractExport = Module.cwrap('GuidoGetAbstractExport', 'number', ['number', 'number']);
GuidoGetBinaryExport = Module.cwrap('GuidoGetBinaryExport', 'number', ['number', 'number', 'number']);

GuidoInitWithIndependentSVG = Module.cwrap('GuidoInitWithIndependentSVG', 'number', ['number']);
GuidoOpenParser = Module.cwrap('GuidoOpenParser', 'number', []);
GuidoString2AR = Module.cwrap('GuidoString2AR', 'number', ['number', 'string']);
GuidoCloseParser = Module.cwrap('GuidoCloseParser', null, ['number']);
GuidoReleaseCString = Module.cwrap('GuidoReleaseCString', null, ['number']);

GuidoCountVoices = Module.cwrap('GuidoCountVoices','number', ['number']);
GuidoGetPageCount = Module.cwrap('GuidoGetPageCount','number', ['number']);
GuidoGetSystemCount = Module.cwrap('GuidoGetSystemCount','number', ['number', 'number']);

GuidoMakeDate = Module.cwrap('GuidoMakeDate','number', ['number', 'number']);
GuidoGetDateNum = Module.cwrap('GuidoGetDateNum','number', ['number']);
GuidoGetDateDenom = Module.cwrap('GuidoGetDateDenom','number', ['number']);
GuidoFreeDate = Module.cwrap('GuidoFreeDate','', ['number']);

GuidoFreeAR = Module.cwrap('GuidoFreeAR', null, ['number']);
GuidoFreeGR = Module.cwrap('GuidoFreeGR', null, ['number']);

GuidoGetDuration = Module.cwrap('GuidoGetDuration','number', ['number']);

GuidoFindEventPage_p = Module.cwrap('GuidoFindEventPage_p','number', ['number','number']);
GuidoFindPageAt_p = Module.cwrap('GuidoFindPageAt_p','number', ['number','number']);
GuidoGetDatePageDate = Module.cwrap('GuidoGetDatePageDate','number', ['number','number']);

GuidoGetPageMap_JSON = Module.cwrap('GuidoGetPageMap_JSON','number', ['number','number','number','number']);
GuidoGetStaffMap_JSON = Module.cwrap('GuidoGetStaffMap_JSON','number', ['number','number','number','number','number']);
GuidoGetVoiceMap_JSON = Module.cwrap('GuidoGetVoiceMap_JSON','number', ['number','number','number','number','number']);
GuidoGetSystemMap_JSON = Module.cwrap('GuidoGetSystemMap_JSON','number', ['number','number','number','number']);

GuidoDrawBoudingBoxes = Module.cwrap('GuidoDrawBoundingBoxes',null, ['number']);
GuidoGetDrawBoundingBoxes = Module.cwrap('GuidoGetDrawBoundingBoxes', null, []);

GuidoGetAR2GR = Module.cwrap('GuidoGetAR2GR', 'number', ['number', 'number']);

GuidoShutdown = Module.cwrap('GuidoShutdown', null, []);
