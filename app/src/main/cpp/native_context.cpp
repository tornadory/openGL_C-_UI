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
    _rect->setWidth(_windowWidth/1.1);
    _rect->setHeight(_windowHeight/1.1);
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

    _rect->setTouchListener(this);
}

void NativeContext::addAnimation(shared_ptr<Rect> _rect,shared_ptr<Rect> _rect1,shared_ptr<Rect> _rect2) noexcept
{

    double time=TimerOur().getCurrentTime();
    TranslateAnimation t(_rect,10.0,time,50.0f,50.0f,90.0f,90.0f);
    ScaleAnimation s(_rect,10.0,time,1.0f,0.5f);
    RotateAnimaton r(_rect,10.0,time,0.0f,0.8f);

    TranslateAnimation t1(_rect,10.0,time,50.0f,50.0f,90.0f,90.0f);
    ScaleAnimation s1(_rect1,10.0,time,1.0f,0.5f);
    RotateAnimaton r1(_rect1,10.0,time,0.0f,1.0f);

    TranslateAnimation t2(_rect,10.0,time,50.0f,50.0f,90.0f,90.0f);
    ScaleAnimation s2(_rect2,10.0,time,1.0f,0.5f);
    RotateAnimaton r2(_rect2,10.0,time,0.0f,0.6f);


    _animationManager.addAnimation(make_shared<TranslateAnimation>(t));
    _animationManager.addAnimation(make_shared<ScaleAnimation>(s));
    _animationManager.addAnimation(make_shared<RotateAnimaton>(r));

    _animationManager.addAnimation(make_shared<TranslateAnimation>(t1));
    _animationManager.addAnimation(make_shared<ScaleAnimation>(s1));
    _animationManager.addAnimation(make_shared<RotateAnimaton>(r1));

    _animationManager.addAnimation(make_shared<TranslateAnimation>(t2));
    _animationManager.addAnimation(make_shared<ScaleAnimation>(s2));
    _animationManager.addAnimation(make_shared<RotateAnimaton>(r2));
}

void NativeContext::onPointerDown(int i_point_id) noexcept // mark status之类应该是listener的逻辑
{
    _commandQueue.addTask([=]{

        if(_rect)
        {
            auto p=_point_map.find(i_point_id)->second;
            if(_rectTouch)
            {
                _rectTouch->onPointerDown(_point_map, i_point_id, p[0], p[1]);
            }
            else
            {
                Rect * rect= _rect->onPointerDown(_point_map, i_point_id, p[0], p[1]);//层级

                if(rect)
                {
                    _rectTouch=rect;
                }
            }

        }

    });
}

void NativeContext::onPointerMoved() noexcept
{
    _commandQueue.addTask([=]{

        if(_rectTouch)
        {
            _rectTouch->onPointerMoved(_point_map);
        }

    });
}

void NativeContext::onPointerUp(int i_point_id)noexcept
{
    _commandQueue.addTask([=]{

        if(_rectTouch)
        {
            _rectTouch->onPointerUp(i_point_id);
        }

        removePoint(i_point_id);

        if(_status==-1)
        {
            _rectTouch= nullptr;
        }
    });
}


void NativeContext::touchDown(int i_point_id) noexcept
{
    switch (_status)
    {
        case -1: //当前没有手指头按下。按下第一个手指头
            _status=1;
            _pen_handler_id=i_point_id;
            break;
        case 1:  //当前有一个手指头按下。按下第二个手指头
            _status=2;
            _scale_handler_ids[0]=_pen_handler_id;
            _pen_handler_id=-1;
            _scale_handler_ids[1]=i_point_id;
            break;
        case 2:
            break;
    }

    auto p=_point_map.find(i_point_id)->second;

    if(_status==1)
    {
        _down_x=p[2];
        _down_y=p[3];

        _offset_x=_rect_2->getTranslateX();
        _offset_y=_rect_2->getTranslateY();

    }else if(_status==2)
    {

        auto p=_point_map.find(_scale_handler_ids[0])->second;
        auto p2=_point_map.find(_scale_handler_ids[1])->second;

        _startPoints={p[2],p[3],p2[2],p2[3]};

        _distance=getCurrentDistance();
        _scaleRadio=_rect_2->getScaleX();
        _cosA=_rect_2->getCos();
        _sinA=_rect_2->getSin();

    }
}

void NativeContext::touchMove() noexcept
{

    if(_status==2)
    {
        float scale=_scaleRadio*getCurrentDistance()/_distance;
        _rect_2.get()->setScale(scale,scale);

        array<float,4> arr= getcurrentPoints();
        float cosb=getCosa(_startPoints,arr);
        float sinb=getSina(_startPoints,arr);
        float cos=cosAB(_cosA,_sinA,cosb,sinb);
        float sin=sinAB(_cosA,_sinA,cosb,sinb);
        _rect_2.get()->setRotate(cos,sin);
    }
    else if(_status==1)
    {
        auto p=_point_map.find(_pen_handler_id)->second;

        _rect_2->setTranslate(p[2]-_down_x+_offset_x,p[3]-_down_y+_offset_y);
    }
}

