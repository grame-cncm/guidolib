


# Slurs 

This example shows nested and interleaved slurs. It makes also use of micro-tonality.

~~~~~~
[
	a/2 \slur( e2/4 
	\slurBegin:1 \alter<detune=0.5>(g1) \slurBegin:3<curve="up"> e 
	\slurBegin:2<r3=0.6> c  \alter<detune=-0.5>(e) 
	\slurEnd:1 f \slurEnd:3  \alter<detune=-1.5>(g) 
	\slurEnd:2 a/1 )
]
~~~~~~
{!GMN/examples/slurs.html!}


