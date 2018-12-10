
import socket
import time
import urllib2

from amazonproduct.api import API

class RetryAPI (API):

    """
    API which will try up to ``TRIES`` times to fetch a result from Amazon
    should it run into a timeout. For the time being this will remain in
    :mod:`amazonproduct.contrib` but its functionality may be merged into the
    main API at a later date.

    Based on work by Jerry Ji
    """

    #: Max number of tries before giving up
    TRIES = 5

    #: Delay between tries in seconds
    DELAY = 3

    #: Between each try the delay will be lengthened by this backoff multiplier
    BACKOFF = 1

    def _fetch(self, url):
        """
        Retrieves XML response from Amazon. In case of a timeout, it will try
        :const:`~RetryAPI.TRIES`` times before raising an error.
        """
        attempts = 0
        delay = self.DELAY

        while True:
            try:
                attempts += 1
                return API._fetch(self, url)
            except urllib2.URLError, e:

                # if a timeout occurred
                # wait for some time before trying again
                reason = getattr(e, 'reason', None)
                if isinstance(reason, socket.timeout) and attempts < self.TRIES:
                    time.sleep(delay)
                    delay *= self.BACKOFF
                    continue

                # otherwise reraise the original error
                raise

