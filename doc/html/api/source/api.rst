API
===

This page provides all of the calls avaiable in the GUIDOEngine web
API.  For all of the examples below, we assume that the base URL
of the server is ``http://faust.grame.fr`` running on port ``8000``.

Setter calls
------------

GMN
^^^

The following call passes in Guido Music Notation code to the server
that will be visualized::

  http://faust.grame.fr:8000/?gmn=[ _/8 \slur(c1 d e) {fis/2, ais} e/2 ]

Resulting in:

.. image:: setGmn.png

Values must be valid GMN.

Page
^^^^

The following call sets the page to be displayed:

.. parsed-literal::
  `http://faust.grame.fr:8000/?page=1 <http://faust.grame.fr:8000/?page=1>`_

Resulting in:

.. image:: setPage.png

Values must be integer values greater than 0.

Width
^^^^^

The following call sets the width of the page:

.. parsed-literal::
  `http://faust.grame.fr:8000/?width=800 <http://faust.grame.fr:8000/?width=800>`_

Resulting in:

.. image:: setWidth.png

Values must be floating point or integer values greater than 0.

Height
^^^^^^

The following call sets the height of the page:

.. parsed-literal::
  `http://faust.grame.fr:8000/?height=800 <http://faust.grame.fr:8000/?height=800>`_

Resulting in:

.. image:: setHeight.png

Values must be floating point or integer values greater than 0.

Left margin
^^^^^^^^^^^

The following call sets the left margin of the page:

.. parsed-literal::
  `http://faust.grame.fr:8000/?marginleft=10 <http://faust.grame.fr:8000/?marginleft=10>`_

Resulting in:

.. image:: setMarginleft.png

Values must be floating point or integer values greater than 0.

Right margin
^^^^^^^^^^^^

The following call sets the right margin of the page:

.. parsed-literal::
  `http://faust.grame.fr:8000/?marginright=10 <http://faust.grame.fr:8000/?marginright=10>`_

Resulting in:

.. image:: setMarginright.png

Values must be floating point or integer values greater than 0.

Top margin
^^^^^^^^^^

The following call sets the top margin of the page:

.. parsed-literal::
  `http://faust.grame.fr:8000/?margintop=10 <http://faust.grame.fr:8000/?margintop=10>`_

Resulting in:

.. image:: setMargintop.png

Values must be floating point or integer values greater than 0.

Bottom margin
^^^^^^^^^^^^^

The following call sets the bottom margin of the page:

.. parsed-literal::
  `http://faust.grame.fr:8000/?marginbottom=10 <http://faust.grame.fr:8000/?marginbottom=10>`_

Resulting in:

.. image:: setMarginbottom.png

Values must be floating point or integer values greater than 0.

Zoom
^^^^

The following call sets the zoom of the page:

.. parsed-literal::
  `http://faust.grame.fr:8000/?zoom=4 <http://faust.grame.fr:8000/?zoom=4>`_

Resulting in:

.. image:: setZoom.png

Values must be floating point or integer values greater than 0.

Resize page to music
^^^^^^^^^^^^^^^^^^^^

The following call tells the web server to resize the page to the music it
contains:

.. parsed-literal::
  `http://faust.grame.fr:8000/?resizepagetomusic=true <http://faust.grame.fr:8000/?resizepagetomusic=true>`_

Resulting in:

.. image:: setResizepagetomusic.png

Values must be either ``true`` or ``false``.

Format
^^^^^^^^^^^^^^^^^^^^

The following call tells the web server change the format of the output

.. parsed-literal::
  `http://faust.grame.fr:8000/?format=jpg <http://faust.grame.fr:8000/?format=jpg>`_

Resulting in:

.. image:: setFormat.jpg

Values must be either ``jpg``, ``gif`` or ``png``.

