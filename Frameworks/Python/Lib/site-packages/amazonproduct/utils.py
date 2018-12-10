# Copyright (C) 2009-2013 Sebastian Rahlf <basti at redtoad dot de>
#
# This program is release under the BSD License. You can find the full text of
# the license in the LICENSE file.

from ConfigParser import SafeConfigParser
import os
import sys

REQUIRED_KEYS = [
    'access_key',
    'secret_key',
    'associate_tag',
    'locale',
]

CONFIG_FILES = [
    '/etc/amazon-product-api.cfg',
    '~/.amazon-product-api'
]


def load_file_config(path=None):
    """
    Loads configuration from file with following content::

        [Credentials]
        access_key = <your access key>
        secret_key = <your secret key>
        associate_tag = <your associate tag>
        locale = us

    :param path: path to config file. If not specified, locations
    ``/etc/amazon-product-api.cfg`` and ``~/.amazon-product-api`` are tried.
    """
    config = SafeConfigParser()
    if path is None:
        config.read([os.path.expanduser(path) for path in CONFIG_FILES])
    else:
        config.read(path)

    if not config.has_section('Credentials'):
        return {}

    return dict(
        (key, val)
        for key, val in config.items('Credentials')
        if key in REQUIRED_KEYS
    )


def load_environment_config():
    """
    Loads config dict from environmental variables (if set):

    * AWS_ACCESS_KEY
    * AWS_SECRET_ACCESS_KEY
    * AWS_ASSOCIATE_TAG
    * AWS_LOCALE
    """
    mapper = {
        'access_key': 'AWS_ACCESS_KEY',
        'secret_key': 'AWS_SECRET_ACCESS_KEY',
        'associate_tag': 'AWS_ASSOCIATE_TAG',
        'locale': 'AWS_LOCALE',
    }
    return dict(
        (key, os.environ.get(val))
        for key, val in mapper.items()
        if val in os.environ
    )


def load_config(path=None):
    """
    Returns a dict with API credentials which is loaded from (in this order):

    * Environment variables ``AWS_ACCESS_KEY``, ``AWS_SECRET_ACCESS_KEY``,
      ``AWS_ASSOCIATE_TAG`` and ``AWS_LOCALE``
    * Config files ``/etc/amazon-product-api.cfg`` or ``~/.amazon-product-api``
      where the latter may add or replace values of the former.

    Whatever is found first counts.

    The returned dictionary may look like this::

        {
            'access_key': '<access key>',
            'secret_key': '<secret key>',
            'associate_tag': 'redtoad-10',
            'locale': 'uk'
        }

    :param path: path to config file.
    """
    config = load_file_config(path)
    config.update(load_environment_config())

    # substitute None for all values not found
    for key in REQUIRED_KEYS:
        if key not in config:
            config[key] = None

    return config


def import_module(name, package=None):
    """Import a module.

    The 'package' argument is required when performing a relative import. It
    specifies the package to use as the anchor point from which to resolve the
    relative import to an absolute import.

    Taken from Django's importlib module
    https://code.djangoproject.com/browser/django/trunk/django/utils/importlib.py
    """
    def _resolve_name(name, package, level):
        """Return the absolute name of the module to be imported."""
        if not hasattr(package, 'rindex'):
            raise ValueError("'package' not set to a string")
        dot = len(package)
        for x in xrange(level, 1, -1):
            try:
                dot = package.rindex('.', 0, dot)
            except ValueError:
                raise ValueError("attempted relative import beyond top-level "
                                  "package")
        return "%s.%s" % (package[:dot], name)

    if name.startswith('.'):
        if not package:
            raise TypeError("relative imports require the 'package' argument")
        level = 0
        for character in name:
            if character != '.':
                break
            level += 1
        name = _resolve_name(name[level:], package, level)
    __import__(name)
    return sys.modules[name]


def running_on_gae():
    """
    Is this module running on Google App Engine (GAE)?
    """
    return 'Google' in os.environ.get('SERVER_SOFTWARE', '')


def load_class(name):
    """
    Loads class from string.

    :param name: fully-qualified class name (e.g. ``processors.etree.
      ItemPaginator``)
    """
    module_name, class_name = name.rsplit('.', 1)
    module = import_module(module_name)
    return getattr(module, class_name)
