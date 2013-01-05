
from setuptools import setup

setup(
      name =            'SimpleGUI',
      version =         '1.0.0',
      author =          'Benjamin Rupprechter',
      author_email =    'rupprechter@collegium.ethz.ch',
      description =     ('SimpleGUI demonstrating PyGuido bindings.'),
      app =             ['simple_gui.py'],
      options =         {'py2app': dict(
                                        semi_standalone = False,
                                        strip = False,
                                        plist = dict(
                                                     # need to set this manually to fix bug in py2app 0.7.1
                                                     PyRuntimeLocations = ['@executable_path/../Frameworks/Python'],
                                                     ),
                                        )},
      data_files =      ['guido2.ttf'],
      setup_requires =  ['py2app'],
)
