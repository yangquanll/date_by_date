#!/usr/bin/python3   (在Linux/Unix系统中，你可以在脚本顶部添加以下命令让Python脚本可以像SHELL脚本一样可直接执行：) 2019-2-13

import math
import os
#段注释 用 ‘’‘ 和 “”“
''' 
list=[1,2,3,4]
it = iter(list)    # 创建迭代器对象
for x in it:
    print (x, end=" \n")
'''
##############################
'''
逻辑判断后面要接 ：号
缩进要保持 一致
可以是 一个 空格也可以是 一个 tab
'''
if True:
 print("OK \n")
 print ("ssss\n")
else:
 print("NO \n")

##############################
'''
多行语句可以使用 \ 来隔开
'''
Tot = 1 +\
      22222 +\
      888
print (Tot)

##############################
'''
数据类型
python 3 只有三种基本数据类型（int，bool，str） 
python基本数据类型

    int        整数，用来进行数学运算 (整数), 如 1, 只有一种整数类型 int，表示为长整型，没有 python2 中的 Long。
    str        字符串，存放少量数据
    bool     布尔值，用来判断True，False bool (布尔), 如 True
    float (浮点数), 如 1.23、3E-2
    list        列表，存放大量数据，[ ]表示
    tuple    元组，只读列表，( ) 表示
    dict      字典，存放大量数据，{key:value}表示
    set        集合，存放大量数据，不重复

'''

a = 2222222222
lang = ["C", "C++", "Perl", "Python"] 

str = "qqqqq"

word = 'yy'

for  c in lang:
    print (c)

for  c in str+word:
    print (c)

###########################
# 运算符

b = 66
c = 11
d = b**11
print(d)

d = b%c
print(d)

d =b//c # 取整除 - 向下- 取接近除数的整数
print(d)


'''
>>> 9//2
4
>>> -9//2
-5
'''

d = b+1 // c
print(d)

d = 9//2
print("9//2 =  \n",d)

d = (b+1) // c
print("(b+1) // c = \n",d)

#赋值运算

d *= c   # b = b*c

print("d *= c = \n",d)

#位运算

# 二进制 bin()   十六进制 hex()  八进制 oct()


e = 60
f = 13
print("e = ",)
g = e|f
g1 = e&f
g2 = e^f
g3 = e << 2
g4 = e >> 3

print ( "f = ",bin(f),"e = ",bin(e),"e|f = ",bin(g), "e&f = ", bin(g1),"e^f = ",bin(g2), " e << 2 = ",bin(g3), " e >> 3 = ",bin(g4))

#逻辑运算符 and &&  or ||  not  

x = 2
y = 4

if ( x>0 and y+x < 9):
    print( "if can \n")
else:
    print( "if not can")

#身份运算符 is is not
'''
is 与 == 区别：

is 用于判断两个变量引用对象是否为同一个， == 用于判断引用变量的值是否相等。
'''

a = [1,3,4,9]
b = a
if (a is b):
    print ("a is b")
else:
    print ("a is not b")
    

#成员运算符
'''
in	如果在指定的序列中找到值返回 True，否则返回 False。 	x 在 y 序列中 , 如果 x 在 y 序列中返回 True。
not in	如果在指定的序列中没有找到值返回 True，否则返回 False。 	x 不在 y 序列中 , 如果 x 不在 y 序列中返回 True。
'''
b = 1
c =8

if(b in a):
    print("b in a ")

if(c in a):
    print("c in a ")
else:
    print("c mot in a ")

if(c not in a):
    print("c not in a ")


#运算符优先级
"""
* 	指数 (最高优先级)
~ + - 	按位翻转, 一元加号和减号 (最后两个的方法名为 +@ 和 -@)
* / % // 	乘，除，取模和取整除
+ - 	加法减法
>> << 	右移，左移运算符
& 	位 'AND'
^ | 	位运算符
<= < > >= 	比较运算符
<> == != 	等于运算符
= %= /= //= -= += *= **= 	赋值运算符
is is not 	身份运算符
in not in 	成员运算符
and or not 	逻辑运算符
"""
########################### 2019-2-14
#数字相关
mu = 0xa1 #八进制的直接 写
print("mu = ",mu)

#数字类型转换
i = 1.1111
o = 1.666

print(" i =  o = ",int(i),int(o),complex(i))

#注意的是 除法运算  /  得到的是一个 float型的数 要想得到一个整数  要用 //(取整除)
#// 得到的并不一定是整数类型的数，它与分母分子的数据类型有关系
#不同类型的数混合运算时会将整数转换为浮点数：

