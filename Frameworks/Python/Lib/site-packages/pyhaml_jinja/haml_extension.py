"""Extension for use with Jinja2."""

import os.path

from jinja2 import TemplateSyntaxError
from jinja2.ext import Extension

from pyhaml_jinja.renderer import Renderer


class HamlExtension(Extension):
  """Implementation of HAML pre-processing extension."""

  FILE_EXTENSIONS = ('.haml', )
  DEFAULT_INDENT_STRING = '  '
  DEFAULT_NEWLINE_STRING = '\n'

  def __init__(self, environment):
    """Configures the extension and environment."""

    super(HamlExtension, self).__init__(environment)

    environment.extend(haml_file_extensions=self.FILE_EXTENSIONS,
        haml_indent_string=self.DEFAULT_INDENT_STRING,
        haml_newline_string=self.DEFAULT_NEWLINE_STRING)

  def preprocess(self, source, name, filename=None):
    """Preprocesses the template from HAML to Jinja-style HTML."""

    if (not name or os.path.splitext(name)[1] not in
        self.environment.haml_file_extensions):
      return source

    try:
      renderer = Renderer(source,
          indent_string=self.environment.haml_indent_string,
          newline_string=self.environment.haml_newline_string)
    except TemplateSyntaxError, e:
      raise TemplateSyntaxError(e.message, e.lineno, name=name, filename=filename)

    return renderer.render()

