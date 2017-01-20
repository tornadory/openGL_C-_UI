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
        _rc.setScreenMatrix(i_width,i_height);
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

            addAnimation(_rect,_rect_1,_rect_2);

        }

        _animationManager.updateAnimations(_timerOur.getCurrentTime());
        draw();
    }
}

void NativeContext::onPauseActivity()noexcept
{
    uint64_t r=_commandQueue.addTask([this]{

        _isPause=true;
        _timerOur.setpauseTime(getTime());

    });

    _signal.notifyOne();

    _commandQueue.waitFence(r);
}

void NativeContext::onResumeAcitity()noexcept
{

    double t=getTime();
    uint64_t r=_commandQueue.addTask([this,t]
                          {
                              _timerOur.setResumeTime(t);
                              _isPause= false;

                          });

    _signal.notifyOne();

    _commandQueue.waitFence(r);
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

    _rect->draw(_rc,_rc.getScreenMatrix());

    _rc.getSurface().swap();
}

void NativeContext::initRect() noexcept
{
    _rect->setWidth(_windowWidth/1.5);
    _rect->setHeight(_windowHeight/2);
    _rect->setCenter(0.5f,0.5f);
    _rect->setInitVertex(20.0f,30.0f);
    _rect->setColor({1.0f,1.0f,1.0f,1.0f});

    _rect_1->setWidth(100);
    _rect_1->setHeight(500);
    _rect_1->setCenter(0.5f,0.5f);
    array<float,2> a=_rect->transformPosition(20.0f,40.0f);
    _rect_1->setInitVertex(a[0],a[1]);
    _rect_1->setColor({0.0f,0.0f,1.0f,1.0f});

    _rect_2->setWidth(200);
    _rect_2->setHeight(400);
    _rect_2->setCenter(0.5f,0.5f);
    a=_rect->transformPosition(150.0f,40.0f);
    _rect_2->setInitVertex(a[0],a[1]);
    _rect_2->setColor({0.0f,1.0f,0.0f,1.0f});

    _rect->addRectChild(_rect_1);
    _rect->addRectChild(_rect_2);

    _rect.get()->setTouchListener(this);
}

void NativeContext::addAnimation(shared_ptr<Rect> _rect,shared_ptr<Rect> _rect1,shared_ptr<Rect> _rect2) noexcept
{

    double time=TimerOur().getCurrentTime();
    TranslateAnimation t(_rect,10.0,time,50.0f,50.0f,90.0f,90.0f);
    ScaleAnimation s(_rect,10.0,time,1.0f,0.5f);
    RotateAnimaton r(_rect,10.0,time,0.0f,1.0f);

    TranslateAnimation t1(_rect,10.0,time,50.0f,50.0f,90.0f,90.0f);
    ScaleAnimation s1(_rect1,10.0,time,1.0f,0.5f);
    RotateAnimaton r1(_rect1,10.0,time,0.0f,1.0f);

    TranslateAnimation t2(_rect,10.0,time,50.0f,50.0f,90.0f,90.0f);
    ScaleAnimation s2(_rect2,10.0,time,1.0f,0.5f);
    RotateAnimaton r2(_rect2,10.0,time,0.0f,1.0f);


//    _animationManager.addAnimation(make_shared<TranslateAnimation>(t));
//    _animationManager.addAnimation(make_shared<ScaleAnimation>(s));
//    _animationManager.addAnimation(make_shared<RotateAnimaton>(r));
//
//    _animationManager.addAnimation(make_shared<TranslateAnimation>(t1));
//    _animationManager.addAnimation(make_shared<ScaleAnimation>(s1));
//    _animationManager.addAnimation(make_shared<RotateAnimaton>(r1));
//
//    _animationManager.addAnimation(make_shared<TranslateAnimation>(t2));
//    _animationManager.addAnimation(make_shared<ScaleAnimation>(s2));
//    _animationManager.addAnimation(make_shared<RotateAnimaton>(r2));
}

