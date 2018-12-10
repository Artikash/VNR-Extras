'''
The TagSet() class is the engine of PyBBCode. It handles all parsing and rule
compiling. 
The default_set() function returns a TagSet() pre-populated with some standard 
tags. 
The `extras` dictionary contains some unbuilt example rules. Any of these may 
be enabled by calling tag_set.add_tag(*extras[tag_name]).


Explanation of some not-so-obvious tags:

* css: This tag allows a BB user to use special 'bbcode-' prefixed css classes. 
  The logic behind it is that many BBCode-using forums have dark and light 
  themes installed, so that a font color which is clearly-readable and 
  contrasting on one theme is non-readable on another. If the forum theme 
  provides a CSS class named, for instance, bbcode-contrast, a user can use 
    [css="contrast"]Look![/css]
  safely, rather than worry that his
    [color="red"]Look![/css]
  will be unreadable for half his readers.
  This tag can also be used to provide fonts, indent, etcetera.

* size: This tag allows a BB user to specify a font size in pixels. To keep the 
  font sizes reasonably small, the maximum font size is 50px. The regex is 
  fairly dense, but it either matches a ones place of at least 7, or exactly 
  50, or a tens place from 1 to 4 plus a ones place from 0 to 9.


WARNING: This library does NOT excape HTML before parsing. It recommended that 
your code take care of this, in order to avoid the posting of malicious 
JavaScript. This must be done BEFORE running the code through the parser to 
avoid excaping the translated BBCode.
'''

__version__ = '1.0'

import re


__all__ = ['TagSet', 'default_set', 'extras']


class TagSet(dict):
    '''
    The PyBBCode tag manager and engine.
    '''
    ignore_re = re.compile('(.*?)\[ignore\](.*)\[/ignore\](.*)', re.DOTALL)
    
    def add_tag(self, bbcode, html):
        self[re.compile(bbcode, re.DOTALL)] = html
    
    def replace_groups(self, match, s):
        groups = match.groups()
        return s % {str(i): g for i, g in zip(range(len(groups)), groups)}
    
    def parse(self, code):
        '''
        Parse text containing BBCode.
        '''
        for t in self:
            m = t.search(code)
            while m:
                b, e = m.span()
                r = self.replace_groups(m, self[t])
                code = code[:b] + r + code[e:]
                m = t.search(code)
        return code
    
    def parse_with_ignore(self, code):
        '''
        Parse text containing BBCode, skipping sections demarcated by the 
        [ignore][/ignore] tag.
        '''
        m = self.ignore_re.match(code)
        if not m:
            # No ignore tags left, just parse normally
            return self.parse(code)
        # At this point, we have a match object with three groups:
        # The stuff before the ignore tag, which needs to be parsed normally
        # The stuff within the ignore tag, which needs no more parsing,
        # The stuff after the ignore tag, which may contain more ignore tags.
        return (self.parse(m.group(1)) + m.group(2) + 
                 self.parse_with_ignore(m.group(3)))


def default_set():
    tag_set = TagSet()
    tag_set.add_tag(r'\[b\](.*?)\[/b\]', '<b>%(0)s</b>')
    tag_set.add_tag(r'\[i\](.*?)\[/i\]', '<i>%(0)s</i>')
    tag_set.add_tag(r'\[u\](.*?)\[/u\]', '<u>%(0)s</u>')
    
    tag_set.add_tag(r'\[url\](.*?)\[/url\]', '<a href="%(0)s">%(0)s</a>')
    tag_set.add_tag(r'\[url="(.*?)"\](.*?)\[/url\]', 
                    '<a href="%(0)s">%(1)s</a>')
    
    tag_set.add_tag(r'\[color="(.*?)"\](.*?)\[/color\]', 
                    '<span style="color: %(0)s">%(1)s</span>')
    
    tag_set.add_tag(r'\[big\](.*?)\[/big\]', 
                    '<span style="font-size: 130%">%(0)s</span>')
    tag_set.add_tag(r'\[small\](.*?)\[/small\]', '<small>%(0)s</small>')
    tag_set.add_tag(r'\[size=([7-9]|50|[1-4]\d)\](.*?)\[/size\]', 
                    '<span style="font-size: %(0)spx">%(1)s</span>')
    
    return tag_set


extras = {'css': (r'\[css="(\w*?)"\](.*?)\[/css\]', 
                  '<span class="bbcode-%(0)s">%(1)s</span>')}
