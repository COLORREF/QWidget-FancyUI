#ifndef COMPOSITIONWINDOWEFFECT_H
#define COMPOSITIONWINDOWEFFECT_H
#include "windowmanager.h"

class CompositionWindowEffect : protected WindowManager
{
public:

    static void removeCompositionWindowEffect(const HWND hWnd);//移除下方的函数设置的窗口效果

    static void setFollowingSystemColor(const HWND hwnd);//跟随系统颜色

    static void setAreoEffect(const HWND hwnd); //Areo

    static void setAreoEffect(const HWND hwnd,COLORREF abgr);//Areo,16进制颜色,每两位表示一个像素分量//使用ABGR宏生成颜色

    static void setClientAreaFullyTransparent(const HWND hwnd);//客户区全透明//即不包括标题栏
};

#endif // COMPOSITIONWINDOWEFFECT_H
