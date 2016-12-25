//
// Created by cheng on 2016/12/16.
//

#include "common.h"
#include "animation.h"

//AnimationManager=====
void AnimationManager::addAnimation(const std::shared_ptr<AnimationInterface> &animation)noexcept
{
    _animations.push_back(animation);
}

void AnimationManager::updateAnimations() noexcept
{
    for(auto  a:_animations)
    {
        double current_time=getTime();

        AnimationInterface & animation=*(a.get());
        animation.updateAnimation(current_time);
    }

    removeAnimations();
}

void AnimationManager::removeAnimations()noexcept
{
    for(auto a=_animations.begin();a!=_animations.end();)
    {
        auto b = a;

        ++a;

        if((*b)->isEnd())
        {
            _animations.erase(b);
        }
    }
}

void AnimationManager::pauseAnimations() noexcept
{

    for(auto  a:_animations)
    {
        double current_time=getTime();

        AnimationInterface & animation=*(a.get());
        animation.setpauseTime(current_time);
    }

}

void AnimationManager::resumeAnimations() noexcept
{

    for(auto  &a:_animations)
    {
        double current_time=getTime();

        AnimationInterface & animation=*(a.get());
        animation.setResumeTime(current_time);
    }

}


//RectAnimation===
RectAnimation::RectAnimation(shared_ptr<Rect> i_rect, double i_time,double i_start_time)noexcept:_rect(i_rect)
{
    _time_run=i_time;
    _time_start=i_start_time;
    _time_end=_time_start+_time_run;
}

bool RectAnimation::isEnd()noexcept
{
    return _isEnd;
}

void RectAnimation::setpauseTime(double i_time) noexcept
{
    _pause_time=i_time;
}

void RectAnimation::setResumeTime(double i_time) noexcept
{
    if(_pause_time!=0.0f)
    {
        _resume_time=i_time;

        double t=_resume_time-_pause_time;
        _pause_total_time=_pause_total_time+t;

        _time_end=_time_end+t;

        _pause_time=0.0f;
        _resume_time=0.0f;
    }

}


//RetateAnimation===============ok====2016年12月25日
RotateAnimaton::RotateAnimaton(shared_ptr<Rect> i_rect, double i_time, double i_start_time,
                               float i_start_angle, float i_end_angle) noexcept:RectAnimation(i_rect,i_time,i_start_time),_startAngle(i_start_angle),_endAngle(i_end_angle)
{

}

void RotateAnimaton::updateAnimation( double i_current_time ) noexcept
{
    double _has_run_animation_time=i_current_time-_pause_total_time;//减去暂停的时间

    float angle;

    if(i_current_time>= _time_end)
    {
        angle= _endAngle;

        _isEnd=true;

    } else if(i_current_time<_time_start)
    {
        angle= _startAngle;
    }else
    {
        double t=_has_run_animation_time-_time_start;

        float f=((float)(t))/_time_run*(_endAngle-_startAngle);

        angle= f+_startAngle;
    }

    angle = fmodf(angle, 1.f);

    _rect->setRotate(cosf(angle*2*(float)_M_PI),sinf(angle*2*(float)_M_PI));
}

//ScaleAnimation===============ok====2016年12月25日
ScaleAnimation::ScaleAnimation(shared_ptr<Rect> i_rect, double i_time, double i_start_time,
                               float i_startyKx, float i_startKy, float i_endKx, float i_endKy) noexcept :RectAnimation(i_rect,i_time,i_start_time),_startKx(i_startyKx),_startKy(i_startKy),_endKx(i_endKx),_endKy(i_endKy)
{

}

ScaleAnimation::ScaleAnimation(shared_ptr<Rect> i_rect, double i_time, double i_start_time,
                                float i_startX, float i_endY) noexcept :ScaleAnimation(i_rect,i_time,i_start_time,i_startX,i_startX,i_endY,i_endY)
{

}

void ScaleAnimation::updateAnimation(double i_current_time ) noexcept
{
    double _has_run_animation_time=i_current_time-_pause_total_time;

    float kx;
    float ky;

    if(i_current_time>= _time_end)
    {
        kx= _endKx;
        ky= _endKy;

        _isEnd=true;

    } else if(i_current_time<_time_start)
    {
        kx= _startKx;
        ky= _startKy;
    }else
    {
        double t=_has_run_animation_time-_time_start;

        kx=((float)(t))/_time_run*(_endKx-_startKx)+_startKx;
        ky=((float)(t))/_time_run*(_endKy-_startKy)+_startKx;

    }

    _rect->setScale(kx,ky);
}

//TransformAnimation========ok====2016年12月24日
TranslateAnimation::TranslateAnimation(shared_ptr<Rect> i_rect,double i_time,double i_start_time,float i_startX,float i_startY,float i_endX,float i_endY)noexcept:RectAnimation(i_rect,i_time,i_start_time),_startX(i_startX),_startY(i_startY),_endX(i_endX),_endY(i_endY)
{

}

void TranslateAnimation::updateAnimation(double i_current_time )noexcept
{
    double _has_run_animation_time=i_current_time-_pause_total_time;

    float x;
    float y;

    if(i_current_time>= _time_end)
    {
        x= _endX;
        y= _endY;

        _isEnd=true;

    } else if(i_current_time<_time_start)
    {
        x= _startX;
        y= _startY;
    }else
    {
        double t=_has_run_animation_time-_time_start;

        x=((float)(t))/_time_run*(_endX-_startX)+_startX;
        y=((float)(t))/_time_run*(_endY-_startY)+_startY;

    }

    _rect->setTranslate(x, y);
}





