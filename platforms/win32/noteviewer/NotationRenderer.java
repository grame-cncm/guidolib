/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

// package nview;

import java.io.*;

public class NotationRenderer
{

	public NotationRenderer()
	{
		// here we need to initialise stuff ...
	}

	// this parses the file ...
	// returns handle or negative number
	// if error occurs
	public native int parse(String filename);

	// this frees the handle.
	public native void free(int handle);

	// this gets the number of pages
	// for a piece
	public native int getNumPages(int handle);

	// this creates a GIF-Picture ...
	public native byte[] getGIF(int handle,
		float zoom,
		int pagenum,
		int adjustpagesize);

    	public native int putGuidoGraphicStream(
					     int handle,
					     int page,
					     String ggs);
					     
	public native void getGuidoGraphicStream(
		int handle,
		int page,
		PrintStream os);


    	public native void getGuido(int handle,
				PrintStream os);

	public native String getGraphicInfo(
		int handle,
		int gid,
		int infotype);

	public native int getTestGID(int handle);

	// load the dynamic link library ...
	static
	{
		System.loadLibrary("nview32");
	}

	public static void main(String[] argv)
	{

		NotationRenderer nr = new NotationRenderer();

		System.out.println("Now calling parse ...");

		
		int handle = nr.parse(argv[0]);

		System.out.println("Parse returned " +
			Integer.toString(handle));

		if (handle<0)
			return;

		System.out.println("now we call getNumPages");

		int numpages = nr.getNumPages(handle);

		System.out.println("getnumpages returned "
			+ Integer.toString(numpages));

		System.out.println("calling getGIF ...");

		// using adjustpagesize
		byte[] picture = nr.getGIF(handle,
			(float) 1.0,1,1);

		System.out.println("Accessing the picture ...");

		if (picture != null)
		{
			int length = picture.length;


			System.out.println("Picture is a byte-array with " 
			 +Integer.toString(length) + " bytes");

		}

		System.out.println("Now doing another zoom ...");
		// no pagesizeadjust ...
		picture = nr.getGIF(handle,(float) 0.5,1,0);

		if (picture != null)
		{
			System.out.println("getGif returned picture with " +
				Integer.toString(picture.length) + " bytes");
		} else
		    System.out.println("getGIF did not return picture");

		System.out.println("Now doing GuidoGraphicStream for first page");
		nr.getGuidoGraphicStream(handle,1,System.out);

		System.out.println("Now putting stuff into the renderer");
		nr.putGuidoGraphicStream(handle,1,"\\add<\"qnote\",650,320>\n");

		System.out.println("Now rereading GGS ... ");
		nr.getGuidoGraphicStream(handle,1,System.out);

		System.out.println("now calling getGuido ...");
		nr.getGuido(handle,System.out);
		System.out.println("");

		System.out.println("Now testing Graphic Info");
		int gid = nr.getTestGID(handle);
		System.out.println("GetTestGID returned " +
			Integer.toString(gid));

		String mystr = nr.getGraphicInfo(handle,
		 gid,0);

		if (mystr != null)
			System.out.println("getGraphicInfo returned " +
				mystr);
		else
			System.out.println("Could not get info");

		nr.free(handle);


	}
}
