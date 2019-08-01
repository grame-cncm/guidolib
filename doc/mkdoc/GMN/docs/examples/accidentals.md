


# Accidentals 

This example shows the use of the \accidental and \alter tags.

~~~~~~
[
	\key<1> \meter<"2/4">
	\acc<dy=-0.5hs,dx=-2hs>( c# ) % set accidental position
	\acc<size=1.4>(d&)            % set accidental size
	\acc( f# )                    % force a sharp
	\acc( c2 )                    % force a natural sign

	\alter<0.5> c                 % \alter is used without range
	\acc<"cautionary", color="blue">(c) % enforce a colored cautionary accidental
	\alter<-0.5>(c) b1            % \alter used with a range, applied to c only
	\alter<0> d a
]
~~~~~~
{!GMN/examples/accidentals.html!}


