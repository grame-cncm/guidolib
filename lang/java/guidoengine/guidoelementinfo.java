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
import guidoengine.*;

/**
	Guido score element basic description
	
	guidoelementinfo is basically a data structure used by the score map API.
	@see guidoscore#GetMap
	@see mapcollector#Graph2TimeMap
*/
public final class guidoelementinfo {
	public static final int kNote			= 1;
	public static final int kRest			= 2;
	public static final int kEmpty			= 3;
	public static final int kBar			= 4;
	public static final int kRepeatBegin	= 5;
	public static final int kRepeatEnd		= 6;
	public static final int kStaff			= 7;
	public static final int kSystemSlice	= 8;
	public static final int kSystem			= 9;
	public static final int kPage			= 10;

	/** the event type ({@code kNote, kBar,...})
    */
	public int type;
	/** the current staff number or 0 when na
    */
	public int staffNum;
	/** the current voice number or 0 when na
    */
	public int voiceNum;

	public guidoelementinfo () { type = staffNum = voiceNum = 0; }
	
	public guidoelementinfo (int t, int sn, int vn) {
		type = t;
		staffNum = sn;
		voiceNum = vn;
	}

	/** Internal jni initialization method.
		Automatically called at package init.
	*/
    protected static native void	Init ();
}
