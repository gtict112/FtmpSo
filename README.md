# FtmpSo

## 原理

- 经过mtp加固的部分游戏，会有资源校验，就是你无论修改安装包里面的任何一个资源，都无法正常进入游戏，通过分析，其检测在libtprt.so中。这个.so文件在com.tencent.tpshell.TPShellApplication类中加载，在这个类中有很多native方法，我们需要重点关注initialize这个函数，通过hook RegiserNatives函数，我们也发现了这个函数是最先被调用的，我们可以在java 层hook 这个函数来绕过校验，具体实现<a link="https://github.com/FateHack/Ftmp">ftmp</a>，缺点是得依赖于xposed。
- 我们也可以无需依赖于xposed，直接修改dex就能达到目的，但是对于大部分来说，修改smali比较麻烦。我们可以在native 层去hook initialize，此类方法属于jni 动态注册。我们可以直接去hook RegiserNatives拿到initialize地址，然后hook，改变参数值，具体请看源码。

## 使用方法

- 可以使用apktool，android killer或者mt管理器等dex反编译&编译工具，找到com.tencent.tpshell.TPShellApplication这个类，在加载tprt.so文件前插入加载我们的.so文件

  ### 例如：

  ```smali
     const-string/jumbo v0, "ftmgp"
  
     invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
     
  	 const-string/jumbo v0, "tprt"
  
     invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
  ```

  

然后将我们的.so文件放在/lib/armeabi-v7a/中，打包编译即可，可能部分游戏会有签名校验，无法正常登录，这个问题需要你们自行解决。
- 可以在此基础上，加上自己的一些修改游戏的代码等。

## Unity mono 会卡在进入游戏界面，至于怎么解决得靠你们自己。

