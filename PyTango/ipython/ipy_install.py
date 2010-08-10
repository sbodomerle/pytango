#!/usr/bin/env python

import sys
import os
import StringIO

import IPython.genutils
import PyTango

__PROFILE = """\
#!/usr/bin/env python
\"\"\"An automaticaly generated IPython profile designed to provide a user 
friendly interface to Tango.
Created with PyTango {pytangover} for IPython {ipyver}\"\"\"

import IPython
import PyTango.ipython

ip = IPython.ipapi.get()
PyTango.ipython.init_ipython(ip)

"""

def install(ipydir=None,verbose=True):
    install_dir = ipydir or IPython.genutils.get_ipython_dir()
    f_name = os.path.join(install_dir, 'ipy_profile_spock.py')
    if verbose:
        out = sys.stdout
    else:
        out = StringIO.StringIO()
    if ipydir is None and os.path.isfile(f_name):
        print "Warning: The file '%s' already exists." % f_name
        r = ''
        while r.lower() not in ('y', 'n'):
            r = raw_input("Do you wish to override it [Y/n]?")
            r = r or 'y'
        if r.lower() == 'n':
            return
    profile = __PROFILE.format(pytangover=PyTango.Release.version, ipyver=IPython.Release.version)
    
    out.write("Installing spock extension to ipython... ")
    out.flush()
    try:
        f = file(f_name, "w")
        f.write(profile)
        f.close()
        out.write("[DONE]\n\n")
    except Exception as e:
        out.write("[FAILED]\n\n")
        raise e
    
    ipy_user_config = os.path.join(IPython.genutils.get_ipython_dir(), 'ipy_user_conf.py')
    out.write("""\
To start spock simply type on the command line:
%% ipython -p spock

If you want spock extension to be automaticaly active when you start ipython,
edit your %s and add the line:
import ipy_profile_spock

Next time, just start ipython on the command line:
%% ipython

and your spock extension should be loaded automaticaly. Note that if you are also
loading other extensions that, for example, overwrite the prompt, the prompt
that will appear is the one from the last extension to be imported.

For more information goto: http://www.tango-controls.org/static/PyTango/latest/doc/html/index.html

Have fun with spock!
The PyTango team
    """ % (ipy_user_config,))

def main():
    d = None
    if len(sys.argv) > 1:
        d = sys.argv[1]
    install(d)
    
if __name__ == "__main__":
    main()
    