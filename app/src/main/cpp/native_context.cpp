//
// Created by cheng on 2016/12/8.
//

#include "common.h"
#include "native_context.h"

void NativeContext::changeSurface(JNIEnv *env, jobject i_surface, int i_width, int i_height)noexcept
{

    auto window=ANativeWindow_fromSurface(env,i_surface);

    auto r=_commandQueue.addTask([=]{

        _rc.createSurface(window,i_width,i_height);
    });

    _commandQueue.flush();

    _commandQueue.waitFence(r);

    _signal.notifyOne();

    ANativeWindow_release(window);

}

void NativeContext::renderThread() noexcept
{
    for(;;)
    {

        _commandQueue.flush();

        _signal.wait([=]
                     {return !_isPause&&_rc.hasSurface()||!_commandQueue.empty();}
        );

        if(_isPause)
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

            _rc.setAnimation();
        }

        _rc.updateAnimation();
        _rc.draw();
    }
}

void NativeContext::onPauseActivity()noexcept
{
    uint64_t r=_commandQueue.addTask([this]{

        _isPause=true;

    });

    _commandQueue.waitFence(r);
}

void NativeContext::onResumeAcitity()noexcept
{
    _isPause= false;

    _signal.notifyOne();
}

void NativeContext::onDestorySurface() noexcept
{
    _rc.destorySurface();
}


