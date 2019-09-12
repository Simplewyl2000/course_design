# 软件安全课程设计


### 实现功能

1. 同源性检测
  + 单个文件以及批量文件copyRate比较
  + 生成自定义CFG, 并比较各个CFG之间的差距以鉴定程序之间差别

2. 漏洞检测
  + 栈溢出检测
  + 堆溢出检测
  + 格式化字符串检测
  + 整数宽度溢出检测
  + 整数符号不匹配检测
  + 整数运算溢出检测

### 相关依赖

1. 其中对词法的分析使用了CodeSensor，jar文件，
调用方法：

  `java -jar CodeSensor.jar 待检测文件 >结果输出文件`

  所以需要环境中配置好java的路径

2. python相关库依赖

  + networkx  (画CFG)
  + pyqt5
  + pyqt5 tools
  + matplotlib
  + re
  + math

### 基本使用

  `python GUI.py` 打开界面

  但是有时未知bug，建议是用pycharm打开，运行GUI.py即可
