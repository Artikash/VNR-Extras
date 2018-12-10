"""Nodes to be converted to Jinja tags."""

import re

from pyhaml_jinja.nodes.node import Node


__all__ = ['JinjaNode', 'SelfClosingJinjaNode']


class JinjaNode(Node):
  """Represents a node that will be rendered as a Jinja tag."""

  TAG_REGEX = re.compile(
      r'^'  # Start of the line
      r'-'  # - is required
      r'(?P<tag>\w+)'  # Tag name is required
      r'(?P<data>:?\s+.+)?'  # Data is optional
      r'$'  # End of the line
  )

  SELF_CLOSING_TAGS = ['break', 'continue', 'do', 'extends', 'from', 'import',
                       'include', 'set']

  EXTENDING_TAGS = {
      'if': ['else', 'elif'],
      'for': ['else'],
      'elif': ['elif', 'else'],
      'trans': ['pluralize'],
      }

  def __init__(self, tag, data=None):
    super(JinjaNode, self).__init__()
    self.tag = tag
    self.data = data or ''

  def __repr__(self):
    return '<{node_type}: {tag} {data} {children} children>'.format(
        node_type=self.__class__.__name__,
        tag=self.tag, data=self.data,
        children=len(self.get_children()))

  @classmethod
  def from_haml(cls, haml):
    """Given a line of HAML markup parse it into a Jinja node."""

    match = cls.TAG_REGEX.match(haml)
    if not match:
      raise ValueError('Text did not match %s' % cls.TAG_REGEX.pattern)

    # Create the node with the proper tag.
    tag = match.group('tag')
    if tag in cls.SELF_CLOSING_TAGS:
      node = SelfClosingJinjaNode(tag=tag)
    else:
      node = cls(tag=tag)

    data = (match.group('data') or '').strip()

    # To handle nested expressions, we need to be sure that colons are only
    # split apart when they aren't inside brackets, braces, or quotes (both
    # single and double).
    # To do this, we'll just go through "data" and keep a stack of where we
    # are. The first colon we find that is outside the stack is a splitter.
    stack = []
    chars = {
        '[': ']',
        '(': ')',
        '{': '}',
        '\'': '\'',
        '"': '"',
        }

    for index, char in enumerate(data):
      # If we found a colon and the stack is empty, treat it as the splitter
      # for a nested tag. Split, re-parse the rest, attach as a child, and
      # update data to what it should be. Then break out of the loop.
      if not stack and char == ':':
        from pyhaml_jinja.parser import Parser
        child = Parser.parse_line(data[index+1:].strip())
        node.add_child(child)
        data = data[:index]
        break

      # If we might be able to close an open tag.
      if char in chars.values():
        # The stack is not empty and this tag closes it.
        if stack and char == stack[-1]:
          stack.pop()
          continue

        # The stack is not empty, but this tag didn't close it, AND it's not a
        # valid opener. Throw an error.
        elif stack and char not in chars.keys():
          raise ValueError('Found unexpected closing tag "%s".' % char)

      # If we're opening, add the proper closing tag to the stack.
      if char in chars.keys():
        stack.append(chars.get(char))

      # TODO: Figure out what we do with backslash escaping characters.

    node.data = data
    return node

  def is_extending(self, node):
    """Determine if the current node is extending a previous node."""
    if not node or not isinstance(node, self.__class__):
      return False

    return self.tag in self.EXTENDING_TAGS.get(node.tag, [])

  def render_start(self):
    return '{%% %s %%}' % ' '.join([self.tag, self.data or '']).strip()

  def render_end(self):
    # Look at our next sibling. If they are extending us, don't return an end
    # tag.
    next_sibling = self.get_next_sibling()
    if (isinstance(next_sibling, self.__class__) and
        next_sibling.is_extending(self)):
      return None

    # If we *are* going to close this tag, get to the root of the sibling that
    # we are extending.
    tag = self.tag
    previous_sibling = self.get_previous_sibling()
    while self.is_extending(previous_sibling):
      tag = previous_sibling.tag
      previous_sibling = previous_sibling.get_previous_sibling()

    return '{%% end%s %%}' % tag


class SelfClosingJinjaNode(JinjaNode):
  """A Jinja tag that closes itself (-extends "base.haml")."""

  def render_end(self):
    return None

