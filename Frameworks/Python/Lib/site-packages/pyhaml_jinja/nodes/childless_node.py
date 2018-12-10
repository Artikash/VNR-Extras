"""Mixin for nodes that cannot have children."""

from pyhaml_jinja.nodes.node import Node


__all__ = ['ChildlessNode']


class ChildlessNode(Node):
  """Parent class for nodes that cannot have children."""

  def children_allowed(self):
    return False