void NativeContext::onPointerDown(int i_point_id, float i_x, float i_y) noexcept
{
    _commandQueue.addTask([=]{

        if(_rect)
        {
            Rect * rect= _rect->onPointerDown(i_point_id, i_x, i_y);

            if(rect)
            {
                _rectTouch=rect;
            }

            dbglog("NC=onPointerDown===%d===%f===%f===",i_point_id,i_x,i_y);

        }

    });
}

void NativeContext::onPointerUp(int i_point_id, float i_x, float i_y)noexcept
{
    _commandQueue.addTask([=]{

        if(_rectTouch)
        {
            _rectTouch->onPointerUp(i_point_id, i_x, i_y);
        }

        dbglog("NC=onPointerUp===%d===%f===%f===",i_point_id,i_x,i_y);
    });
}

void NativeContext::onPointerMoved(int i_point_id, float i_x, float i_y) noexcept
{
    _commandQueue.addTask([=]{

        if(_rectTouch)
        {
            _rectTouch->onPointerMoved(i_point_id, i_x, i_y);

            dbglog("NC==onPointerMoved===%d===%f===%f===",i_point_id,i_x,i_y);
        }

    });
}

void NativeContext::touchMove(int i_point_id, float i_dx, float i_dy) noexcept
{

    int point_id=-1;

    float distance=0.0f;
    float distanceInit=0.0f;

    for(auto i=_point_map.cbegin();i!=_point_map.cend();i++)
    {
        int a=i->first;

        if(i_point_id==a)
        {
            if(abs((i->second)[0]-i_dx)>=5||abs((i->second)[1]-i_dy)>=5)
            {
                _move_map.insert({i_point_id,{i_dx,i_dy}}).first->second={i_dx,i_dy};
                point_id=i_point_id;

                dbglog("touchListenerMove====%d===%f===%f===",i_point_id,i_dx,i_dy);
            }
        }
    }

    if(point_id!=-1)
    {
        if(_point_map.size()>=2)
        {
            for(auto i=_point_map.begin();i!=_point_map.end();++i)
            {
                float init_x=i->second[0];
                float init_y=i->second[1];

                if(++i!=_point_map.end())
                {
                    float minusX=i->second[0]-init_x;
                    float minusY=i->second[1]-init_y;

                    distanceInit=distanceInit+sqrt(minusX*minusX+minusY*minusY);
                }

                --i;
            }

            for(auto i=_move_map.begin();i!=_move_map.end();++i)
            {
                float init_x=i->second[0];
                float init_y=i->second[1];

                if(++i!=_point_map.end())
                {
                    float minusX=i->second[0]-init_x;
                    float minusY=i->second[1]-init_y;

                    distance=distance+sqrt(minusX*minusX+minusY*minusY);
                }

                --i;
            }

            _rect_2.get()->setScale(distance/distanceInit,distance/distanceInit);
        }
    }

//    _rect_2.get()->setTranslate(i_dx-_down_x+_rect_2->getInitTranslateX(),i_dy-_down_y+_rect_2->getInitTranslateY());
}

void NativeContext::touchDown(int i_point_id, float i_dx, float i_dy) noexcept
{
    _down_x=i_dx;
    _down_y=i_dy;

    addPoint(i_point_id,i_dx,i_dy);

    dbglog("touchListenerdown====%d===%f===%f===",i_point_id,i_dx,i_dy);
}

void NativeContext::touchUp(int i_point_id, float i_dx, float i_dy) noexcept
{
    removePoint(i_point_id,i_dx,i_dy);

    if(_point_map.size()==0)
    {
        _rectTouch= nullptr;
    }

    dbglog("touchListenerup====%d===%f===%f===",i_point_id,i_dx,i_dy);

//    _rect_2.get()->setInitTranslate(i_dx-_down_x+_rect_2->getInitTranslateX(),i_dy-_down_y+_rect_2->getInitTranslateY());
}

void NativeContext::addPoint(int i_pointId, float i_x, float i_y)noexcept
{
    _point_map.insert({i_pointId,{i_x,i_y}});
    _move_map.insert({i_pointId,{i_x,i_y}});
}

void NativeContext::removePoint(int i_pointId, float i_x, float i_y)noexcept
{
    _point_map.erase(i_pointId);
    _move_map.erase(i_pointId);
}





















