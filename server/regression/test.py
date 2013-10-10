import urllib
import urllib2
import json
import xml.etree.ElementTree as ET

URL = 'http://guido.grame.fr:8000/'
#URL = 'http://localhost:8000/'

def guidourl(end='', url=URL) :
  return url+end

def gulp(fn) :
  infile = file(fn, 'r')
  s = infile.read()
  return s

def equality_test(t, a, b) :
  if a != b :
    msg = 'The following two {0} should be equal and are not:\n{1}\nand\n{2}\n'.format(t, a, b)
    raise ValueError(msg)

def json_equality_test(a, b) :
  equality_test('json objects', a, b)

def status_code_equality_test(a, b) :
  equality_test('status codes', a, b)

def test_400(url, code) :
  try :
    test = urllib2.urlopen(url)
    msg = "Should have raised error {0}, not {1}".format(code, test.getcode())
    raise ValueError(msg)
  except urllib2.HTTPError as e:
    if not (str(code) in str(e)) :
      msg = "Expecting error {0}, got: {1}".format(code, str(e))
      raise ValueError(msg)
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

"""
Test: post from a string
"""

TEST = urllib2.urlopen(guidourl(), 'data={0}'.format(urllib.quote_plus('[e f g a]')))
RESULT_RAW = TEST.read()
EXPECTED = { "ID" : "917a8f70ccb5388e2f79ea0fd61cd899eb30f4b2"}
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 201)

"""
Test: post from a file
"""

TEST = urllib2.urlopen(guidourl(), 'data={0}'.format(urllib.quote_plus(gulp('test.gmn'))))
RESULT_RAW = TEST.read()
EXPECTED = { "ID" : "29ba36a3f7d3cf1ca008948d4707d9a8470b335c"}
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 201)

"""
Test: referencing inexistant score
"""

test_400(guidourl('enemble101'), 404)

"""
Test: SvG
currently impossible beacuse diffs are different all the time, probably
due to sorting of pointers
"""
'''
TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/?format=svg'))
RESULT_RAW = TEST.read().replace('\n','')
EXPECTED_F = file('test.svg', 'r')
EXPECTED = EXPECTED_F.read().replace('\n','')
EXPECTED_F.close()
equality_test('svg documents', RESULT_RAW, EXPECTED)
status_code_equality_test(TEST.getcode(), 201)
'''

"""
Test: voicescount
"""

TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/voicescount'))
RESULT_RAW = TEST.read()
EXPECTED = { "29ba36a3f7d3cf1ca008948d4707d9a8470b335c" : {"voicescount" : 3 } }
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: pagescount
"""

TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pagescount'))
RESULT_RAW = TEST.read()
EXPECTED = { "29ba36a3f7d3cf1ca008948d4707d9a8470b335c" : {"pagescount" : 1 } }
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: duration
"""

TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/duration'))
RESULT_RAW = TEST.read()
EXPECTED = { "29ba36a3f7d3cf1ca008948d4707d9a8470b335c" : {"duration" : '"1/1"' } }
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: pageat
"""

TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pageat?date="3/4"'))
RESULT_RAW = TEST.read()
EXPECTED = { "29ba36a3f7d3cf1ca008948d4707d9a8470b335c" : {"date" : '"3/4"', "page" : 1 } }
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: date out of bounds
"""

