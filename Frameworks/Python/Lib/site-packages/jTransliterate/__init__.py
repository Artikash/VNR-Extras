# -*- coding: utf-8 -*- 
#!/usr/bin/python

__author__ = "Ryan McGrath <ryan@venodesigns.net>"
__version__ = "1.0.3"

"""
    A class that allows for easy transliteration of [Hirag/Katak]ana
    and English/Latin. Much of the work here is inspired/inherited/etc
    from Kim Ahlström and his work on "Ve", built in Ruby. 
    
    Credit where credit is due:
    https://github.com/Kimtaro/ve/blob/master/lib/providers/japanese_transliterators.rb
"""

import re

# Lookup tables for character conversions. Much of this is borrowed from the work of
# Kim Ahlström and Ve: https://github.com/Kimtaro/ve/
#
# Ve's Transliterators are written in Ruby, and I wanted Python. Consider it a nice port. ;)
from translation_maps import H_SYLLABIC_N, H_SMALL_TSU, HIRA_TO_LATN, LATN_TO_HIRA

def defaultToSelfText(fn):
    """
        A fun little decorator that makes it so we can default to
        the text stored on a class instance, but also let people just
        instantiate and re-use calls while supplying new text. Whee.
    """
    def wrapper(self, text = None):
        if text is None:
            text = self.text
        
        return fn(self, text = text)
        
    return wrapper

