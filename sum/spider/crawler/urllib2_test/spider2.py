#!/usr/bin/env python
#coding:utf-8

import urllib2
import urllib
import re
import os




class Spider(object):

    def __init__(self):
        self.url = 'https://www.qiushibaike.com/8hr/page/%s/?s=4999744'
        self.user_agent = 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36'


    def getPage(self, page_index):

        headers = {'User-Agent': self.user_agent}
        try:
            request = urllib2.Request(url=self.url%str(page_index), headers=headers)
            response = urllib2.urlopen(request)
            content = response.read()
            return content
        except urllib2.HTTPError as e:
            print e
            exit()
        except urllib2.URLError as e:
            print e
            exit()

    def analysis(self, content):

        pattern = '<div class="content">.*?<span>(.*?)</span>.*?</div>'
        regex = re.compile(pattern, re.S)  # re.S可以使.匹配到\n
        items = regex.findall(content)
        return items

    def save(self, path, items, i):

        for item in items:

            new = item.replace('\n', '').replace('<br/>', '\n')

            if not os.path.exists(path):
                os.makedirs(path)
            filepath = path + str(i) + '.txt'
            f = open(filepath, 'w')
            f.write(new)
            f.close
            print '第' + str(i) + '条爬取成功'
            i = i + 1

    def run(self, ):
        print '开始'
        i = 1
        for page in range(1, 35):
            content = self.getPage(page)
            items = self.analysis(content)
            path = '/Users/sequin_yf/Desktop/糗百spider_class/'
            self.save(path,items, i)
            i = i + 20
        print '结束'

if __name__ == '__main__':
    spider = Spider()
    spider.run()
    pass