test_400(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pageat?date="42/4"'), 400)

"""
Test: pagedate
"""

TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pagedate?page=1'))
RESULT_RAW = TEST.read()
EXPECTED = { "29ba36a3f7d3cf1ca008948d4707d9a8470b335c" : {"date" : '"0/1"', "page" : 1 } }
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: page out of bounds
"""

test_400(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pagedate?page=42'), 400)

"""
Test: staffmap
"""

TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/staffmap?staff=1'))
RESULT_RAW = TEST.read()
EXPECTED = {
	"29ba36a3f7d3cf1ca008948d4707d9a8470b335c": {
		"staffmap": [
			{
				"graph": {
					"left": 546.244,
					"top": 74.4057,
					"right": 861.053,
					"bottom": 272.821
				},
				"time": {
					"start": "\"0/1\"",
					"end": "\"1/4\""
				}
			},
			{
				"graph": {
					"left": 861.053,
					"top": 74.4057,
					"right": 1178.14,
					"bottom": 272.821
				},
				"time": {
					"start": "\"1/4\"",
					"end": "\"1/2\""
				}
			},
			{
				"graph": {
					"left": 1178.14,
					"top": 74.4057,
					"right": 1336.69,
					"bottom": 272.821
				},
				"time": {
					"start": "\"1/2\"",
					"end": "\"5/8\""
				}
			},
			{
				"graph": {
					"left": 1336.69,
					"top": 74.4057,
					"right": 1495.24,
					"bottom": 272.821
				},
				"time": {
					"start": "\"5/8\"",
					"end": "\"3/4\""
				}
			},
			{
				"graph": {
					"left": 1495.24,
					"top": 74.4057,
					"right": 1761.96,
					"bottom": 272.821
				},
				"time": {
					"start": "\"3/4\"",
					"end": "\"1/1\""
				}
			}
		]
	}
}
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: pagemap
"""

TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/pagemap'))
RESULT_RAW = TEST.read()
EXPECTED = {
	"29ba36a3f7d3cf1ca008948d4707d9a8470b335c": {
		"pagemap": [
			{
				"graph": {
					"left": 235.228,
					"top": 0,
					"right": 1761.96,
					"bottom": 1185.53
				},
				"time": {
					"start": "\"0/1\"",
					"end": "\"1/1\""
				}
			}
		]
	}
}
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: systemmap
"""

TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/systemmap'))
RESULT_RAW = TEST.read()
EXPECTED = {
	"29ba36a3f7d3cf1ca008948d4707d9a8470b335c": {
		"systemmap": [
			{
				"graph": {
					"left": 546.244,
					"top": 0,
					"right": 745.03,
					"bottom": 1185.53
				},
				"time": {
					"start": "\"0/1\"",
					"end": "\"3/16\""
				}
			},
			{
				"graph": {
					"left": 745.03,
					"top": 0,
					"right": 861.053,
					"bottom": 1185.53
				},
				"time": {
					"start": "\"3/16\"",
					"end": "\"1/4\""
				}
			},
			{
				"graph": {
					"left": 861.053,
					"top": 0,
					"right": 1019.6,
					"bottom": 1185.53
				},
				"time": {
					"start": "\"1/4\"",
					"end": "\"3/8\""
				}
			},
			{
				"graph": {
					"left": 1019.6,
					"top": 0,
					"right": 1178.14,
					"bottom": 1185.53
				},
				"time": {
					"start": "\"3/8\"",
					"end": "\"1/2\""
				}
			},
			{
				"graph": {
					"left": 1178.14,
					"top": 0,
					"right": 1336.69,
					"bottom": 1185.53
				},
				"time": {
					"start": "\"1/2\"",
					"end": "\"5/8\""
				}
			},
			{
				"graph": {
					"left": 1336.69,
					"top": 0,
					"right": 1495.24,
					"bottom": 1185.53
				},
				"time": {
					"start": "\"5/8\"",
					"end": "\"3/4\""
				}
			},
			{
				"graph": {
					"left": 1495.24,
					"top": 0,
					"right": 1761.96,
					"bottom": 1185.53
				},
				"time": {
					"start": "\"3/4\"",
					"end": "\"1/1\""
				}
			}
		]
	}
}
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: voicemap
"""

TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/voicemap'))
RESULT_RAW = TEST.read()
EXPECTED = {
	"29ba36a3f7d3cf1ca008948d4707d9a8470b335c": {
		"voicemap": [
			{
				"graph": {
					"left": 546.244,
					"top": 74.4057,
					"right": 861.053,
					"bottom": 272.821
				},
				"time": {
					"start": "\"0/1\"",
					"end": "\"1/4\""
				}
			},
			{
				"graph": {
					"left": 861.053,
					"top": 74.4057,
					"right": 1178.14,
					"bottom": 272.821
				},
				"time": {
					"start": "\"1/4\"",
					"end": "\"1/2\""
				}
			},
			{
				"graph": {
					"left": 1178.14,
					"top": 74.4057,
					"right": 1336.69,
					"bottom": 272.821
				},
				"time": {
					"start": "\"1/2\"",
					"end": "\"5/8\""
				}
			},
			{
				"graph": {
					"left": 1336.69,
					"top": 74.4057,
					"right": 1495.24,
					"bottom": 272.821
				},
				"time": {
					"start": "\"5/8\"",
					"end": "\"3/4\""
				}
			},
			{
				"graph": {
					"left": 1495.24,
					"top": 74.4057,
					"right": 1761.96,
					"bottom": 272.821
				},
				"time": {
					"start": "\"3/4\"",
					"end": "\"1/1\""
				}
			}
		]
	}
}
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: timemap
"""

TEST = urllib2.urlopen(guidourl('29ba36a3f7d3cf1ca008948d4707d9a8470b335c/timemap'))
RESULT_RAW = TEST.read()
EXPECTED = {
	"29ba36a3f7d3cf1ca008948d4707d9a8470b335c": {
		"timemap": [
			{
				"score": {
					"start": "\"0/1\"",
					"end": "\"1/4\""
				},
				"perf": {
					"start": "\"0/1\"",
					"end": "\"1/4\""
				}
			},
			{
				"score": {
					"start": "\"1/4\"",
					"end": "\"1/2\""
				},
				"perf": {
					"start": "\"1/4\"",
					"end": "\"1/2\""
				}
			},
			{
				"score": {
					"start": "\"1/2\"",
					"end": "\"5/8\""
				},
				"perf": {
					"start": "\"1/2\"",
					"end": "\"5/8\""
				}
			},
			{
				"score": {
					"start": "\"5/8\"",
					"end": "\"3/4\""
				},
				"perf": {
					"start": "\"5/8\"",
					"end": "\"3/4\""
				}
			},
			{
				"score": {
					"start": "\"3/4\"",
					"end": "\"1/1\""
				},
				"perf": {
					"start": "\"3/4\"",
					"end": "\"1/1\""
				}
			}
		]
	}
}
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: version
"""

TEST = urllib2.urlopen(guidourl('version'))
RESULT_RAW = TEST.read()
EXPECTED = {
	"version": "1.5.2"
}
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: server version
"""

TEST = urllib2.urlopen(guidourl('server'))
RESULT_RAW = TEST.read()
EXPECTED = {
	"server": "0.50"
}
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

"""
Test: linespace
"""

TEST = urllib2.urlopen(guidourl('linespace'))
RESULT_RAW = TEST.read()
EXPECTED = {
	"linespace": 50
}
RESULT_JSON = json.loads(RESULT_RAW)
json_equality_test(EXPECTED, RESULT_JSON)
status_code_equality_test(TEST.getcode(), 200)

# done
print "All regtests check clean."
