import urllib
import urllib2
import json
import sys
import urlparse
import os
import difflib
import shutil
import hashlib
import guido_utilities
from optparse import OptionParser
from filecmp import dircmp

'''
BELOW YOU CAN SET CERTAIN SCRIPT VARIABLES
THAT CANNOT BE SET FROM THE COMMAND LINE.

IN THEORY, THESE ARE THE ONLY VARIABLES THAT SHOULD
EVER NEED TO CHANGE TO MAKE THIS WORK.
'''

# The directory holding the GUIDO examples
EXAMPLE_DIR = '../../gmn-examples'

# THE EXTENSION OF A GMN FILE FOUND IN THIS DIRECTORY
# OR ITS SUB-DIRECTORIES
GMN_FILE_EXTENSION = 'gmn'

# PATH TO A LIST OF URLS THAT ARE TESTED FOR SCORES
SCORE_URLS_PATH = 'score_urls.txt'

# PATH TO A LIST OF URLS THAT ARE TESTED FOR THE SERVER
SERVER_URLS_PATH = 'server_urls.txt'

# FUNCTION FOR CREATING FILE-SYSTEM FRIENDLY URLS
#URL_TRANLSATION_FUNCTION = hashlib.sha1
URL_TRANSLATION_FUNCTION = guido_utilities.my_url_translation_function

####################################################


parser = OptionParser(usage = "Runs regtests on the guido server.\n"
"Must be invoked with either the command `baseline' or `check'. Meaning...\n"
"  python test.py baseline\n"
"or\n"
"  python test.py check\n"
"More detailed information should be in the README that came with this code.")
parser.add_option("-u", "--url", dest="url", help="url of the server [default: %default]", default = "http://localhost:8000")
parser.add_option("-b", "--baselinedir", dest="baselinedir", help="directory to stash the baseline results in [default: %default]", default = "baseline")
parser.add_option("-c", "--checkdir", dest="checkdir", help="directory to stash the check results in [default: %default]", default = "check")
parser.add_option("-v", "--verbose", dest="verbose", help="show all regtest results at the end", action = "store_true", default = False)
parser.add_option("-l", "--log", dest="log", help="Filename to write log to. If not specified, log will be printed to STDOUT.", default = None)

(OPTIONS, ARGS) = parser.parse_args()

MSG = '''You must call this script as either:
  python test.py baseline
or
  python test.py check
There are additional options that you can specify,
but at a minimum, you need one of these two commands.
To learn more about these options, run the command:
  python test.py -h
'''

if len(ARGS) != 1 :
  print MSG
  sys.exit(1)

if ARGS[0] not in ['baseline', 'check'] :
  print MSG
  sys.exit(1)

BASELINE = {'baseline':True,'check':False}[ARGS[0]]

URL = OPTIONS.url

MYDIR = OPTIONS.baselinedir if BASELINE else OPTIONS.checkdir

print "Welcome to the GUIDO server regtests. Run this script with -h for help."
print "Running regtest in {0} mode.".format("baseline" if BASELINE else "check")
print "  If you meant to run this test in {0} mode instead,".format("baseline" if not BASELINE else "check")
print "  please run this script with the word `{0}' afterwards".format("baseline" if not BASELINE else "check")
print "  (i.e. python test.py {0}).".format("baseline" if not BASELINE else "check")
print " "

if (not BASELINE) and (not os.path.exists(OPTIONS.baselinedir)) :
  print "Cowardly refusing to check the regtests without a baseline."
  print "Run:"
  print "  python test.py baseline"
  print "first."
  sys.exit(1)

###########
## TESTS ##
###########

# checking to see if directories exist

if (not os.path.exists(MYDIR)) :
  os.makedirs(MYDIR)
else :
  print "Cowardly refusing to overwrite the directory `{0}'.".format(MYDIR)
  print "There are two options:"
  print "  1) Stash this directory somewhere and then run this script again."
  print "  2) Specify a different directory for the regtest {0}.  This can be done with the {1} option.".format('baseline' if BASELINE else 'check', '-b' if BASELINE else '-c')
  print " "
  print "For a full list of options, run this script with the flag -h."
  sys.exit(1)

if not BASELINE :
  if (OPTIONS.log) :
    if os.path.exists(OPTIONS.log) :
      print "Cowardly refusing to do the check because the logfile {0} exists already.".format(OPTIONS.log)
      print "Please stash it somewhere and run again."
      sys.exit(1)

try :
  urllib2.urlopen(URL, 'data={0}'.format(urllib.quote_plus('[e f g a]')))
except urllib2.URLError :
  print "Could not compile a simple test on the server with url {0}. Run this script with the -h flag to see how to specify a URL of the server.".format(URL)
  sys.exit(1)

def guidourl(end='', url=URL) :
  return urlparse.urljoin(url, end)

def gulp(fn) :
  infile = file(fn, 'r')
  s = infile.read()
  return s

def my_urlopen(url, data=None) :
  res = None
  try :
    res = urllib2.urlopen(url, data)
  except urllib2.HTTPError as e :
    res = e
  return res

def get_extension(tp) :
  if tp == 'image/xml+svg' :
    return 'svg'
  if tp == 'image/png' :
    return 'png'
  if tp == 'image/gif' :
    return 'gif'
  if tp == 'image/jpeg' :
    return 'jpg'
  if tp == 'application/json' :
    return 'json'
  raise ValueError('Server is not supposed to return {0}.  This is bad.'.format(tp))

