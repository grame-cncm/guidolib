
import sys

from PySide import QtCore, QtGui

import PyGuido

class SimpleGUI(QtGui.QMainWindow):
    def __init__(self):
        super(SimpleGUI, self).__init__()
        
        self.resize(600, 300)

        central_widget = QtGui.QWidget()
        self.setCentralWidget(central_widget)
        
        layout = QtGui.QVBoxLayout()
        central_widget.setLayout(layout)
        
        guido_widget = PyGuido.QGuidoWidget()
        
        lineedit_gmn_string = QtGui.QLineEdit()
        lineedit_gmn_string.textChanged.connect(guido_widget.setGMNCode)
        
        layout.addWidget(guido_widget)
        layout.addWidget(lineedit_gmn_string)
        
        # set example Guido string
        lineedit_gmn_string.setText('[c d e f g]')

def main(args):
    qt_app = QtGui.QApplication(args)
    
    # start Guido engine
    guidofont_filename = 'guido2.ttf'
    QtGui.QFontDatabase.addApplicationFont(guidofont_filename)
    PyGuido.QGuidoPainter.startGuidoEngine()
    
    # create SimpleGUI main window
    simple_gui = SimpleGUI()
    simple_gui.show()
    simple_gui.raise_()
    
    qt_app.exec_()

if __name__ == "__main__":
    # call the main() function whenever this script is run as the main script in the Python interpreter
    main(sys.argv)