"""Represents a custom block of (usually) pre-formatted text."""

from pyhaml_jinja.nodes.node import Node


__all__ = ['CustomBlockNode']


class CustomBlockNode(Node):

  BLOCK_TYPES = {
      'javascript': ('<script type="text/javascript">', '</script>'),
      'plain': ('', ''),
      'css': ('<style type="text/css">', '</style>'),
      }

  def __init__(self, block_type):
    super(CustomBlockNode, self).__init__()

    if block_type not in self.BLOCK_TYPES:
      raise ValueError('Unknown custom block type: "%s".' % block_type)

    self.block_type = block_type
    self.start, self.end = self.BLOCK_TYPES[block_type]

  def render_start(self):
    return self.start

  def render_end(self):
    return self.end

