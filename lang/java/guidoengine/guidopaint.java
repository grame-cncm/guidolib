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
	
	guidopaint is basically a data structure used for clipping.
    Only systems that intersect with this rectangle will be drawn.
	Coordinates should be given in internal units.
	@see guidoscore#Draw
*/
public class guidopaint {
   /** a flag to ignore the following rect and to redraw everything
   */
    public boolean fErase;
	
    //! Absolute Guido virtual coordinates of the clipping rectangle.
    //! Only systems that intersect with this rectangle will be drawn.
    public int fLeft;
	public int fTop;
	public int fRight;
	public int fBottom;

	public guidopaint() {
		fErase = true;
		fLeft = fTop = 0;
		fRight = fBottom = 100000;
	}

	public guidopaint(int left, int top, int right, int bottom) {
		fErase = false;
		fLeft = left;
		fTop = top;
		fRight = right;
		fBottom = bottom;
	}

	/** Print utility.
	*/
	public void print() {
       System.out.println("guidopaint ltrb: " + fLeft + "/" + fTop + "/" + fRight + "/" + fBottom + " erase: " + fErase);
	}

	/** Internal jni initialization method.
		Automatically called at package init.
	*/
    protected static native void	Init ();
}
