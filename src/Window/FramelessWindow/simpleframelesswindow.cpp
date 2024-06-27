#include "simpleframelesswindow.h"
#include "simpletitlebar.h"
#include "maximizebutton.h"
#include "standardtitlebar.h"

SimpleFramelessWindow::SimpleFramelessWindow(QWidget *parent)
    : FramelessWindowBase(parent, Type::Simple),
      maximize_button((MaximizeButton *)((SimpleTitleBar *)this->_titleBar)->maximizeButton())
{
}

void SimpleFramelessWindow::deleteTitleBar()
{
    FramelessWindowBase::deleteTitleBar();
    if (this->maximize_button)
        this->maximize_button = nullptr;
}

SimpleFramelessWindow::SimpleFramelessWindow(QWidget *parent, Type type)
    : FramelessWindowBase(parent, type)
{
    switch (type)
    {
    case Type::Base:
        break;
    case Type::Simple:
        this->maximize_button = (MaximizeButton *)((SimpleTitleBar *)this->_titleBar)->maximizeButton();
        break;
    case Type::Standard:
        this->maximize_button = (MaximizeButton *)((StandardTitleBar *)this->_titleBar)->maximizeButton();
        break;
    }
}

F_DEFINITION_NATIVEEVENT(SimpleFramelessWindow)
{
    if (this->maximize_button)
    {
        MSG *msg = (MSG *)message;
        switch (msg->message)
        {
        case WM_NCHITTEST:
        {
            POINT p{GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam)}; // 获取鼠标的绝对坐标
            if (!::MapWindowPoints(HWND_DESKTOP, msg->hwnd, &p, 1))        // 将p转换成相对坐标
            {
                if (!::ScreenToClient((HWND)winId(), &p))
                {
                    QPoint t(this->mapFromGlobal(QPoint(p.x, p.y)));
                    p.x = t.x();
                    p.y = t.y();
                }
            }
            QPoint widget_point(p.x, p.y);                                                   // Qt相对坐标
            widget_point /= this->_dpi;                                                             // 除以缩放dpi

            QRect max_btn_rect(maximize_button->mapTo(this,QPoint(0,0)),maximize_button->size());// 最大化按钮区域
            if (max_btn_rect.contains(widget_point))                                         // 鼠标在最大化按钮内
            {
                *result = HTMAXBUTTON;
                if (this->maximize_button->isPressed()) // 鼠标按下
                    emit this->maximize_button->mouseMove(true);
                else
                    emit this->maximize_button->enter();
                return true;
            }
        }
        break;
        case WM_NCMOUSELEAVE: // 光标从非工作区离开
            if (this->maximize_button->isPressed())
                emit this->maximize_button->mouseMove(false);
            else
                emit this->maximize_button->leave();
            break;
        case WM_NCLBUTTONDBLCLK: // 鼠标左键位于窗口非工作区双击时发出
        case WM_NCLBUTTONUP:     // 鼠标左键非客户区松开,相当于点击最大化按钮
            if (msg->wParam == HTMAXBUTTON)
            {
                emit this->maximize_button->leave();
                if (this->isMaximized())
                    this->showNormal();
                else
                    this->showMaximized();
                *result = 0;
                return true;
            }
            break;
        case WM_LBUTTONUP: // 客户区鼠标左键松开
            if (this->maximize_button->isPressed())
                emit this->maximize_button->leave();
            break;
        case WM_NCLBUTTONDOWN: // 鼠标左键位于窗口非工作区按下时发出
            if (msg->wParam == HTMAXBUTTON)
            {
                *result = 0;
                emit this->maximize_button->mousePress();
                return true;
            }
            break;
        default:
            break;
        }
    }
    return FramelessWindowBase::nativeEvent(eventType, message, result);
}

F_DEFINITION_CHANEGEVENT(SimpleFramelessWindow)
{
    if (event->type() == QEvent::Type::WindowStateChange)
    {
        QWindowStateChangeEvent *stateChangeEvent = static_cast<QWindowStateChangeEvent *>(event);
        // 最大化后最小化
        if (stateChangeEvent->oldState() & Qt::WindowState::WindowMaximized && this->windowState() & Qt::WindowState::WindowMinimized)
            return QWidget::changeEvent(event);
        // 最大化
        if (this->windowState() & Qt::WindowState::WindowMaximized)
        {
            emit this->maximize_button->stateChange(MaximizeButton::State::MAXIMIZE);
            this->_verticalLayout->setContentsMargins(
                this->X_BORDER_WIDTH,
                this->Y_BORDER_WIDTH,
                this->X_BORDER_WIDTH,
                this->Y_BORDER_WIDTH);
            this->update();
        }
        // 最大化后还原
        if (stateChangeEvent->oldState() & Qt::WindowState::WindowMaximized && !(this->windowState() & Qt::WindowState::WindowMaximized))
        {
            emit this->maximize_button->stateChange(MaximizeButton::State::NORMAL);
            this->_verticalLayout->setContentsMargins(0,0,0,0);
            this->update();
        }
    }
    QWidget::changeEvent(event);
}
