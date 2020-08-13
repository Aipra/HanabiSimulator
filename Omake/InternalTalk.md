# 烟花模拟器 Omake  
这里是开发团队内部用备忘讨论版

### 13 Aug 15:06  
acr: 考虑到实际问题，开发采用C/C++混合编程，对要在C++中使用的头文件考虑以下格式： 

---
`#ifdef __cplusplus`

`extern "C" {`

`#endif`

`/**** some declaration or so *****/`

`#ifdef __cplusplus`

`}`

`#endif /* end of __cplusplus */`

---  

望周知

