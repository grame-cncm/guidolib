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

import java.awt.Rectangle;

/**
 * A Guido score map. <br>
 * It an extension on guidoscoremapbase which use java.awt.Rectangle instead of guidorect.
 * This class is maintain for backward compatibility.
 */
public final class guidoscoremap extends guidoscoremapbase {

	public guidoscoremap() {
		super();
	}

	/**
	 * Give a relation by index
	 * 
	 * @param index
	 *            the map index
	 * @param time
	 *            on output, contains the corresponding time segment.
	 * @param r
	 *            on output, contains the corresponding graphic Rectangle.
	 * @return false in case of incorrect index
	 */
	public boolean get(int index, guidosegment time, Rectangle r) {
		guidorect guidorect = new guidorect();
		boolean returnValue = super.get(index, time, guidorect);

		r.setRect(guidorect.getLeft(), guidorect.getTop(), guidorect.getRight() - guidorect.getLeft(),
				guidorect.getBottom() - guidorect.getTop());
		return returnValue;
	}

	/**
	 * Give a relation by date
	 * 
	 * @param date
	 *            a guido date
	 * @param time
	 *            on output, contains the corresponding time segment.
	 * @param r
	 *            on output, contains the corresponding graphic Rectangle.
	 * @return true when the date is found in a time segment
	 */
	public boolean getTime(guidodate date, guidosegment time, Rectangle r) {
		guidorect guidorect = new guidorect();
		boolean returnValue = super.getTime(date, time, guidorect);
		
		r.setRect(guidorect.getLeft(), guidorect.getTop(), guidorect.getRight() - guidorect.getLeft(),
				guidorect.getBottom() - guidorect.getTop());
		return returnValue;
	}

	/**
	 * Give a relation by point
	 * 
	 * @param x
	 *            the point x coordinate
	 * @param y
	 *            the point y coordinate
	 * @param time
	 *            on output, contains the corresponding time segment.
	 * @param r
	 *            on output, contains the corresponding graphic Rectangle.
	 * @return true when the point is found in a graphic segment
	 */
	public boolean getPoint(float x, float y, guidosegment time, Rectangle r) {
		guidorect guidorect = new guidorect();
		boolean returnValue = super.getPoint(x, y, time, guidorect);
		
		r.setRect(guidorect.getLeft(), guidorect.getTop(), guidorect.getRight() - guidorect.getLeft(),
				guidorect.getBottom() - guidorect.getTop());
		return returnValue;
	}
}
