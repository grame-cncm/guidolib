import os
import unittest

import PyGuido

class TestBasicInformation(unittest.TestCase):
    def setUp(self):
        init_desc = PyGuido.GuidoInitDesc()
        self.assertEqual(PyGuido.GuidoErrCode.guidoNoErr, PyGuido.GuidoInit(init_desc))
        
    def tearDown(self):
        PyGuido.GuidoShutdown()
        
    def test_init_and_shutdown(self):
        # do nothing except for init and shutdown
        pass
    
    def test_unit_conversion(self):
        for val in [1.0, 10.0, 50.0]:
            self.assertAlmostEqual(PyGuido.GuidoCM2Unit(PyGuido.GuidoUnit2CM(val)), val, places=5)
            self.assertAlmostEqual(PyGuido.GuidoUnit2CM(PyGuido.GuidoCM2Unit(val)), val, places=5)
    
    def test_version(self):
        # get version as string and numbers
        version_str = PyGuido.GuidoGetVersionStr()
        major, minor, sub = PyGuido.GuidoGetVersionNums()
        
        # check that both results are the same
        version_str_list = version_str.split('.')
        self.assertEqual(len(version_str_list), 3)
        self.assertEqual(version_str_list[0], str(major))
        self.assertEqual(version_str_list[1], str(minor))
        self.assertEqual(version_str_list[2], str(sub))

        # ensure that checking the version works
        self.assertEqual(PyGuido.GuidoErrCode.guidoNoErr, PyGuido.GuidoCheckVersionNums(major, minor, sub))
        self.assertEqual(PyGuido.GuidoErrCode.guidoNoErr, PyGuido.GuidoCheckVersionNums(major-1, minor, sub))
        self.assertEqual(PyGuido.GuidoErrCode.guidoErrActionFailed, PyGuido.GuidoCheckVersionNums(major+1, minor, sub))
        
    def test_line_space(self):
        self.assertEqual(PyGuido.GuidoGetLineSpace(), 50.0)
        