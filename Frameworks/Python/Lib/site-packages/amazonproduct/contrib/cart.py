
class Cart (object):

    """
    Convenience class for working with XML responses of Cart operations.

    A shopping cart instance has the following attributes:

    ``cart_id`` (str)
        Alphanumeric token returned by :meth:`create_cart` that identifies a
        cart.

    ``hmac`` (str)
        The Hash Message Authentication Code is an encrypted alphanumeric token
        that is used to authenticate requests.

    ``url`` (str)
        URL that customers should use to purchase the items in the cart. It
        includes the Associate's ID. It is important that they use this URL
        otherwise the Associate will not get credit for the purchase.

    ``subtotal`` (float, str)
         The total price of all of the items in a car but not including tax or
         shipping.

    ``items`` (list)
        List of :class:`Item` instances in cart.
    
    """

    def __init__(self):
        self.items = []
        self.cart_id = self.hmac = None
        self.url = None
        self.subtotal = (None, None)

    def __getitem__(self, key):
        for item in self.items:
            if key in (item.asin, item.item_id):
                return item
        raise IndexError(key)

    def __len__(self):
        return sum([item.quantity for item in self.items])

    def __iter__(self):
        return iter(self.items)

    def __repr__(self):
        try:
            return '<Cart %s %s %.2f %s>' % (self.cart_id, self.items,
                self.subtotal[0]/100.0, self.subtotal[1])
        except TypeError:
            return '<Cart at %s>' % hex(id(self))

    def get_itemid_for_asin(self, asin):
        """
        Returns the ``item_id`` of the first item to match the given ``asin``.

        :param asin: ASIN
        :type asin: str
        """
        for item in self.items:
            if asin == item.asin:
                return item.item_id
        raise None


class Item (object):

    """
    Item in a :class:`Cart`.
    """

    def __init__(self):
        self.item_id = None
        self.quantity = self.asin = None
        self.title = self.product_group = None
        self.price = self.total = (None, None)
        self.merchant_id = None
        self.seller = None

    def __repr__(self):
        if self.item_id is None:
            return '<Item at %s>' % hex(id(self))
        return '<Item %ix %s (=%s %s)>' % (
            self.quantity, self.asin, self.total[0], self.total[1])

