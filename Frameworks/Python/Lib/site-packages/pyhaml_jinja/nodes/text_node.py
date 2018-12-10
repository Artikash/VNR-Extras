"""Represents a line of text."""

from pyhaml_jinja.nodes.node import Node


__all__ = ['TextNode', 'PreformattedTextNode']


class TextNode(Node):
  """Represents a node that contains text only.

  TextNodes can actually have children.
  If there are tags we don't understand, we shouldn't get in the way::

    {#$% if True %$#}
      %div
        content
    {#$% endif %$#}

  might be perfectly valid, and is technically a TextNode -> HtmlNode ->
  TextNode.
  """

  def __init__(self, data):
    self.data = data
    super(TextNode, self).__init__()

  def render_start(self):
    return self.data


class PreformattedTextNode(TextNode):
  """Represents a text node with pre-formatted text."""

  def get_indent(self, *args, **kwargs):
    """No matter what, ignore the indent level render_lines() wants."""

    return ''

  def render_start(self, force=False):
    from pyhaml_jinja import nodes

    # Special case if we are the first child of an HtmlNode (the parent has
    # already taken care of rendering us).
    if (not force and isinstance(self.parent, nodes.HtmlNode) and
        self.get_previous_sibling() is None):
      return None
    else:
      return super(PreformattedTextNode, self).render_start()

