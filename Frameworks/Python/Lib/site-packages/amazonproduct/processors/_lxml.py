
"""
XPath based paginators for lxml.etree and lxml.objectify based processors.
"""

from amazonproduct.processors import BaseResultPaginator


class XPathPaginator (BaseResultPaginator):

    """
    Result paginator using XPath expressions to extract page and result
    information from XML.
    """

    counter = current_page_xpath = total_pages_xpath = total_results_xpath = None

    def paginator_data(self, root):
        nspace = root.nsmap.get(None, '')
        def fetch_value(xpath, default):
            try:
                node = root.xpath(xpath, namespaces={'aws': nspace})[0]
                return int(node.text)
            except (IndexError, ValueError):
                return default
        return map(lambda a: fetch_value(*a), [
            (self.current_page_xpath, 1),
            (self.total_pages_xpath, 0),
            (self.total_results_xpath, 0)
        ])

    def iterate(self, root):
        nspace = root.nsmap.get(None, '')
        return root.xpath(self.items, namespaces={'aws': nspace})


class SearchPaginator (XPathPaginator):

    counter = 'ItemPage'
    current_page_xpath = '//aws:Items/aws:Request/aws:ItemSearchRequest/aws:ItemPage'
    total_pages_xpath = '//aws:Items/aws:TotalPages'
    total_results_xpath = '//aws:Items/aws:TotalResults'
    items = '//aws:Items/aws:Item'


class RelatedItemsPaginator (XPathPaginator):

    """
    XPath paginator which will work for both :meth:`item_lookup` and
    :meth:`item_search`. The corresponding paths are::

        ItemLookupResponse
          Items
            Item
              RelatedItems
                RelatedItemPage (counter)
                RelatedItemCount (total_results)
                RelatedItemPageCount (total_pages)

        ItemSearchResponse
          Request
            ItemSearchRequest
              ItemPage (counter)
          Items
            TotalResults (total_results)
            TotalPages (total_pages)

    """
    counter = 'RelatedItemPage'
    current_page_xpath = '//aws:RelatedItemPage'
    total_pages_xpath = '//aws:RelatedItems/aws:RelatedItemPageCount'
    total_results_xpath = '//aws:RelatedItems/aws:RelatedItemCount'
    items = '//aws:RelatedItems/aws:RelatedItem/aws:Item'


