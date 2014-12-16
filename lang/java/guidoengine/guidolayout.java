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
    Global settings of the Guido Engine for the graphic score layout.
*/
public class guidolayout
{
	public static final int kAutoDistrib	= 1;
	public static final int kAlwaysDistrib	= 2;
	public static final int kNeverDistrib	= 3;


    /** Distance between systems
	
		Distance is in internal units (default value: 75)
	*/
	public float fSystemsDistance;

    /** Systems distribution.

		Possible values: kAutoDistrib (default), kAlwaysDistrib, kNeverDistrib
	*/
	public int fSystemsDistribution;

	/** Maximum distance allowed between two systems.
	
		Used in automatic distribution mode.
		Distance is relative to the height of the inner page.
		Default value: 0.25 (that is: 1/4 of the page height)
	*/
	public float fSystemsDistribLimit;

	/** Force value of the Space-Force function.
	
        Typical values range from 400 to 1500. Default value: 750
    */
	public float   fForce;

    /** Spring parameter

        Typical values range from 1 to 5. Default value: 1.1
    */
    public float   fSpring;

    /** Spacing algorithm control
	
		Tells the engine to use the Neighborhood spacing algorithm or not
  		(default value: 0)
	*/
	 public int fNeighborhoodSpacing;


	/** Optimal page fill algorithm control
	
		Tells the engine to use the optimal page fill algorithm or not
		(default value: 1)
	*/
	public boolean fOptimalPageFill;

	/**
        Retrieves the engine settings values.
		
		On output, the guidolayout structure contains the engine settings values.
	*/
	public native final void	GetDefault();
	
	public guidolayout() {
		fSystemsDistance = 75;
		fSystemsDistribution = kAutoDistrib;
		fSystemsDistribLimit = 0.25f;
		fForce = 750;
		fSpring = 1.1f;
		fNeighborhoodSpacing = 0;
		fOptimalPageFill = true;
	}

	/** Print utility.
	*/
	public void print () {
        System.out.println("guidolayout sysDistance: " + fSystemsDistance + " sysDistr: " + fSystemsDistribution 
			+ " sysDistrLimit: " + fSystemsDistribLimit + " force: " + fForce + " spring: " + fSpring 
			+ " NeighborhoodSpacing: " + fNeighborhoodSpacing + " OptimalPageFill: " + fOptimalPageFill);
	}

	/** Internal jni initialization method.
		Automatically called at package init.
	*/
    protected static native void	Init ();
}
