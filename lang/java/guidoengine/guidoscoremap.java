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
	A Guido score map.
	<br>
	A guido score map describes the relations between the time and graphic space.
	It is typically used as argument of the guidscore extended mappings methods.
	@see guidoengine.guidoscore#GetVoiceMap guidoscore.GetVoiceMap
	@see guidoengine.guidoscore#GetStaffMap guidoscore.GetStaffMap
	@see guidoengine.guidoscore#GetSystemMap guidoscore.GetSystemMap
*/
public final class guidoscoremap {
	
	private long fMap;


	public static final int kGuidoPage			= 0;
	public static final int kGuidoSystem		= 1;
	public static final int kGuidoSystemSlice	= 2;
	public static final int kGuidoStaff			= 3;
	public static final int kGuidoBar			= 4;
	public static final int kGuidoEvent			= 5;
	
	public				guidoscoremap()		{ fMap = 0; }
	@Override
	protected	void	finalize()			{ dispose(); }
	public		void	dispose()			{ disposeNative(); }

	private native void disposeNative();


	/** Give the map size
		@return the map size
	*/
    public native final synchronized int  size();

	/** Give a relation by index

		@param index the map index
		@param time on output, contains the corresponding time segment.
		@param r on output, contains the corresponding graphic rectangle.
		@return false in case of incorrect index
	*/
    public native final synchronized boolean  get(int index, guidosegment time, rectangle r);

	/** Give a relation by date

		@param date a guido date
		@param time on output, contains the corresponding time segment.
		@param r on output, contains the corresponding graphic rectangle.
		@return true when the date is found in a time segment
	*/
    public native final synchronized boolean  getTime(guidodate date, guidosegment time, rectangle r);

	/** Give a relation by point

		@param x the point x coordinate
		@param y the point y coordinate
		@param time on output, contains the corresponding time segment.
		@param r on output, contains the corresponding graphic rectangle.
		@return true when the point is found in a graphic segment
	*/
    public native final synchronized boolean  getPoint(float x, float y, guidosegment time, rectangle r);
	
	/** Internal jni initialization method.
		Automatically called at package init.
	*/
    protected static native void	Init ();
}
