def _reset_sys_path():
    # Clear generic sys.path[0]
    import sys, os
    resources = os.environ['RESOURCEPATH']
    while sys.path[0] == resources:
        del sys.path[0]
_reset_sys_path()


def _chdir_resource():
    import os
    os.chdir(os.environ['RESOURCEPATH'])
_chdir_resource()


def _disable_linecache():
    import linecache
    def fake_getline(*args, **kwargs):
        return ''
    linecache.orig_getline = linecache.getline
    linecache.getline = fake_getline
_disable_linecache()


def _run():
    global __file__
    import os, sys, site
    sys.frozen = 'macosx_app'
    base = os.environ['RESOURCEPATH']

    argv0 = os.path.basename(os.environ['ARGVZERO'])
    script = SCRIPT_MAP.get(argv0, DEFAULT_SCRIPT)

    path = os.path.join(base, script)
    sys.argv[0] = __file__ = path
    with open(path, 'rU') as fp:
        source = fp.read() + "\n"
    exec(compile(source, path, 'exec'), globals(), globals())


DEFAULT_SCRIPT='simple_gui.py'
SCRIPT_MAP={}
_run()