print(" 9/4 = ", 9/4)
print(" 9//4 = ", 9//4)
print(" 9.0//4 = ", 9.0/4)
print(" 3 * 3.75 / 1.5 = ", 3 * 3.75 / 1.5)

# 用 ** 来取 幂mi 运算

print(" 2 ** 4 = ", 2 ** 4)     
   

#在交互模式中，最后被输出的表达式结果被赋值给变量 _  , _ 变量应被用户视为只读变量。 
ta = 100.11
pr = 2
r = ta * pr
print(" round(r, 2)  = ", round(r, 2))
#print(" _ = ", _)
#print(" round(_, 2)  = ", round(_, 2))

'''
数学函数
函数	返回值 ( 描述 )
abs(x)	返回数字的绝对值，如abs(-10) 返回 10
ceil(x) 	返回数字的上入整数，如math.ceil(4.1) 返回 5

cmp(x, y)
	如果 x < y 返回 -1, 如果 x == y 返回 0, 如果 x > y 返回 1。 Python 3 已废弃 。使用 使用 (x>y)-(x<y) 替换。
exp(x) 	返回e的x次幂(ex),如math.exp(1) 返回2.718281828459045
fabs(x)	返回数字的绝对值，如math.fabs(-10) 返回10.0
floor(x) 	返回数字的下舍整数，如math.floor(4.9)返回 4
log(x) 	如math.log(math.e)返回1.0,math.log(100,10)返回2.0
log10(x) 	返回以10为基数的x的对数，如math.log10(100)返回 2.0
max(x1, x2,...) 	返回给定参数的最大值，参数可以为序列。
min(x1, x2,...) 	返回给定参数的最小值，参数可以为序列。
modf(x) 	返回x的整数部分与小数部分，两部分的数值符号与x相同，整数部分以浮点型表示。
pow(x, y)	x**y 运算后的值。
round(x [,n])	返回浮点数x的四舍五入值，如给出n值，则代表舍入到小数点后的位数。
sqrt(x) 	返回数字x的平方根。

'''

'''
round 注意
1)4舍6入5看齐,奇进偶不进
（1）要求保留位数的后一位如果是4或者4以下的数字，则舍去， 例如 5.214保留两位小数为5.21。
（2）如果保留位数的后一位如果是6或者6以上的数字，则进上去， 例如5.216保留两位小数为5.22。
（3）如果保留位数是保留整数部分或保留一位小数，则要根据保留位来决定奇进偶舍：
>>> round(5.215,2)#实际并没有进位
5.21
>>> round(5.225,2)
5.22
>>>
>>> round(1.5)#此处进位
2
>>> round(1.5)==round(2.5)#偶数舍去
True
>>> round(1.15,1)
1.1
>>> round(1.25,1)
1.2
>>> round(1.151,1)
1.2
>>> round(1.251,1)
1.3

(4) 如果保留位数的后一位如果是5，且该位数后有数字。则进上去，例如5.2152保留两位小数为5.22，5.2252保留两位小数为5.23，5.22500001保留两位小数为5.23。

从统计学的角度，“奇进偶舍”比“四舍五入”要科学，在大量运算时，它使舍入后的结果误差的均值趋于零
'''
########################### 2019-2-18
# 字符串
arry = " hhh yq \n"
arry1 = 'yq yl' # 字符串的 内容可以用 ’‘ “” 括起来

print (arry,arry1)
print (arry[0:2])

print ("已更新字符串 : ", arry[:3] + ' ylaiw!')

#列表 
list = ['yy','qq','ll',66,2009]
print (list)
print ( "l3 = ",list[3]," l :3 = ",list[:3])
list[2] = 'ss'
print (list)

####################### 2019 - 2 -20
#字典
'''
字典是另一种可变容器模型，且可存储任意类型对象。

字典的每个键值(key=>value)对用冒号(:)分割，每个对之间用逗号(,)分割，整个字典包括在花括号({})中 ,格式如下所示：
'''
#dict = { 'y': 'yq','l':66, 'qq':99,'ww': ww }
#print("dict y",dict['y']);
dict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First', '99': 'qq'}

dict['Age'] = 8;               # 更新 Age
dict['School'] = "cccc"  # 添加信息


print ("dict['Age']: ", dict['99'])
print ("dict['School']: ", dict['School'])

#删除字典元素
del dict['99']
#print(dict["99"])

'''
1	len(dict)
计算字典元素个数，即键的总数。 	

>>> dict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First'}
>>> len(dict)
3

2	str(dict)
输出字典，以可打印的字符串表示。 	

>>> dict = {'Name': 'Runoob', 'Age': 7, 'Class': 'First'}
>>> str(dict)
"{'Name': 'Runoob', 'Class': 'First', 'Age': 7}"
'''
print(len(dict))

########################## 2019 -2-21

#if while
if 7 > 8:
    print("shzi")
else:
    print("cm")

num = int(input("please input numb："))
if num < 0:
    print("shasha")
elif num >2:
    print("mb>2")

nu = 1;
while nu <10:
    print("nu = ",nu)
    nu = nu+1

#无限循环 while
w = 0
"""
while 1:
    input("name = ")
    print("w = ",w)
    w = w+1
    if w > 3:
        break   #can end while break 语句可以跳出 for 和 while 的循环体。如果你从 for 或 while 循环中终止，任何对应的循环 else 块将不执行
"""
while 1:  #可以使用 else
    input("name = ")
    print("w = ",w)
    w = w+1
    if w > 3:
        print("into continue")   #can end while break 语句可以跳出 for 和 while 的循环体。如果你从 for 或 while 循环中终止，任何对应的循环 else 块将不执行
#        continue                 #can  under not function
        break
        print("end continue")
    print("in while")   
#else:
           
# FOR USE

va = ["ww","yy","qq",11.99,99,66]

for w in va:
    print(w)
    
#for i in range(6): #range auto 0-6
#    print(i)
for i in range(len(va)): 
    print(i,va[i])

for i  in range(len(va)):
    print(i)
    if i == "qq":
        break;
else:
    print ("for")
    
        
