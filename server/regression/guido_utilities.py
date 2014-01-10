def my_url_translation_function(url) :
  naughty = "?/&"
  #naughty = "#%&{}\\<>*?/ $!'\":@+`|="
  for x in naughty :
    url = url.replace(x,"_")
  return url