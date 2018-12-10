
# paginator types
ITEMS_PAGINATOR = 'ItemPage'
RELATEDITEMS_PAGINATOR = 'RelatedItemPage'


class BaseProcessor (object):

    """
    Skeleton class for processors.

    If you like to implement your own result processing, subclass
    :class:`BaseProcessor` and override the methods.
    """

    #: contains mapping of paginator types (e.g ``ITEMS_PAGINATOR``) to the
    #: appropriate subclass of :class:`BaseResultPaginator`
    paginators = {}

    def parse(self, fp):
        """
        Parses a file-like XML source returned from Amazon. This is the most
        import method of this class!

        :return: parsed XML node
        """
        raise NotImplementedError  # pragma: no cover

    @classmethod
    def parse_cart(cls, node):
        """
        Returns an instance of :class:`amazonproduct.contrib.Cart` based on
        information extracted from ``node``.

        Obviously, this has to be implemented in each subclass of
        :class:`BaseProcessor`.

        :param node: parsed XML node (as returned by :meth:`parse`).
        :return: a :class:`~amazonproduct.contrib.Cart` instance
        """
        raise NotImplementedError  # pragma: no cover


class BaseResultPaginator (object):

    """
    Wrapper class for paginated results. This class will call the passed
    function iteratively until either the specified limit is reached or all
    result pages, which can be retrieved, are fetched.

    .. note:: Amazon does put a rather restrictive limit on pagination. Don't
       expect to be able to retrieve all result pages!

    A result paginator has the following attributes:

    ``pages`` (same as ``len(<paginator>)``)
        Number of *total* pages. This may differ from the number of pages
        actually iterated over because of limits either imposed by Amazon or
        yourself (using ``limit``).

    ``results``
        Number of total results. This may differ from the number of results
        actually retrievable because Amazon generally limits pagination to ten
        pages.

    ``current``
        Number of result page retrieved last.
    """

    #: Default pagination limit imposed by Amazon.
    LIMIT = 10

    counter = None
    items = None

    def __init__(self, fun, *args, **kwargs):
        """
        :param fun: original API method which will be called repeatedly with
        ``args`` and ``kwargs``.
        """
        self.fun = fun
        self.args, self.kwargs = args, kwargs
        self.limit = kwargs.pop('limit', self.LIMIT)

        self._pagecache = {}

        # fetch first page to get pagination parameters
        self.page(kwargs.get(self.counter, 1))

    def __iter__(self):
        """
        .. versionchanged:: 0.2.6
           Iterates over items rather than pages. Use :meth:`iterpages` to do
           the former.

        Iterate over all paginated results of ``fun`` returning the items.
        """
        for page in self.iterpages():
            for item in self.iterate(page):
                yield item

    def __len__(self):
        """
        Returns the number of pages which can be *iterated over* as opposed to
        the total number of pages which Amazon tell you there are (but won't
        give you in their entirety).
        """
        if self.pages < self.limit:
            return self.pages
        return self.limit

    def page(self, index):
        """
        Fetch single page from results.
        """
        self.kwargs[self.counter] = index
        # use cached page if found
        if index in self._pagecache:
            root = self._pagecache[index]
        else:
            root = self.fun(*self.args, **self.kwargs)
            self._pagecache[index] = root
        self.current, self.pages, self.results = self.paginator_data(root)
        return root

    def iterpages(self):
        """
        Iterates over all pages. Keep in mind that Amazon limits the number of
        pages it makes available, although attribute ``pages`` may say
        otherwise!
        """
        yield self.page(1)
        while self.pages > self.current < self.limit:
            yield self.page(self.current + 1)

    def paginator_data(self, node):
        """
        Extracts pagination data from XML node, i.e.

        * current page
        * total number of pages
        * total number of results

        .. note:: *Number of pages* and *number of results* which may differ
           from the ones that Amazon is actually willing to return!

        :return: ``(current page, total pages, total results)``
        """
        raise NotImplementedError # pragma: no cover

    def iterate(self, node):
        """
        Returns iterable over XML item nodes.
        """
        raise NotImplementedError # pragma: no cover
