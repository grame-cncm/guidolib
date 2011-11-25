{
	"patcher" : 	{
		"fileversion" : 1,
		"rect" : [ 59.0, 67.0, 525.0, 452.0 ],
		"bglocked" : 0,
		"defrect" : [ 59.0, 67.0, 525.0, 452.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "See:\n• the guido project at http://guidolib.sourceforge.net for more information about Guido and for the Guido language specification.\n• the libmusicxml project at http://libmusicxml.sourceforge.net/ for more information about the musicxml library.",
					"linecount" : 8,
					"presentation_linecount" : 8,
					"numoutlets" : 0,
					"presentation_rect" : [ 13.498806, 300.54306, 267.0, 113.0 ],
					"fontsize" : 11.600006,
					"patching_rect" : [ 13.498806, 300.54306, 267.0, 113.0 ],
					"presentation" : 1,
					"id" : "obj-10",
					"numinlets" : 1,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "You must have the Guido shared library installed to use the guido external.\nOptionally, you should have the libmusicxml2 library installed to support the MusicXML format as file input. ",
					"linecount" : 5,
					"presentation_linecount" : 5,
					"numoutlets" : 0,
					"presentation_rect" : [ 13.498806, 219.54306, 266.0, 73.0 ],
					"fontsize" : 11.600006,
					"patching_rect" : [ 13.498806, 219.54306, 266.0, 73.0 ],
					"presentation" : 1,
					"id" : "obj-8",
					"numinlets" : 1,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "version 1.01 by D. Fober",
					"numoutlets" : 0,
					"presentation_rect" : [ 13.0, 423.0, 182.0, 20.0 ],
					"fontsize" : 12.0,
					"patching_rect" : [ 13.0, 423.0, 182.0, 20.0 ],
					"presentation" : 1,
					"id" : "obj-27",
					"numinlets" : 1,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"numoutlets" : 0,
					"presentation_rect" : [ 326.0, 420.0, 34.0, 20.0 ],
					"fontsize" : 12.0,
					"patching_rect" : [ 326.0, 420.0, 34.0, 20.0 ],
					"presentation" : 1,
					"id" : "obj-7",
					"numinlets" : 1,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read bach.gmn",
					"numoutlets" : 1,
					"presentation_rect" : [ 403.0, 113.0, 91.0, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 403.0, 113.0, 91.0, 18.0 ],
					"presentation" : 1,
					"id" : "obj-5",
					"numinlets" : 2,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "page 1",
					"numoutlets" : 1,
					"presentation_rect" : [ 358.0, 169.0, 47.0, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 358.0, 169.0, 47.0, 18.0 ],
					"presentation" : 1,
					"id" : "obj-3",
					"numinlets" : 2,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "read",
					"numoutlets" : 1,
					"presentation_rect" : [ 358.0, 113.0, 35.0, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 358.0, 113.0, 35.0, 18.0 ],
					"presentation" : 1,
					"id" : "obj-2",
					"numinlets" : 2,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "reset",
					"numoutlets" : 1,
					"presentation_rect" : [ 358.0, 141.0, 37.0, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 358.0, 141.0, 37.0, 18.0 ],
					"presentation" : 1,
					"id" : "obj-9",
					"numinlets" : 2,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"presentation_rect" : [ 294.0, 167.0, 20.0, 20.0 ],
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 294.0, 167.0, 20.0, 20.0 ],
					"presentation" : 1,
					"id" : "obj-6",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "set \"[ g a b c ]\"",
					"numoutlets" : 1,
					"presentation_rect" : [ 358.0, 83.0, 88.0, 18.0 ],
					"outlettype" : [ "" ],
					"fontsize" : 12.0,
					"patching_rect" : [ 358.0, 83.0, 88.0, 18.0 ],
					"presentation" : 1,
					"id" : "obj-11",
					"numinlets" : 2,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "guido",
					"numoutlets" : 1,
					"presentation_rect" : [ 326.0, 207.0, 140.0, 203.0 ],
					"outlettype" : [ "" ],
					"patching_rect" : [ 326.0, 207.0, 140.0, 203.0 ],
					"presentation" : 1,
					"id" : "obj-1",
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "guido displays music scores.\nIt is based on the Guido Music Notation language and accepts litteral Guido code or Guido files as input. Scores are automatically resized to the box size.\nIn addition and when the libmusicxml2 library is available, it also supports the MusicXML format as file input.",
					"linecount" : 8,
					"presentation_linecount" : 8,
					"numoutlets" : 0,
					"presentation_rect" : [ 17.498806, 85.54306, 265.0, 113.0 ],
					"fontsize" : 11.600006,
					"patching_rect" : [ 17.498806, 85.54306, 265.0, 113.0 ],
					"presentation" : 1,
					"id" : "obj-30",
					"numinlets" : 1,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_title",
					"text" : "guido",
					"numoutlets" : 0,
					"presentation_rect" : [ 20.0, 20.0, 485.0, 30.0 ],
					"fontface" : 3,
					"fontsize" : 20.871338,
					"frgb" : [ 0.2, 0.2, 0.2, 1.0 ],
					"patching_rect" : [ 20.0, 20.0, 485.0, 30.0 ],
					"presentation" : 1,
					"id" : "obj-40",
					"textcolor" : [ 0.2, 0.2, 0.2, 1.0 ],
					"numinlets" : 1,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"varname" : "autohelp_top_digest",
					"text" : "Music score support based on the Guido Engine",
					"numoutlets" : 0,
					"presentation_rect" : [ 20.0, 48.0, 485.0, 21.0 ],
					"fontsize" : 12.754705,
					"frgb" : [ 0.2, 0.2, 0.2, 1.0 ],
					"patching_rect" : [ 20.0, 48.0, 485.0, 21.0 ],
					"presentation" : 1,
					"id" : "obj-41",
					"textcolor" : [ 0.2, 0.2, 0.2, 1.0 ],
					"numinlets" : 1,
					"fontname" : "Arial"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "panel",
					"varname" : "autohelp_top_panel[2]",
					"numoutlets" : 0,
					"presentation_rect" : [ 19.0, 20.0, 495.0, 52.0 ],
					"grad1" : [ 0.682353, 0.776471, 0.823529, 1.0 ],
					"mode" : 1,
					"patching_rect" : [ 19.0, 20.0, 495.0, 52.0 ],
					"grad2" : [ 0.9, 0.9, 0.9, 1.0 ],
					"presentation" : 1,
					"id" : "obj-4",
					"numinlets" : 1,
					"background" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 367.5, 109.5, 335.5, 109.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 367.5, 137.5, 335.5, 137.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 367.5, 166.5, 335.5, 166.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 367.5, 196.5, 335.5, 196.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 412.5, 137.5, 335.5, 137.5 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 303.5, 197.0, 335.5, 197.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
