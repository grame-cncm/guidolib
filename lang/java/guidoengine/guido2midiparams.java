/*
   Copyright (C) 2010 Sony CSL Paris
   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

       * Redistributions of source code must retain the above copyright
         notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above copyright
         notice, this list of conditions and the following disclaimer in the
         documentation and/or other materials provided with the distribution.
       * Neither the name of Sony CSL Paris nor the names of its contributors 
         may be used to endorse or promote products derived from this software 
         without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


package guidoengine;

/**
	Guido to MIDI file settings
*/
public class guido2midiparams
{
	private final int kMaxVoices = 256;
	public int		fTempo;				///< default tempo in quarter per minute - default value: 120
	public int		fTicks;				///< ticks per quarternote - default value: 960 (64*3*5)
	public int		fChan;				///< the default Midi channel - default value: 1

	public float	fIntensity;			///< default intensity [0.0 ... 1.0] - default value: 0.8
	public float	fAccentFactor;		///< accent intensity factor - default value: 1.1
	public float	fMarcatoFactor;		///< marcato intensity factor - default value: 1.2

	public float	fDFactor;			///< default duration factor [0.0 ... 1.0] - default value: 0.8
	public float	fStaccatoFactor;	///< staccato duration factor - default value: 0.5
	public float	fSlurFactor;		///< legato duration factor  - default value: 1.0
	public float	fTenutoFactor;		///< tenuto duration factor  - default value: 0.90
	public float	fFermataFactor;		///< fermata duration factor - default value: 2.0
	
	public int[]	fVoice2Chan;

	public guido2midiparams() { 
		fTempo	= 120;
		fTicks	= 960;
		fChan	= 1;
		fIntensity		= 0.8f;
		fAccentFactor	= 1.1f;
		fMarcatoFactor	= 1.2f;
		fDFactor		= 0.8f;
		fStaccatoFactor = 0.5f;
		fSlurFactor		= 1.0f;
		fTenutoFactor	= 0.9f;
		fFermataFactor	= 2.0f;
		fVoice2Chan = new int[kMaxVoices];
		for (int i=0; i<kMaxVoices; i++)
			fVoice2Chan[i] = 0;
	}

	public void mapvoice(int voice, int chan) {
		if (voice < kMaxVoices)
			fVoice2Chan[voice] = chan;
	}

	/** Print utility.
	*/
	public void print () {
        System.out.println("guido2midiparams tempo: " + fTempo + " ticks: " + fTicks 
			+ " chan: " + fChan + " intensity/accent/marcato: " + fIntensity + "/" + fAccentFactor + "/" + fMarcatoFactor
			+ " duration: " +  fDFactor + " stacc/slur/tenuto: " + fStaccatoFactor + "/" + fSlurFactor + "/" + fTenutoFactor
			+ " fermata: " + fFermataFactor);
	}

	/** Internal jni initialization method.
		Automatically called at package init.
	*/
    protected static native void	Init ();
}
