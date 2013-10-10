import urllib
import urllib2
import json
import xml.etree.ElementTree as ET
import cPickle
import sys

URL = 'http://guido.grame.fr:8000/'
#URL = 'http://localhost:8000/'

RES = 0
CODE = 1

INSTR = "Regtest script must be called with an argument of either \"baseline\" or \"check\""

if len(sys.argv) != 2 :
  print INSTR
  sys.exit(1)

BASELINE = True
RESULTS = {}
if sys.argv[1] == 'baseline' :
  pass
elif sys.argv[1] == 'check' :
  BASELINE = False
else :
  print INSTR
  sys.exit(1)

if not BASELINE :
  output = open('test.pkl','rb')
  RESULTS = cPickle.load(output)
  output.close()
    
def guidourl(end='', url=URL) :
  return url+end

def gulp(fn) :
  infile = file(fn, 'r')
  s = infile.read()
  return s

def equality_test(t, a, b) :
  if a != b :
    msg = 'The following two {0} should be equal and are not:\n{1}\nand\n{2}\n'.format(t, a, b)
    print #ValueError(msg)

def json_equality_test(a, b) :
  equality_test('json objects', a, b)

def status_code_equality_test(a, b) :
  equality_test('status codes', a, b)

def test_400(url, code) :
  try :
    test = urllib2.urlopen(url)
    msg = "Should have raised error {0}, not {1}".format(code, test.getcode())
    #raise ValueError(msg)
    print msg
  except urllib2.HTTPError as e:
    if not (str(code) in str(e)) :
      msg = "Expecting error {0}, got: {1}".format(code, str(e))
      #raise ValueError(msg)
      print msg
    return

# from http://stackoverflow.com/questions/3007330/xml-comparison-in-python
# currently not useable as the guido svgs are not well formed xml
def xml_equality_test(xml_string1, xml_string2) :
  xml1 = ET.fromstring(xml_string1)
  xml2 = ET.fromstring(xml_string2)

  xml_string_io1 = StringIO()
  xml1.getroottree().write_c14n(xml_string_io1)
  cxml1 = xml_string_io1.getvalue()

  xml_string_io2 = StringIO()
  xml2.getroottree().write_c14n(xml_string_io2)
  cxml2 = xml_string_io2.getvalue()
  equality_test('SVG documents', a, b)

def json_test(url, testname, data=None) :
  test = urllib2.urlopen(url, data)
  result_raw = test.read()
  result_json = json.loads(result_raw)
  if BASELINE :
    return [result_json, test.getcode()]
  else :
    json_equality_test(RESULTS[testname][RES], result_json)
    status_code_equality_test(test.getcode(), RESULTS[testname][CODE])
    return [result_json, test.getcode()]

"""
Test: post from a string
"""

RESULTS["poststr"] = json_test(guidourl(), "poststr", 'data={0}'.format(urllib.quote_plus('[e f g a]')))


"""
Test: post from a file
"""

RESULTS["postfile"] = json_test(guidourl(), "postfile", 'data={0}'.format(urllib.quote_plus(gulp('test.gmn'))))

"""
Test: referencing inexistant score
"""

test_400(guidourl('enemble101'), 404)

"""
Test: SvG
currently impossible beacuse diffs are different all the time, probably
due to sorting of pointers
"""

"""
Test: voicescount
"""
RESULTS["voicescount"] = json_test(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/voicescount'), "voicescount")

"""
Test: pagescount
"""
RESULTS["pagescount"] = json_test(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pagescount'), "pagescount")

"""
Test: duration
"""
RESULTS["duration"] = json_test(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/duration'), "duration")

"""
Test: pageat
"""
RESULTS["pageat"] = json_test(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pageat?date="3/4"'), "pageat")

"""
Test: date out of bounds
"""

test_400(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pageat?date="42/4"'), 400)

"""
Test: pagedate
"""
RESULTS["pagedate"] = json_test(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pagedate?page=1'), "pagedate")

"""
Test: page out of bounds
"""

test_400(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pagedate?page=42'), 400)

"""
Test: staffmap
"""
RESULTS["staffmap"] = json_test(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/staffmap?staff=1'), "staffmap")

"""
Test: pagemap
"""
RESULTS["pagemap"] = json_test(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pagemap'), "pagemap")

"""
Test: systemmap
"""
RESULTS["systemmap"] = json_test(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/systemmap'), "systemmap")

"""
Test: voicemap
"""
RESULTS["voicemap"] = json_test(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/voicemap'), "voicemap")

"""
Test: timemap
"""
RESULTS["timemap"] = json_test(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/timemap'), "timemap")

"""
Test: version
"""
RESULTS["version"] = json_test(guidourl('version'), "version")

"""
Test: server version
"""
RESULTS["server"] = json_test(guidourl('server'), "server")

"""
Test: linespace
"""
RESULTS["linespace"] = json_test(guidourl('linespace'), "linespace")

if BASELINE :
  output = open('test.pkl','wb')
  cPickle.dump(RESULTS, output)
  output.close()
  print "REGTEST BASELINE COMPLETED"
else :
  print "REGTEST CHECK COMPLETED"