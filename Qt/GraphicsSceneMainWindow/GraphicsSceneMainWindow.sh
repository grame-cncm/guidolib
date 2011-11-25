appname=`basename $0 | sed s,\.sh$,,`
dirname=`dirname $0`
if [ "${dirname:0:1}" != "/" ]; then
  dirname=$PWD/$dirname
fi
LD_LIBRARY_PATH=$dirname
export LD_LIBRARY_PATH
$dirname/$appname $*