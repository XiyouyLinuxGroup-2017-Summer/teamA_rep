#!/usr/bin/env python
#coding:utf-8

import re

if __name__ == '__main__':

    #1 判断字符串是否全部是小写[a-z]+$
    #
    # str1 = 'fdsfdsfdsfdsfds'
    # ret = re.match('[a-z]+$', str1)
    # if ret:
    #     print 'yes'
    # else:
    #     print 'no'


     #match是从开头着，search是从任意位置找，所有要有开头的边界判断
    # str1 = 'fdsfdsfdsfdsfds'
    # ret = re.search('^[a-z]+$', str1)
    # if ret:
    #     print 'yes'
    # else:
    #     print 'no'

     #通过对正则编译
    # str1 = 'fdsfdsfdsfdsfds'
    # regex = re.compile('^[a-z]+$')
    # ret = regex.match(str1)
    # if ret:
    #     print 'yes'
    # else:
    #     print 'no'

    #提取分组的字符串,(\D+)(\d+)(\D+)(\d+)
    # str1 = 'ds233rdc2435'
    # obj = re.search('(\D+)(\d+)(\D+)(\d+)', str1)
    # print obj.group()
    # print obj.group(0)
    # print obj.group(1)
    # print obj.group(2)
    # print obj.group(3)
    # print obj.group(4)

    #提取邮箱和手机号,findall返回列表，
    str = 'cat17629066807herine199787@qq.com'
    regex_phone = re.compile('\d{11}')
    print regex_phone.findall(str)




