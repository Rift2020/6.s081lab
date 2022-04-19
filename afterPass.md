人家高手的解答：http://xv6.dgs.zone/labs/answers/lab5.html

最后一个Lazytests and Usertests没能处理好所有的细节



1.忘记处理sbrk缩减内存后空间不能小于0了:不过这不影响usertests分数

2.嗨害嗨，我也想到了uvmunmap要用两个continue，不过是通过观察walk函数+print大法得出的，属于是只知道要改而不太清楚为什么要改

3.关于系统调用处理lazy的页面。。。我没能解决，我想了两种：

- 每次系统调用函数里检查一下然后分配(但我后来觉得这太麻烦了(其实可以封装成函数))

- 不做任何检查(甚至去除copyin的检查)，让它跳入kerneltrap然后再做处理(似乎理论上是可以的，但我没能修好)

高手是直接把这个检查嵌在argaddr了(我看了以后也照着就这么写了)，也就是syscall读取地址参数的地方，不过我感觉略有不妥，万一有另外一种分配地址(可能类似sbrk)等等的特别系统调用以后要添加，嵌在argaddr里也不太好，最好还是写一个额外的addrcheck，argaddr之后顺便调用一下就可以了(大差不差)

另外，检查用户栈下面的无效页面，也一开始没想到

高手是这样写的

`PGROUNDUP(p->trapframe->sp) - 1 < *ip`

但是..`r_stval()>p->trapframe->sp`

我这样写也过了...而且我觉得我这样写也对啊...不太懂