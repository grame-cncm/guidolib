The POST method
===============

| The POST method is used to create or to modify scores.
| The HTTP request entity-body is expected to contain **valid GMN code** that is **URL encoded**. Many URL transfer tools do this URL encoding automatically for certain POST methods.  When in doubt, use a URL encoding method like, for example, the function ``urlencode`` in PHP.
| You have two differents manners to create score. You can send totality of your score or send your score as a stream.

.. toctree::
   :maxdepth: 1

   postscore
   poststream

