"""Given a tree of nodes, render into a string."""

from pyhaml_jinja.parser import Parser


class Renderer(object):
  """Uses a Parser to build a tree, and then properly renders it."""

  def __init__(self, source, newline_string=None, indent_string=None):
    self.parser = Parser(source)
    self.newline_string = newline_string or ''
    self.indent_string = indent_string or ''

  def render(self):
    """Renders the current source tree into an HTML string."""

    # Since the root node has no indentation, kick off the indentation level
    # at -1.
    lines = self.parser.tree.render_lines(indent_string=self.indent_string,
                                          indent_level=-1)
    return self.newline_string.join(lines)


def render(source, newline_string='\n', indent_string='  '):
  return Renderer(source, newline_string, indent_string).render()