class JapaneseTransliterator(object):
    def __init__(self, text):
        """__init__(self, text)
        
            JapaneseTransliterator("fadjfnjsfnjsafnjsdnf")
        
            I envisioned storing the original text on the instantiated object
            itself, and allowing it to be overridden on a per-function-call basis.
            
            So I did.
            
            Parameters:
                 text - Text to be operated on. Unicode please!
        """
        self.text = text
    
    @defaultToSelfText
    def transliterate_from_hrkt_to_latn(self, text):
        """transliterate_from_hrkt_to_latn(self, text)
        
            Transliterates from [Hirag/Katak]ana to Latin/En.
            
            Parameters:
                text - Optional. Use different text than what's on
                    the class instance.
        """
        text = self.transliterate_from_kana_to_hira(text)
        return self.transliterate_from_hira_to_latn(text.encode('utf-8'))
    
    @defaultToSelfText    
    def transliterate_from_hira_to_latn(self, text):
        """transliterate_from_hira_to_latn(self, text)
        
            Transliterates from Hiragana to Latin/En. Phonetics, that is. 
            
            Parameters:
                text - Optional. Use different text than what's on
                    the class instance.
        """
        # Decode once, not twice   
        _H_SMALL_TSU = H_SMALL_TSU.decode('utf-8')
        _H_SYLLABIC_N = H_SYLLABIC_N.decode('utf-8')
        
        kana = (text * 1).decode('utf-8')
        romaji = ''
        geminate = False
        
        index = 0
        klength = len(kana)
        
        while klength > 0:
            for length in [2, 1]:
                mora = ''
                for_conversion = kana[index:(index + length)]
                
                if for_conversion == _H_SMALL_TSU:
                    geminate = True
                    index += length
                    klength -= length
                    break
                    
                elif for_conversion == _H_SYLLABIC_N and re.match(u'[\u3084-\u3088]', kana[(index + 1):(index + 2)]):
                    # Syllabic N before ya, yu or yo
                    mora = "n'"
                elif for_conversion in HIRA_TO_LATN:
                    mora = HIRA_TO_LATN[for_conversion]
                
                if len(mora) > 0:
                    if geminate:
                        geminate = False
                        romaji += mora[index:index + 1]
                    
                    romaji += mora
                    index += length
                    klength -= length
                    break
                elif length == 1:
                    romaji += for_conversion
                    index += length
                    klength -= length
        
        return romaji
    
    @defaultToSelfText
    def transliterate_from_latn_to_hrkt(self, text):
        """transliterate_from_latn_to_hrkt(self, text)
        
            Transliterates from Latin/En to Hiragana (mostly).
            
            Parameters:
                text - Optional. Use different text than what's on
                    the class instance.
        """
        # Duplicate the text...
        romaji = (text * 1).decode('utf-8')
        kana = ''
        
        romaji = re.sub('/m([BbPp])/', 'n\1', romaji)
        romaji = re.sub('/M([BbPp])/', 'N\1', romaji)
        
        index = 0
        rlength = len(romaji) - 1
        
        while rlength > 0:
            for for_removal in [3, 2, 1]:
                mora = ''
                for_conversion = romaji[index:(index + for_removal)]
                is_upper = True if re.search('[A-Z][^A-Z]*', for_conversion) else False
                for_conversion = for_conversion.lower()
                
                if re.match('/nn[aiueo]/', for_conversion):
                    mora = H_SYLLABIC_N
                    for_removal = 1
                elif for_conversion in LATN_TO_HIRA:
                    mora = LATN_TO_HIRA[for_conversion]
                elif for_conversion == 'tch' or (for_removal == 2  and re.match('/([kgsztdnbpmyrlwc])\1/', for_conversion)):
                    mora = H_SMALL_TSU
                    for_removal = 1
                
                if mora != '':
                    if is_upper:
                        kana += self.transliterate_from_hira_to_kana(text = (mora * 1))
                    else:
                        kana += mora
                    
                    index += for_removal
                    rlength -= for_removal
                    break
                elif for_removal == 1:
                    kana += for_conversion
                    index += 1
                    rlength -= 1
        
        return kana
    
    @defaultToSelfText
    def transliterate_from_kana_to_hira(self, text):
        """transliterate_from_kana_to_hira(self, text)
        
            Transliterates from Katakana to Hiragana.
            
            Parameters:
                text - Optional. Use different text than what's on
                    the class instance.
        """
        return JapaneseTransliterator.transpose_codepoints_in_range(text, -96, 12449, 12534)
    
    @defaultToSelfText
    def transliterate_from_hira_to_kana(self, text):        
        """transliterate_from_hira_to_kana(self, text)
        
            Transliterates from Hiragana to Katakana.
            
            Parameters:
                text - Optional. Use different text than what's on
                    the class instance.
        """        
        return JapaneseTransliterator.transpose_codepoints_in_range(text, 96, 12353, 12438)
    
    @defaultToSelfText
    def transliterate_from_fullwidth_to_halfwidth(self, text):
        """transliterate_from_fullwidth_to_halfwidth(self, text)
        
            Transliterates from full-width to half-width.
            
            Parameters:
                text - Optional. Use different text than what's on
                    the class instance.
        """
        text = JapaneseTransliterator.transpose_codepoints_in_range(text, -65248, 65281, 65374)
        return JapaneseTransliterator.transpose_codepoints_in_range(text, -12256, 12288, 12288)
    
    @defaultToSelfText
    def transliterate_from_halfwidth_to_fullwidth(self, text):
        """transliterate_from_fullwidth_to_halfwidth(self, text)
        
            Transliterates from half-width to full-width.
            
            Parameters:
                text - Optional. Use different text than what's on
                    the class instance.
        """
        text = JapaneseTransliterator.transpose_codepoints_in_range(text, 65248, 33, 126)
        return JapaneseTransliterator.transpose_codepoints_in_range(text, 12256, 32, 32)
    
    @staticmethod
    def transpose_codepoints_in_range(text, distance, range_start, range_end):
        """JapaneseTransliterator.transpose_codepoints_in_range(text, distance, range_start, range_end)
        
            Given a set of text (unicode...), coupled with distance and range, transposes
            it for a corresponding swap and returns the new set.
        
            Parameters:
                text - text to be transposed, codepoint-wise
                distance - to the other side of the map
                range_start - start of the range we're interested in, codepont-wise
                range_end - end of the range we're interested in, codepoint-wise
            
            Returns:
                string, text, etc
        """
        if not isinstance(text, unicode):
            # Python will raise a UnicodeEncodeError here if there are any
            # outstanding issues, otherwise things should be fine. *shrug*
            text = unicode(text, 'utf-8')
        
        transposed_text = u''
        codepoints = map(lambda char: ord(char), list(text))
        
        for codepoint in codepoints:
            if codepoint >= range_start and codepoint <= range_end:
                transposed_text += unichr(codepoint + distance)
            else:
                transposed_text += unichr(codepoint)
            
        return transposed_text
