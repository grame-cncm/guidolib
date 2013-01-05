import os
import unittest

test_directories = [
                    'base',
                    'mac',
                    'qt',
                    ]

def load_tests(loader, tests, pattern):
    suite = unittest.TestSuite()
    
    for dir in test_directories:
        suite.addTest(unittest.defaultTestLoader.discover(dir, top_level_dir = '.'))

    return suite

if __name__ == '__main__':
    unittest.main()
