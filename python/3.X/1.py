#!/usr/bin/python3   (在Linux/Unix系统中，你可以在脚本顶部添加以下命令让Python脚本可以像SHELL脚本一样可直接执行：)

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



    



