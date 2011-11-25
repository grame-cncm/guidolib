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
	Guido page format
	
	The Guido language includes a {@code \\pageFormat} tag to specify the page layout
	within the score descritpion. When a guido score description doesn't include this 
	{@code \\pageFormat} tag, the guido engine applies a default page format.
	The guidopageformat is basically a data structure used to control the default page 
	format strategy of the score layout engine.
*/
public class guidopageformat
{
    public float fWidth;
    public float fHeight;
    public float fMarginleft;
    public float fMargintop;
    public float fMarginright;
    public float fMarginbottom;

	/** Retrieve the engine default page format.
	*/
    public native final void  GetDefault ();

	/** Sets the engine default score page format.

		The default page format is used when no {@code \\pageFormat} tag is present.
		Parameters are Guido internal units. Default values for the default page
        format are:
        - paper size: A4
        - left margin: 2cm
        - right margin: 2cm
        - top margin: 5cm
        - bottom margin: 3cm
	*/
    public native final void  SetDefault ();

	public guidopageformat() { 
		fWidth		= 21.f;
		fHeight		= 29.7f;
		fMarginleft	= fMargintop = fMarginright = fMarginbottom	= 2.f;
		GetDefault(); 
	}

	public guidopageformat(float w, float h, float ml, float mt, float mr, float mb) {
		fWidth		= w;
		fHeight		= h;
		fMarginleft		= ml;
		fMargintop		= mt;
		fMarginright	= mr;
		fMarginbottom	= mb;
	}

	/** Print utility.
	*/
	public void print () {
        System.out.println("guidopageformat size: " + fWidth + " : " + fHeight 
			+ " margins: " + fMarginleft + " : " + fMargintop + " : " + fMarginright + " : " + fMarginbottom );
	}

	/** Internal jni initialization method.
		Automatically called at package init.
	*/
    protected static native void	Init ();
}
