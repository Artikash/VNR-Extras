# Copyright (C) 2009-2013 Sebastian Rahlf <basti at redtoad dot de>
#
# This program is release under the BSD License. You can find the full text of
# the license in the LICENSE file.

from lxml import etree, objectify

from amazonproduct.contrib.cart import Cart, Item
from amazonproduct.errors import AWSError
from amazonproduct.processors import BaseProcessor
from amazonproduct.processors import ITEMS_PAGINATOR, RELATEDITEMS_PAGINATOR

from amazonproduct.processors._lxml import SearchPaginator
from amazonproduct.processors._lxml import RelatedItemsPaginator


class SelectiveClassLookup(etree.CustomElementClassLookup):
    """
    Lookup mechanism for XML elements to ensure that ItemIds (like
    ASINs) are always StringElements and evaluated as such.

    Thanks to Brian Browning for pointing this out.
    """
    # pylint: disable-msg=W0613
    def lookup(self, node_type, document, namespace, name):
        if name in ('ItemId', 'ASIN'):
            return objectify.StringElement


class Processor (BaseProcessor):

    """
    Response processor using ``lxml.objectify``. It uses a custom lookup
    mechanism for XML elements to ensure that ItemIds (such as ASINs) are
    always StringElements and evaluated as such.

    ..warning:: This processors does not run on Google App Engine!
      http://code.google.com/p/googleappengine/issues/detail?id=18
    """

    # pylint: disable-msg=R0903

    paginators = {
        ITEMS_PAGINATOR: SearchPaginator,
        RELATEDITEMS_PAGINATOR: RelatedItemsPaginator,
    }

    def __init__(self):
        self._parser = etree.XMLParser()
        lookup = SelectiveClassLookup()
        lookup.set_fallback(objectify.ObjectifyElementClassLookup())
        self._parser.set_element_class_lookup(lookup)

    def parse(self, fp):
        """
        Parses a file-like object containing the Amazon XML response.
        """
        tree = objectify.parse(fp, self._parser)
        root = tree.getroot()

        #~ from lxml import etree
        #~ print etree.tostring(tree, pretty_print=True)

        try:
            nspace = root.nsmap[None]
            errors = root.xpath('//aws:Error', namespaces={'aws': nspace})
        except KeyError:
            errors = root.xpath('//Error')

        for error in errors:
            raise AWSError(
                code=error.Code.text,
                msg=error.Message.text,
                xml=root)

        return root

    @classmethod
    def parse_cart(cls, node):
        """
        Returns an instance of :class:`amazonproduct.contrib.Cart` based on
        information extracted from ``node``.
        """
        cart = Cart()
        # TODO This is probably not the safest way to get <Cart>
        root = node.Cart
        cart.cart_id = root.CartId.pyval
        cart.hmac = root.HMAC.pyval

        def parse_item(item_node):
            item = Item()
            item.item_id = item_node.CartItemId.pyval
            item.asin = item_node.ASIN.pyval
            item.seller = item_node.SellerNickname.pyval
            item.quantity = item_node.Quantity.pyval
            item.title = item_node.Title.pyval
            item.product_group = item_node.ProductGroup.pyval
            item.price = (
                item_node.Price.Amount.pyval,
                item_node.Price.CurrencyCode.pyval)
            item.total = (
                item_node.ItemTotal.Amount.pyval,
                item_node.ItemTotal.CurrencyCode.pyval)
            return item

        try:
            for item_node in root.CartItems.CartItem:
                cart.items.append(parse_item(item_node))
            cart.url = root.PurchaseURL.pyval
            cart.subtotal = (root.SubTotal.Amount, root.SubTotal.CurrencyCode)
        except AttributeError:
            cart.url = None
            cart.subtotal = None
        return cart

