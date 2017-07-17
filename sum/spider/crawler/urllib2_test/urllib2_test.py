#!/usr/bin/env python
#coding:utf-8

import urllib2
import urllib

if __name__ == '__main__':

    #设置requset的头部
    url = 'https://www.oschina.net/action/user/hash_login?from='
    headers = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36'}

    #数据字典
    valuse = {'email': 'catehrine199787@outlook.com', 'pwd': '213027795c5b355bc06bee7abda2989d485db82a', 'verifyCode': 'jsdkfm'}
    #数据转换
    data = urllib.urlencode(valuse)
    request = urllib2.Request(url = url, data = data, headers=headers)

    #发送请求并接收响应
    response = urllib2.urlopen(request)

    print response.read()
