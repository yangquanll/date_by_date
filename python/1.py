#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：test.py

# 第一个注释
print "Hello, Python!";  # 第二个注释

counter = 100 # 赋值整型变量
miles = 1000.0 # 浮点型
name = "John" # 字符串

print counter
print miles
print name
str = "123456789"


print str[0]
print str
print str[2:]

for letter in 'Python':     # 第一个实例
   print '当前字母 :', letter

fruits = ['banana', 'apple',  'mango']
for fruit in fruits:        # 第二个实例
   print '当前水果 :', fruit

numbers = [11,23,12,34,35]

a = []
b = []

while len(numbers) > 0 :
    number = numbers.pop()
    if(number % 2 == 0) :
        a.append(number)
    else:
        b.append(number)

print 'a = ',a
print 'b = ',b

import urllib
import re

def get_member():
    print 's 1'
    response = urllib.request.urlopen("http://521xunlei.com/portal.php")
    html = str(response.read(), 'gbk')
    result = re.search(r'<div id="portal_block_62_content"[\s\S]+?<a href="(thread.+?)"[\s\S]+?</li>', html)
    print 's 2'
    if result is None:
        return None
    url = r'http://521xunlei.com/' + result.group(1)
    response = urllib.request.urlopen(url)
    html = str(response.read(), 'gbk')
    print 's 3'
    pattern = re.compile(r'(?:账号共享|迅雷会员账号分享|迅雷号|迅雷账号|迅雷共享号|迅雷会员账号|共享账号)([a-zA-Z0-9]+?:[12]).*?(?:分享密码|密码分享|首发密码)(.+?)(?:<br|</font>)')
    result = pattern.findall(html)
    print 's 4'
    if len(result) == 0:
        return None
    else:
        return  result


get_member()
