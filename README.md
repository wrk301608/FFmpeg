# VideoPlay001

## 记得一键三连哦

> - 使用qt+ffmpeg开发简单的视频播放器，无声音
> - 视频解码使用的是[软解码](https://blog.51cto.com/u_15284125/3079102 "视频解码")即只用CPU进行QPainter绘制每一帧图像，CPU占用过高
> - ![image-20231119121201748](https://cdn.jsdelivr.net/gh/wrk301608/pico_img/image-20231119121201748.png)
> - 简单易学，适合小白入门学习FFMpeg视频解析的基本API

## 遗留问题

> - 视频播放时间的处理，基匀速播放的实现原理



## 项目代码

> [videoPlay001分支](https://github.com/wrk301608/FFmpeg/tree/videoPlay001)

## 项目警告

> - 注：博主本人学习过程的分享，引用他人的文章皆会标注原作者
> - 注：点名某初生DN，未来是属于开源的
> - 注：本人文章非盈利性质，若有侵权请联系我删除
> - 注：联系方式Q：2950319782
> - 注：博主本人很菜，文章基本是二次创作，大佬请忽略我的随笔
> - 注：我会一步步分享实现的细节，若仍有问题请留言，还可以问ChatGPT
> - 

## 项目引用

> - [Qt-FFmpeg开发-视频播放（1）_qt如何实现播放.mp4视频_mahuifa的博客-CSDN博客](https://blog.csdn.net/qq_43627907/article/details/127329028)
> - [Qt使用QPainter绘制方式显示图片_qpainter绘制图片-CSDN博客](https://blog.csdn.net/qq_43627907/article/details/124599535)
> - 

## 问题解决

> - [.qri工程创建](https://blog.csdn.net/qq_18223347/article/details/125982449)
> - [qt创建子工程](https://blog.csdn.net/kllo__/article/details/123210266)
> - [条件编译指令](https://www.cnblogs.com/yhjoker/p/12228761.html)
> - [多文件编程](https://www.cnblogs.com/zjuhaohaoxuexi/p/16470868.html)
> - [qt自定义控件](https://blog.csdn.net/y396397735/article/details/78451245)
> - [QImage转QPixmap原因](https://blog.csdn.net/nolatestudy/article/details/6295064)
> - [FFmpeg基本模块]([FFmpeg 是什么? - fengMisaka - 博客园 (cnblogs.com)](https://www.cnblogs.com/linuxAndMcu/p/12039546.html))
> - [FFmpeg错误处理函数](https://www.cnblogs.com/jj-Must-be-sucessful/p/16693144.html)
> - [AVDictionary使用](https://blog.csdn.net/eieihihi/article/details/114502699)
> - [FFmpeg的基本API](https://www.cnblogs.com/linuxAndMcu/p/12041359.html)
> - [FFmpeg的基本结构体](https://www.cnblogs.com/linuxAndMcu/p/12041578.html)
> - [qreal](https://blog.csdn.net/qq_41801082/article/details/115096489)
> - [FFmpeg时间戳](https://www.cnblogs.com/leisure_chn/p/10584910.html)
> - [QThread](https://www.cnblogs.com/linuxAndMcu/p/11076305.html)

## 开发环境

> - 系统：Win10
> - Qt：5.14.2
> - 编译器：qtcreator  4.11.1， minGW64
> - ffmpeg： 5.12

## 实现功能

> - 使用ffmpeg音视频库软解码实现视频播放器
> - 支持打开多种本地视频文件（如mp4，mov，avi等）
> - 支持解析多种网络视频流（如rtsp，rtmp，http等）
> - 支持视频匀速播放
> - 采用QPainter进行图像显示，支持自适应窗口缩放
> - 视频播放支持实时开始，暂停，继续播放
> - 采用模块化编程，视频解码，线程控制，图像显示各功能分离，低耦合
> - 多线程编程

## 实现逻辑

> - 程序主逻辑
> - ![image-20231119121247974](https://cdn.jsdelivr.net/gh/wrk301608/pico_img/image-20231119121247974.png)
> - ffmpeg软解码流程
> - ![ffmpeg](https://cdn.jsdelivr.net/gh/wrk301608/pico_img/ffmpeg.png)

## 项目实现

### 项目结构

> ![image-20231119121333432](https://cdn.jsdelivr.net/gh/wrk301608/pico_img/image-20231119121333432.png)

### 项目思路

#### 页面搭建

> - 需要实现视频的播放，那么要先要有个页面，使用qtcreator设计界面
> - 不是很复杂的页面我们直接使用自带的ui界面即可
> - ![image-20231120180634222](https://cdn.jsdelivr.net/gh/wrk301608/pico_img/image-20231120180634222.png)
> - 博主使用的是minGW64编译
> - 我们要有一个combo box来获取网络视频流的URL或者本地视频流的地址
> - 还需要一个打开文件的按钮，来打卡文件对话框，选择视频文件
> - 还需要一个开始播放的按钮和一个暂停播放的按钮
> - 至于QPainter绘制解码出来的每一帧图片我们自定义一个PlayImage控件，来显示视频
> - [PlayImage](https://blog.csdn.net/qq_43627907/article/details/124599535)继承自Widget，记得把ui文件中的PlayImage控件提升为[自定义的控件](https://blog.csdn.net/y396397735/article/details/78451245)
> - 最终的ui图
> - ![image-20231120182308679](https://cdn.jsdelivr.net/gh/wrk301608/pico_img/image-20231120182308679.png)

#### 自定义控件

> - 一步步来，我们先实现自定义的控件来解决视频的功能
> - 先创建一个CPP类PlayImage继承自QWidget
> - 为什么不用QLabel显示图片呢，因为显示静态的图片还可以，但是如果像视频这样频繁的更新图片，会使程序变得异常卡顿，因此使用QPainter重绘每一帧图像来实现视频播放的功能
> - 这个Demo图片数据量比较小，完全可以实现，基本的逻辑就是不断更新图像路径，一直重绘，直到没有图像传进来
> - 其实这里自定义控件应该是单独封装的，作为外部文件引入，方便复用，但这里为了简单，还是直接写入代码吧
> - 在这里封装了两个主要的方法updateImage和updatePixmap，解码出的每一帧图片只需要调用对应的图像更新方法就可以实现视频显示

#### 视频解码实现

> - 到这里，已经有视频播放的功能了，现在需要把视频的解码完成出来
> - 到这里呢，其实建议把视频解码封装成一个单独的功能，使用pri引入即可
> - 这里呢，直接创建一个不带ui文件的pri就可以，我是直接在源文件目录下新建了一个play文件了，新建一个videoplay.pri文件，再修改一下主工程的pro文件，引入即可
> - 在这个子工程中，添加一个纯cpp类，videoDocode,实现视频解码主要功能
> - 视频解码按照上面的流程图实现即可，当然需要先引入FFMpeg的l相关文件，这里建议以外部文件引入，我是放在videoplay.pri目录下，新建一个ffmpeg文件夹，再更改一下pri文件，引入即可

##### 视频解码

> - 引入avformat解封装模块，先来把封装的格式剥去
> - 这里呢因为使用了FFmpeg，建议还是写c风格的代码，省的报错
> - 先定义一个通用的处理错误的函数errHandle
> - 然后在open函数中实现解析视频流，剥去封装格式，然后读取视频流获取信息
> - 然后我们发现需要手动释放一些资源，自己定义一个free函数，先释放解封装上下文，后续还需要释放什么资源自己添加到free函数里就可以了
> - 下面的小逻辑都在代码里以注释的形式表示了，不写了，太累了
> - 这里完了之后，其实就是把视频解码的功能封装好，供下面的读取线程调用，因为都写在子线程的run函数里太复杂了，所以这里单独处理视频解码

##### 视频解码线程

> - 这里为什么需要有一个线程类呢，因为在qt设计中，窗口的控制和各种功能的后台实现应该是不同线程处理的，否则都给窗口线程处理，这个程序会变得异常卡顿
> - 直接创建一个readThread类继承自QThread类，这里基本是按照流程图调用视频解码的函数即可，并返回给窗口线程相应的值,下面要定义几个public的接口供窗口线程调用
> - 主要是处理重写run函数，在cpp文件中引入videoDocode，并实例化对象，调用视频解码功能
> - 先调用videoDocode的open函数，
> - 需要一个自定义信号playState来与窗口线程传递信息，处理播放状态
> - open成功后开始调用videoDocode的read函数
> - 还需要处理视频暂停pause的功能，这里需要先实现sleep延时操作
> - 视频解码是一个相对耗时的操作，不能影响窗口线程，因此解码线程应该是非阻塞延时
> - 成功open和read后，需要关闭线程close了
> - 下面我们来处理窗口线程

#### 窗口线程

> - 在ui文件中绑定控件的槽函数
> - 通过将获取的文件路径显示在combox上，其他控件通过文本值调用，其实是不安全的
> - 页面的具体状态通过自定义的信号与槽与视频解码线程交互
