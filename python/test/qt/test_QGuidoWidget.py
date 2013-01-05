import os
import unittest

from PySide import QtGui, QtCore
        
import PyGuido

class GuidoTestWidget(PyGuido.QGuidoWidget):
    pass

class TestGuidoQtImport(unittest.TestCase):
    def setUp(self):
        qt_app = QtGui.QApplication([])

        # initialize GuidoLib
        guidofont_filename = os.path.join('..', 'lib', 'guido2.ttf')
        self.assertEqual(0, QtGui.QFontDatabase.addApplicationFont(guidofont_filename))
        PyGuido.QGuidoPainter.startGuidoEngine()
        
        self.guido_widget = GuidoTestWidget()
        
    def tearDown(self):
        PyGuido.QGuidoPainter.stopGuidoEngine()
        
    def test_start_and_stop(self):
        # do nothing except for setUp and tearDown
        pass
