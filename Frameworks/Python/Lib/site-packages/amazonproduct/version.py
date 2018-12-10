
import os.path

# The version is defined in its own module so we can import it from setup.py
# without introducing unwanted dependencies at that stage.
VERSION = '0.2.8'

# The following code is borrowed from Sphinx
# https://bitbucket.org/birkenfeld/sphinx/
package_dir = os.path.abspath(os.path.dirname(__file__))

if '+' in VERSION or 'pre' in VERSION:
    # try to find out the changeset hash if checked out from hg, and append
    # it to VERSION (since we use this value from setup.py, it gets
    # automatically propagated to an installed copy as well)
    try:
        import subprocess
        p = subprocess.Popen(
            ['hg', 'id', '-i', '-R', os.path.join(package_dir, '..')],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
        if out:
            VERSION += '/' + out.strip()
    except Exception, e:
        pass

