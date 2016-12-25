//
// Created by cheng on 2016/12/8.
//

#include "common.h"
#include "native_context.h"

void NativeContext::changeSurface(JNIEnv *env, jobject i_surface, int i_width, int i_height)noexcept
{
    _windowWidth=i_width;
    _windowHeight=i_height;

    auto window=ANativeWindow_fromSurface(env,i_surface);

    auto r=_commandQueue.addTask([=]{

        _rc.createSurface(window,i_width,i_height);
    });

    _signal.notifyOne();

    _commandQueue.waitFence(r);

    ANativeWindow_release(window);
}

void NativeContext::renderThread() noexcept
{
    for(;;)
    {
        _signal.wait([=]
                     {return !_isPause&&_rc.hasSurface()||!_commandQueue.empty();}
        );

        _commandQueue.flush();

        if(_isPause || !_rc.hasSurface())
        {
            continue;
        }

        if(!_rc.isHasContext())
        {
            _rc.createContext();

            _rc.makeCurrent();

            _rc.CreateRectProgram();

            _rc.initRectBuffer();

            _rc.initRectBuffer2();

            initRect();

            addAnimation(_rect);

        }

        _animationManager.updateAnimations();
        draw();
    }
}

void NativeContext::onPauseActivity()noexcept
{
    uint64_t r=_commandQueue.addTask([this]{

        _isPause=true;
        _animationManager.pauseAnimations();

    });

    _commandQueue.waitFence(r);
}

void NativeContext::onResumeAcitity()noexcept
{
    _isPause= false;

    _commandQueue.addTask([this]
                          {
                              _animationManager.resumeAnimations();
                          });

    _signal.notifyOne();
}

void NativeContext::onDestorySurface() noexcept
{
    _rc.destorySurface();
}

void NativeContext::draw()noexcept {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _rc.getGLProgam().use();

    _rc.getBuffer1().bind();

    _rc.getGLProgam().bindPos(2, 0);
    _rc.getGLProgam().setColor({1.0f, 0.0f, 0.0f, 1.0f});
    _rc.getGLProgam().setTransform(Matrix3X2());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    _rect->draw(_rc.getBuffer2(),_rc.getGLProgam());

    _rc.getSurface().swap();
}

void NativeContext::initRect() noexcept
{
    _rect->setParentWidthHeight(_windowWidth,_windowHeight);
    _rect->setWidth(_windowWidth/3);
    _rect->setHeight(_windowHeight/2);
    _rect->setCenter((_rect->getWidth())/2,(_rect->getHeight())/2);

    _rect->setInitVertex(20.0f,30.0f);
    _rect->setTranslate(_rect->getInitVertexX(),_rect->getInitVertexY());

}

void NativeContext::addAnimation(shared_ptr<Rect> _rect) noexcept
{

    double time=getTime();
    TranslateAnimation t(_rect,10.0,time,5.0f,5.0f,9.0f,9.0f);
    ScaleAnimation s(_rect,10.0,time,1.0f,0.5f);
    RotateAnimaton r(_rect,10.0,time,0.0f,1.0f);

    _animationManager.addAnimation(make_shared<TranslateAnimation>(t));
    _animationManager.addAnimation(make_shared<ScaleAnimation>(s));
    _animationManager.addAnimation(make_shared<RotateAnimaton>(r));
}





