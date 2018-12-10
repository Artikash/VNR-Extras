# Copyright (C) 2009-2013 Sebastian Rahlf <basti at redtoad dot de>
#
# This program is release under the BSD License. You can find the full text of
# the license in the LICENSE file.

from lxml import etree

from amazonproduct.contrib.cart import Cart, Item
from amazonproduct.errors import AWSError
from amazonproduct.processors import BaseProcessor
from amazonproduct.processors import ITEMS_PAGINATOR, RELATEDITEMS_PAGINATOR

from amazonproduct.processors._lxml import SearchPaginator
from amazonproduct.processors._lxml import RelatedItemsPaginator


class Processor (BaseProcessor):

    """
    Result processor using lxml.etree.
    """

    paginators = {
        ITEMS_PAGINATOR: SearchPaginator,
        RELATEDITEMS_PAGINATOR: RelatedItemsPaginator,
    }

    def parse(self, fp):
        root = etree.parse(fp).getroot()
        nspace = {'aws': root.nsmap.get(None, '')}
        errors = root.xpath('//aws:Error', namespaces=nspace)
        for error in errors:
            raise AWSError(
                code=error.findtext('./aws:Code', namespaces=nspace),
                msg=error.findtext('./aws:Message', namespaces=nspace),
                xml=root)
        return root

    def __repr__(self):  # pragma: no cover
        return '<%s using %s at %s>' % (
            self.__class__.__name__, getattr(self.etree, '__name__', '???'), hex(id(self)))

    @classmethod
    def parse_cart(cls, node):
        """
        Returns an instance of :class:`amazonproduct.contrib.Cart` based on
        information extracted from ``node``.
        """
        nspace = {'aws': node.nsmap.get(None, '')}
        root = node.find('.//aws:Cart', namespaces=nspace)
        _extract = lambda node, xpath: node.findtext(xpath, namespaces=nspace)

        cart = Cart()
        cart.cart_id = _extract(root, './aws:CartId')
        cart.hmac = _extract(root, './aws:HMAC')
        cart.url = _extract(root, './aws:PurchaseURL')

        def parse_item(item_node):
            item = Item()
            item.item_id = _extract(item_node, './aws:CartItemId')
            item.asin = _extract(item_node, './aws:ASIN')
            item.seller = _extract(item_node, './aws:SellerNickname')
            item.quantity = int(_extract(item_node, './aws:Quantity'))
            item.title = _extract(item_node, './aws:Title')
            item.product_group = _extract(item_node, './aws:ProductGroup')
            item.price = (
                int(_extract(item_node, './aws:Price/aws:Amount')),
                _extract(item_node, './aws:Price/aws:CurrencyCode'))
            item.total = (
                int(_extract(item_node, './aws:ItemTotal/aws:Amount')),
                _extract(item_node, './aws:ItemTotal/aws:CurrencyCode'))
            return item

        try:
            for item_node in root.findall('./aws:CartItems/aws:CartItem', namespaces=nspace):
                cart.items.append(parse_item(item_node))
            cart.subtotal = (node.SubTotal.Amount, node.SubTotal.CurrencyCode)
        except AttributeError:
            cart.subtotal = (None, None)
        return cart

