import os
import unittest

import PyGuido

class TestFloatRect(unittest.TestCase):
    def test_creation(self):
        r1 = PyGuido.FloatRect(1.0, 2.0, 3.0, 4.0)

        r2 = PyGuido.FloatRect()
        r2.Set(r1)
        
        r3 = PyGuido.FloatRect()
        r3.Set(1.0, 2.0, 3.0, 4.0)
        
        for r in [r1, r2, r3]:
            self.assertEqual(r1.left(), 1.0)
            self.assertEqual(r1.top(), 2.0)
            self.assertEqual(r1.right(), 3.0)
            self.assertEqual(r1.bottom(), 4.0)
        
    def test_get_pos(self):
        r = PyGuido.FloatRect(1, 2, 3, 4)
        
        self.assertEqual(1, r.GetXPos())
        self.assertEqual(2, r.GetYPos())
        self.assertEqual((1, 2), r.GetXYPos())
        self.assertEqual((1, 2, 3, 4), r.GetPos())