SCORE_URLS = gulp(SCORE_URLS_PATH).split('\n')
SERVER_URLS = gulp(SERVER_URLS_PATH).split('\n')
SCORE_URLHEXS = [URL_TRANSLATION_FUNCTION(URL) for URL in SCORE_URLS]
SERVER_URLHEXS = [URL_TRANSLATION_FUNCTION(URL) for URL in SERVER_URLS]

TESTTREE = os.walk(EXAMPLE_DIR)
TESTFILES = []
for entry in TESTTREE :
  for filename in entry[2] :
    if filename.split('.')[-1] == GMN_FILE_EXTENSION :
      TESTFILES.append(os.path.join(entry[0], filename))

class Result(object) :
  def __init__(self, url, tp, code, data) :
    self.url = url
    self.type = tp
    self.code = code
    self.data = data

PATH_TO_URL = {}

# SERVER TEST
for i in range(len(SERVER_URLS)) :
  URL = SERVER_URLS[i]
  HEX = SERVER_URLHEXS[i]
  localpath = os.path.join(MYDIR, 'server-test', HEX)
  os.makedirs(localpath)
  myurl = URL
  PATH_TO_URL[localpath] = myurl
  if OPTIONS.verbose :
    print "CHECKING", guidourl(myurl)
  res = my_urlopen(guidourl(myurl))
  kres = Result(myurl, res.info().gettype(), res.getcode(), res.read())
  res.close()
  ext = get_extension(kres.type)
  # write the info to files in the file tree
  for key in kres.__dict__.keys() :
    F = file(os.path.join(localpath, HEX+'.'+key), 'w')
    if OPTIONS.verbose :
      print key, str(getattr(kres, key))
    F.write(str(getattr(kres, key)))
    F.close()

# SCORE TEST
for FILE in TESTFILES :
  naked = '.'.join(FILE.replace('../../','').split('.')[:-1])
  mypath = os.path.join(MYDIR, naked)
  os.makedirs(mypath)
  shutil.copy(FILE, mypath)
  '''
  for the initial test, we assume that the mime type is JSON and we get
  an identifier.
  a lack of identifier means that the GMN is bad or the server is broken.
  '''
  res = my_urlopen(guidourl(),
                   'data={0}'.format(urllib.quote_plus(gulp(FILE))))
  SHA = ''
  try :
    SHA = str(json.loads(res.read())['ID'])
  except :
    continue 
  # we loop through the tests, running them and writing info
  BASE = os.path.split(naked)[1]
  for i in range(len(SCORE_URLS)) :
    URL = SCORE_URLS[i]
    HEX = SCORE_URLHEXS[i]
    localpath = os.path.join(mypath, HEX)
    os.makedirs(localpath)
    myurl = SHA + '/' + URL
    PATH_TO_URL[localpath] = myurl
    if OPTIONS.verbose :
      print "CHECKING", guidourl(myurl)
    res = my_urlopen(guidourl(myurl))
    kres = Result(myurl, res.info().gettype(), res.getcode(), res.read())
    res.close()
    ext = get_extension(kres.type)
    # write the info to files in the file tree
    for key in kres.__dict__.keys() :
      F = file(os.path.join(localpath, BASE+'_'+HEX+'.'+key), 'w')
      if OPTIONS.verbose :
        print key, str(getattr(kres, key))
      F.write(str(getattr(kres, key)))
      F.close()


def identical_dirs(dcmp):
  if (dcmp.left_only + dcmp.right_only) != [] :
    return False
  for sub_dcmp in dcmp.subdirs.values():
    if not identical_dirs(sub_dcmp) :
      return False
  return True

def make_report(dcmp, baselinedir, checkdir, currentdir = '') :
  UD = ''
  for FILE in dcmp.diff_files :
    old_fn = os.path.join(baselinedir, currentdir, FILE)
    new_fn = os.path.join(checkdir, currentdir, FILE)
    maybehex = currentdir.rpartition('/')[-1]
    # ugh .. code dup
    if maybehex in (SCORE_URLHEXS+SERVER_URLHEXS) :
      UD += 'URL sent to the server: {0}\n'.format(PATH_TO_URL[os.path.join(checkdir,currentdir)])
    UD += ''.join([line+'\n' for line in difflib.unified_diff(gulp(old_fn).split('\n'), gulp(new_fn).split('\n'), old_fn, new_fn)])
  for key in dcmp.subdirs.keys():
    UD += make_report(dcmp.subdirs[key], baselinedir, checkdir, os.path.join(currentdir, key))
  return UD

if not BASELINE :
  dcmp = dircmp(OPTIONS.baselinedir, OPTIONS.checkdir)
  if not identical_dirs(dcmp) :
    print "WARNING: The structure of your baseline directory (called `{0}') differs from that of your check (called `{1}') directory.".format(OPTIONS.baselinedir, OPTIONS.checkdir)
    print "Files that are present in one but not the other will not be checked against each other."
  report = make_report(dcmp, OPTIONS.baselinedir, OPTIONS.checkdir)
  if report != "" :
    report = "Below are diffs between the baseline and the check.\n" + report
  else :
    report = "There were no differences between the baseline and the check."
  if not OPTIONS.log :
    print report
  else :
    F = file(OPTIONS.log, 'w')
    F.write(report)
    F.close()
    print "Log written to `{0}'.".format(OPTIONS.log)

if BASELINE :
  print "REGTEST BASELINE COMPLETED"
else :
  print "REGTEST CHECK COMPLETED"
