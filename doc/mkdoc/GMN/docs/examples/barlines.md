

# Barlines 

This example shows the use of barlines, bar format and accolades.

~~~~~~
{
	[ \accol<id=1, range="1-3", dx=-1, type="straightBrace">	 
	  \barFormat<"system", range="1-3"> 
	   c c | f g \doubleBar a f \repeatBegin  d e \repeatEnd e/2 ],
	[  c c | f g \doubleBar a f \repeatBegin  d e \repeatEnd e/2 ], 
	[  c c | f g \doubleBar a f \repeatBegin  d e \repeatEnd e/2 ],

	[  \accol<id=1, range="4-5">
	   \barFormat<"system", range="4-5"> 
	   c c | f g \doubleBar a f \repeatBegin  d e \repeatEnd e/2 ],
	[  c c | f g \doubleBar a f \repeatBegin  d e \repeatEnd e/2 ],

	[  \accol<id=1, range="6-8", type="thinBrace">
	   \barFormat<"system", range="6-8">
	   c c | f g \doubleBar a f \repeatBegin  d e \repeatEnd e/2 ],
	[  c c | f g \doubleBar a f \repeatBegin  d e \repeatEnd e/2 ],
	[  c c | f g \doubleBar a f \repeatBegin  d e \repeatEnd e/2 ]
}
~~~~~~
{!GMN/examples/barlines.html!}

