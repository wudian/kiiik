#! 

> ###作者：吴典（Andy.Woo） 联系方式 qq：84020702
>



启动服务器：	bin/WarehouseReceiptSystem.exe

启动客户端：	bin/WrUiClient.exe		账号kiiik  密码888888




# 《环境搭建》
> 1.vs2013、qt5.5.1      下载地址： http://pan.baidu.com/s/1boUNd07
> 1.KIIIK_HOME设为本工程根目录，另外自行设置PYTHON_HOME、JAVA_HOME等



#《一》项目

> 1.debug_client： okcoin的websocket c++项目，用于取ticker、depth、交易
目前用于向mysql存数据

> 2.NewTrader： ctp期货的量化框架
> 3.行情收集器: 存ctp期货的tick、分钟线

#《二》基础模块
> 1.baselib
> 2.




> svn注意："218.81.139.87"如果发生变化，应该按如下步骤操作svn：
> TortoiseSVN -> Relocate... -> 将ip替换为新的ip   -> 点击OK确认
> 如果SVN Update失败，则需要clean up（将Break Locks选项勾上！）后再Update就行了
