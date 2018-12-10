"""Represent an empty line."""

from pyhaml_jinja.nodes.node import Node


__all__ = ['EmptyNode']


class EmptyNode(Node):
  """Represents an empty line (mostly for debugging)."""

  pass