void NativeContext::touchUp(int i_point_id) noexcept
{

    if(_status==2)
    {
        if(_scale_handler_ids[0]==i_point_id)
        {
            auto p=_point_map.find(_scale_handler_ids[1])->second;

            _down_x=p[2];
            _down_y=p[3];

            _pen_handler_id=_point_map.find(_scale_handler_ids[1])->first;

        } else
        {
            auto p=_point_map.find(_scale_handler_ids[0])->second;

            _down_x=p[2];
            _down_y=p[3];

            _pen_handler_id=_point_map.find(_scale_handler_ids[0])->first;
        }

        _offset_x=_rect_2->getTranslateX();
        _offset_y=_rect_2->getTranslateY();

        _scale_handler_ids[0]=-1;
        _scale_handler_ids[1]=-1;

        _scaleRadio=1.0f;
        _cosA=1.0f;
        _sinA=1.0f;
        _distance=0.0f;

    }else if(_status==1)
    {
        _down_x=0.0f;
        _down_y=0.0f;
    }

    switch (_status)
    {
        case -1: //当前没有手指头按下
            break;
        case 1:  //当前有一个手指头按下
            _status=-1;
            _pen_handler_id=-1;
            break;
        case 2: // 当前有两个手指头按下
            _status=1;

            break;
    }
}

void NativeContext::addPoint(int i_pointId, float i_x, float i_y)noexcept
{
    _point_map[i_pointId] = {i_x,i_y,0.0f,0.0f};
    //_point_map.insert({i_pointId,{i_x,i_y,0.0f,0.0f}}); // mark operator[]
}

void NativeContext::removePoint(int i_pointId)noexcept
{
    _point_map.erase(i_pointId);
}

float NativeContext::getCurrentDistance() noexcept
{
    auto p=_point_map.find(_scale_handler_ids[0])->second;
    auto p2=_point_map.find(_scale_handler_ids[1])->second;

    float x=p[2]- p2[2];
    float y=p[3]- p2[3];

    return sqrt(x*x+y*y);

}

void NativeContext::setPoint(int i_point_id, float i_x, float i_y)noexcept
{
    if(_point_map.count(i_point_id)==0)
    {
        addPoint(i_point_id,i_x,i_y);
    } else
    {
//       auto a= _point_map.find(i_point_id);   // TODO  修改成  auto a= _point_map.find(i_point_id)->second 不能起作用; 用
//        a->second[0]=i_x;
//        a->second[1]=i_y;

        array<float,4>& a=_point_map.find(i_point_id)->second;
        a[0]=i_x;
        a[1]=i_y;
    }
}

array<float, 4> NativeContext::getcurrentPoints()noexcept
{
    auto p=_point_map.find(_scale_handler_ids[0])->second;
    auto p2=_point_map.find(_scale_handler_ids[1])->second;

    return { p[2],p[3],p2[2],p2[3] };
}

//sin(a+b)=sin(a)•cos(b)+cos(a)•sin(b)
float NativeContext::sinAB(float i_cosa,float i_sina,float i_cosb,float i_sinb)noexcept
{
    return i_sina*i_cosb+i_cosa*i_sinb;
}

//cos(a+b)=cosacosb-sinasinb
float NativeContext::cosAB(float i_cosa,float i_sina,float i_cosb,float i_sinb)noexcept
{
    return  i_cosa*i_cosb-i_sina*i_sinb;
}

//cos=a*b/[|a|*|b|]=(x1x2+y1y2)/[√[x1^2+y1^2]*√[x2^2+y2^2]]
float NativeContext::getCosa(array<float, 4> i_start, array<float, 4> i_end) noexcept // mark static utils
{
    float x1= i_start[2]-i_start[0];
    float y1= i_start[3]-i_start[1];

    float x2= i_end[2]-i_end[0];
    float y2= i_end[3]-i_end[1];

    return (x1*x2+y1*y2)/(sqrt(x1*x1+y1*y1)*sqrt(x2*x2+y2*y2));
}

//sin=()/(x1y2-x2y1)/[√[x1^2+y1^2]*√[x2^2+y2^2]
float NativeContext::getSina(array<float, 4> i_start, array<float, 4> i_end) noexcept
{
    float x1= i_start[2]-i_start[0];
    float y1= i_start[3]-i_start[1];

    float x2= i_end[2]-i_end[0];
    float y2= i_end[3]-i_end[1];

    return (x1*y2-x2*y1)/(sqrt(x1*x1+y1*y1)*sqrt(x2*x2+y2*y2));
}

