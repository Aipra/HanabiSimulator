# 烟花模拟器 Omake  
这里是开发团队内部用备忘讨论版

### 13 Aug 15:06  
acr
> 考虑到实际问题，开发采用C/C++混合编程，对要在C++中使用的头文件考虑以下格式： 
>
>---
>`#ifdef __cplusplus`
>
>`extern "C" {`
>
>`#endif`
>
>`/**** some declaration or so *****/`
>
>`#ifdef __cplusplus`
>
>`}`
>
>`#endif /* end of __cplusplus */`
>
>---  
>
>望周知

### 13 Aug 15:25

acr
>因为不想让只有两个人的开发团队背负编程，美术，编曲和策划四大项学习任务，Alpha版只能阉了又阉(捂脸
>
>目前该游戏为伪初学者和初学者的实验基地，开发完毕遥遥无期，alpha版预期最佳只能以没有BGM、没有立绘、其他音乐美术素材皆为免费素材、只能看烟花结尾（真 烟花模拟器，因此把更多的精力放到无尽模式的开发上可能能获得更好的效果。

### 13 Aug 22:02

acr
>翻了翻openGL的入门教程，发现这个东西真的是一言难尽(捂脸  
>
>OpenGL真不愧是底层的API，除了绘图的函数啥都没有(摔，Alpha版全功能实现遥遥无期啊。  
>
>那么剩下的问题就好说了，一个轮子一个轮子慢慢造吧
>
>> 草查了下发现这玩意是真麻烦，要不Alpha版改成弹幕游戏引擎得了QAQ  
>>目前看至少要把计算机图形学、游戏引擎、OpenGL的资料看完，我还是咕一会吧
>
>码代码码累了写下大概思路，备忘  
>资料汇总  
>OpenGL相关  
>[OpenGL入门学习](http://www.cppblog.com/doing5552/archive/2009/01/08/71532.html)  
>[最好的OpenGL教程之一](https://www.bilibili.com/video/BV1MJ411u7Bc)  
>*OpenGL编程指南* 已更新至第八版(2014)  
>
>东方原作相关  
>[游戏攻略/STG判定数据](https://thwiki.cc/%E6%B8%B8%E6%88%8F%E6%94%BB%E7%95%A5/STG%E5%88%A4%E5%AE%9A%E6%95%B0%E6%8D%AE)  
>[游戏攻略/STG机体说明](https://thwiki.cc/%E6%B8%B8%E6%88%8F%E6%94%BB%E7%95%A5/STG%E6%9C%BA%E4%BD%93%E8%AF%B4%E6%98%8E)  
>[脚本对照表/ECL](https://thwiki.cc/%E8%84%9A%E6%9C%AC%E5%AF%B9%E7%85%A7%E8%A1%A8/ECL)  
>
> @Aipra,最好先把分工做好，另外只写讨论版也好，至少给我露个面
>
>+ 绘制部分
>
>   + UI绘制
>
>   + Stage背景绘制
>
>   + 自机绘制
>
>   + 敌机绘制
>
>   + 弹幕绘制 
>
>+ 地图部分
>
>+ 符卡部分