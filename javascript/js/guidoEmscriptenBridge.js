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
GuidoGetNewPageDate = Module.cwrap('GuidoGetNewPageDate','number', ['number','number']);

GuidoGetPageMap_JSON = Module.cwrap('GuidoGetPageMap_JSON','number', ['number','number','number','number']);
GuidoGetStaffMap_JSON = Module.cwrap('GuidoGetStaffMap_JSON','number', ['number','number','number','number','number']);
GuidoGetVoiceMap_JSON = Module.cwrap('GuidoGetVoiceMap_JSON','number', ['number','number','number','number','number']);
GuidoGetSystemMap_JSON = Module.cwrap('GuidoGetSystemMap_JSON','number', ['number','number','number','number']);

GuidoDrawBoudingBoxes = Module.cwrap('GuidoDrawBoundingBoxes',null, ['number']);
GuidoGetDrawBoundingBoxes = Module.cwrap('GuidoGetDrawBoundingBoxes', null, []);

GuidoGetNewPageFormat = Module.cwrap('GuidoGetNewPageFormat', 'number', ['number', 'number']); 
GuidoGetNewDefaultPageFormat = Module.cwrap('GuidoGetNewDefaultPageFormat', 'number', []); 
GuidoSetDefaultPageFormat = Module.cwrap('GuidoSetDefaultPageFormat', null, ['number']);
GuidoDeletePageFormat = Module.cwrap('GuidoDeletePageFormat', null, ['number']);
GuidoResizePageToMusic = Module.cwrap('GuidoResizePageToMusic', 'number', ['number']);

GuidoUnit2CM = Module.cwrap('GuidoUnit2CM', 'number', ['number']);
GuidoCM2Unit = Module.cwrap('GuidoCM2Unit', 'number', ['number']);
GuidoUnit2Inches = Module.cwrap('GuidoUnit2Inches', 'number', ['number']);
GuidoInches2Unit = Module.cwrap('GuidoInches2Unit', 'number', ['number']);

GuidoGetAR2GR = Module.cwrap('GuidoGetAR2GR', 'number', ['number', 'number']);
GuidoUpdateGR = Module.cwrap('GuidoUpdateGR', 'number', ['number', 'number']); 

GuidoGetVersionStr = Module.cwrap('GuidoGetVersionStr', 'number', []);
GuidoCheckVersionNums = Module.cwrap('GuidoCheckVersionNums', 'number', ['number', 'number', 'number']);
GuidoGetLineSpace = Module.cwrap('GuidoGetLineSpace', 'number', []);

//GuidoGetNewDefaultLayoutSettings = Module.cwrap('GuidoGetNewDefaultLayoutSettings', 'number', ['number']);
GuidoDeleteLayoutSettings = Module.cwrap('GuidoDeleteLayoutSettings', null, ['number']);

GuidoGetArError = Module.cwrap('GuidoGetGrError', 'number', ['number']);
GuidoGetGrError = Module.cwrap('GuidoGetGrError', 'number', ['number']);
GuidoGetErrorString = Module.cwrap('GuidoGetErrorString', 'number', ['number']);

GuidoShutdown = Module.cwrap('GuidoShutdown', null, []);
