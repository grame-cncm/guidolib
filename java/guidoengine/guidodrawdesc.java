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
	Guido score drawing descriptor
	
	guidodrawdesc is basically a data structure used to indicate how to draw a score
	to the guido engine.
	@see guidoscore#Draw
*/
public class guidodrawdesc
{
	/** The page number. Starts from 1.
	*/
    public int fPage;

	/** Indicates the coordinates of the score point that will appear at the graphic
        origin. 
		
		Typical values are 0. Non null values have the effect of moving a window
        over the score page, like scroll bars that move a page view.
        Units are guido internal units.
    */
	public int fScrollx, fScrolly;

	/** Indicates the size of the drawing area.
        
		Units are device units (typically pixels).
    */
    public int fWidth, fHeight;

	/** Flag for printing.
        
		If true, the engine ignores scroll and sizes parameters.
		(probably obsolete now!) 
    */
    public boolean fIsprint;

	public guidodrawdesc() {
		fPage = 1;
		fWidth = fHeight = 100;
		fScrollx = fScrolly = 0;
		fIsprint = false;
	}

	public guidodrawdesc(int w, int h) {
		fPage = 1;
		fScrollx = fScrolly = 0;
		fWidth = w;
		fHeight = h;
		fIsprint = false;
	}


	/** Print utility.
	*/
	public void print() {
       System.out.println("guidodrawdesc page: " + fPage + " scrollx/y: " + fScrollx + "/" + fScrolly 
			+ " w/h: " + fWidth + "/" + fHeight + " print: " + fIsprint);
	}

	/** Internal jni initialization method.
		Automatically called at package init.
	*/
    protected static native void	Init ();
};

