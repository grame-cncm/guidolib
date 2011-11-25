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

import java.awt.*;
import java.awt.event.*;
import java.awt.print.*;
import java.io.*;
import javax.swing.*;
import javax.swing.event.*;
import guidoengine.*;

public class guidoviewer {  
   
    public static void main(String[] args) {
        JFrame win = new guidoviewerGUI();
        win.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        win.setVisible(true);
    }
}
   
class scorePanel extends Canvas implements Printable {
 	static {
        try {
			System.loadLibrary("jniGUIDOEngine");
			guido.Init("Guido2", "Times");
			if (guido.xml2gmn())
				System.out.println("libMusicXML v." + guido.musicxmlversion() + 
					" with GMN converter v." + guido.musicxml2guidoversion());
		} catch (UnsatisfiedLinkError e) {
			System.err.println("Native code library failed to load.\n" + e);
		}
	}

	guidoscore m_gmnscore;
	
	public scorePanel()						{ m_gmnscore = new guidoscore(); }
	public Dimension getPreferredSize()		{ return new Dimension(500,600); }


	public void setGMN(String str, boolean gmncode) {
		m_gmnscore.close();
		int err = gmncode ? m_gmnscore.ParseString(str) : m_gmnscore.ParseFile(str);
		if (err != guido.guidoNoErr) {
			String msg = gmncode ? 
				  new String("Error reading string:\n") + guido.GetErrorString (err)
				: new String("Error opening ") + str + ":\n" + guido.GetErrorString (err);
			if (err == guido.guidoErrParse) {
				if (guido.xml2gmn() && !gmncode) {
				System.out.println("try xml 2 guido conversion:");
					String gmn = guido.xml2gmn(str);
//				System.out.println(gmn);
					setGMN (gmn, true);
					return;
				}	
				msg += " line " + guido.GetParseErrorLine();
			}
			JOptionPane.showMessageDialog(this, msg);
			m_gmnscore = null;
		}
		else {
			err = m_gmnscore.AR2GR();
			if (err != guido.guidoNoErr) {
				JOptionPane.showMessageDialog(this, "Error converting AR to GR " + str + ":\n" + guido.GetErrorString (err));
				m_gmnscore = null;
			}
			else {
				m_gmnscore.ResizePageToMusic();
				repaint();
			}
		}
	}

	public void paint(Graphics g) {
		if (m_gmnscore.fGRHandler != 0) {
			guidodrawdesc desc = new guidodrawdesc(getSize().width, getSize().height);		
			int ret = m_gmnscore.Draw (g, getSize().width, getSize().height, desc, new guidopaint());
			if (ret != guido.guidoNoErr)
				System.err.println("error drawing score: " + guido.GetErrorString(ret));
		}
	}
	
	public int print(Graphics g, PageFormat pf, int page) throws PrinterException {
		if (page >= m_gmnscore.GetPageCount())
			return NO_SUCH_PAGE;
		if (m_gmnscore.fGRHandler != 0) {
			Graphics2D g2d = (Graphics2D)g;
			g2d.translate(pf.getImageableX(), pf.getImageableY());
			int w = (int)pf.getImageableWidth();
			int h = (int)pf.getImageableHeight();
			guidodrawdesc desc = new guidodrawdesc(w, h);
			desc.fPage = page+1;
			int ret = m_gmnscore.Draw (g, w, h, desc, new guidopaint());
			if (ret != guido.guidoNoErr)
				System.err.println("error printing score: " + guido.GetErrorString(ret));
		}		
		return PAGE_EXISTS;
	}
}


///// guidoviewerGUI
class guidoviewerGUI extends JFrame {
    String	 m_Title = "Guido Viewer Java";
	Menu     m_fileMenu = new Menu("File");		// declare and create new menu
    MenuItem m_openItem = new MenuItem("Open");	// create new menu item
    MenuItem m_printItem = new MenuItem("Print");	// create new menu item
    MenuItem m_quitItem = new MenuItem("Quit");	// another menu item
	scorePanel m_score = new scorePanel();

    
    public guidoviewerGUI() {
        //... Add listeners to menu items
        m_openItem.addActionListener(new OpenAction(this));
        m_printItem.addActionListener(new PrintAction(m_score));
        m_quitItem.addActionListener(new QuitAction());
        
        //... Menubar, menus, menu items.  Use indentation to show structure.
        MenuBar menubar = new MenuBar();  // declare and create new menu bar
		menubar.add(m_fileMenu);			// add the menu to the menubar
		m_fileMenu.add(m_openItem);			// add the menu item to the menu
		m_fileMenu.add(m_printItem);		// add the menu item to the menu
		m_fileMenu.addSeparator();			// add separator line to menu
		m_fileMenu.add(m_quitItem);
        
        //... Content pane: create, layout, add components
        JPanel content = new JPanel();
        content.setBackground(Color.white);
        content.setLayout(new BorderLayout());
        content.add(m_score, BorderLayout.CENTER);

        //... Set JFrame's menubar, content pane, and title.
        this.setContentPane(content);       // Set windows content pane..
        this.setMenuBar(menubar);          // Set windows menubar.
        this.setTitle(m_Title);
        this.pack();
    }
    
    public void setGMNFile(File file) {
        this.setTitle(m_Title + " - " + file.getName());
 		m_score.setGMN(file.getPath(), false); 
	}

	//// OpenAction
	class OpenAction implements ActionListener {
		guidoviewerGUI m_viewer;
        public OpenAction(guidoviewerGUI viewer)	{ m_viewer = viewer; }
		public void actionPerformed(ActionEvent e) {
			JFileChooser chooser = new JFileChooser();
    		if(chooser.showOpenDialog(null) == JFileChooser.APPROVE_OPTION)
				m_viewer.setGMNFile(chooser.getSelectedFile());
    	}
    }

	//// PrintAction
	class PrintAction implements ActionListener {
         scorePanel m_score;
         public PrintAction(scorePanel score)	{ m_score = score; }
		 public void actionPerformed(ActionEvent e) {
			 PrinterJob job = PrinterJob.getPrinterJob();
			 job.setPrintable(m_score);
			 boolean ok = job.printDialog();
			 if (ok) {
				 try {
					  job.print();
				 } catch (PrinterException ex) {
					System.err.println("printing exception: " + ex);
				 }
			 }
		}
    }
    
	/// QuitAction
    class QuitAction implements ActionListener {
        public void actionPerformed(ActionEvent e) { System.exit(0); }
    }
}
