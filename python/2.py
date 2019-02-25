#coding:utf-8
import urllib

page = urllib.urlopen('https://www.baidu.com/')#打开网页
htmlcode = page.read()#读取页面源码
print htmlcode#在控制台输出
