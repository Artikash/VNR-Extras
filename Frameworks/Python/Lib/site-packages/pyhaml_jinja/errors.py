"""Custom exceptions used in the project."""

from jinja2 import TemplateSyntaxError


__all__ = ['TemplateIndentationError', 'TemplateSyntaxError']


class TemplateIndentationError(TemplateSyntaxError):
  """Raised when a line's indentation has an error.

  Usually this is when a single line mixes tabs and spaces, however it is also
  raised when a line doesn't match any of the parent line's indentation levels.
  """
  pass

