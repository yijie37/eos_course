# 第三课作业
##自定义存储表，观察ram消耗

建了几个表测试ram消耗/**
 * 测试操作multi_index消耗的RAM
 * 0.一个primary key，两个secondary key
 * 1.primary key 使用auto increament
 * 2.不使用 EOSLIB_SERIALIZE序列化
 * 3.去掉一个没有索引的字段
 * 4.去掉一个二级索引字段
 */
合约见代码见ramtests.cpp

![**1.编译合约**](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/ramtests_compile.png)

![**2.部署合约**](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/deploy_ram0.png)

此时ram使用 184K

**3.测试0**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/add.png)
ram消耗 0.6K

**4.测试1**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/add1.png)
ram消耗 0.4K

**5.测试2**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/add2.png)
ram消耗 0.4K

**6.测试3**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/add3.png)
ram消耗 0.4K

**7.测试4**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/add4.png)
ram消耗 0.3K

**8.update**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/update.png)
ram消耗 0K

**9.delete**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/del.png)
ram释放 0.6K

**10.总结**
和原始的emplace 4个字段（消耗0.6K）相比:
1.插入字段越少，消耗ram越少。
2.插入一个相同字段但没有EOSLIB_SERIALIZE的multi_task，消耗ram反而更少。（和理解不符）
3.update没有ram消耗。（是否真的不消耗？还是显示ram used只到0.1K导致看不出来？）
4.erase会释放ram。

##kpi合约
合约代码见kpi.cpp

**1.部署合约**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/deploy_kpi.png)

**2.调用前账户的EOS和记录表内容**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/before_token.png)

![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/before_record.png)

**3.调用record**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/call_record.png)

**4.调用后账户的EOS和记录表内容**
![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/after_token.png)

![](https://github.com/yijie37/eos_course/blob/master/lesson3/resources/after_record.png)
