#!/usr/bin env python
#coding:utf-8

import urllib2
import urllib

class sign:
    def __init__(self):
        self.url = 'http://qiandao.xiyoulinux.org/login1.do'
        self.user_agent = 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36'

    def run(self):
        headers = {'User-Agent': self.user_agent}
        value = {'Number':'04153161'}

        data = urllib.urlencode(value)

        request = urllib2.Request(url = self.url, data = data, headers = headers)
        reponse = urllib2.urlopen(request)


if __name__ == '__main__':

    lab  = sign()
    lab.run()
    pass