//
// Created by cheng on 2016/12/16.
//

#include "common.h"
#include "animation.h"

//Animation=======
Animation::Animation(double i_time, double i_postpone)noexcept
{
    _time_run=i_time;
    _time_start=getTime()+i_postpone;
    _time_end=_time_start+_time_run;
}

bool Animation::isEnd()noexcept
{
    return _isEnd;
}


//RotateAnimation======
RotateAnimaton::RotateAnimaton(double i_time, float i_angle, Matrix3X2 *i_matrix, double i_postpone) noexcept:_angle(i_angle),_mat(i_matrix),Animation(i_time,i_postpone)
{
}

void RotateAnimaton::updateAnimation() noexcept
{
    double current_time=getTime();

    float angle;

    if(current_time>= _time_end)
    {
        angle= _angle;

        _isEnd=true;

    } else if(current_time<_time_start)
    {
        angle= 0.0f;
    }else
    {
        double t=current_time-_time_start;

        float f=((float)(t))/_time_run*_angle;

        angle= f;
    }

    *_mat=_mat->rotate(angle-_lastAngle);

    _lastAngle=angle;
}


//ScaleAnimation=======
ScaleAnimation::ScaleAnimation(double i_time, float i_k, Matrix3X2 *i_matrix, double i_postpone) noexcept:_k(i_k),_mat(i_matrix),Animation(i_time,i_postpone)
{
}

void ScaleAnimation::updateAnimation()noexcept
{
    double current_time=getTime();

    float k;

    if(current_time>= _time_end)
    {
        k= _k;

        _isEnd=true;

    } else if(current_time<_time_start)
    {
        k= 1.0f;
    }else
    {
        double t=current_time-_time_start;

        float f=((float)(t))/_time_run*(_k-1.0f);

        k=1.0f+f;
    }

    *_mat=_mat->scale(1.0f/_lastK);
    *_mat=_mat->scale(k);

    dbglog("=====%f===",k);

    _lastK=k;
}

//TransformAnimation
TransformAnimation::TransformAnimation(double i_time, float i_x, float i_y, Matrix3X2 *i_matrix,
                                       double i_postpone)noexcept:_x(i_x),_y(i_y),_mat(i_matrix),Animation(i_time,i_postpone)
{

}

void TransformAnimation::updateAnimation() noexcept
{
    double current_time=getTime();

    float x;
    float y;

    if(current_time>= _time_end)
    {
         x= _x;
         y= _y;

        _isEnd=true;

    } else if(current_time<_time_start)
    {
        x= 0.0f;
        y= 0.0f;
    }else
    {
        double t=current_time-_time_start;

        x=((float)(t))/_time_run*_x;
        y=((float)(t))/_time_run*_y;

    }

    *_mat=_mat->translate(x-_lastX,y-_lastY);

    _lastX=x;
    _lastY=y;
}

//AnimationManager=====
void AnimationManager::addAnimation(const std::shared_ptr<AnimationInterface> &animation)noexcept
{
    _animations.push_back(animation);
}

void AnimationManager::updateAnimations() noexcept
{
    for(auto  a:_animations)
    {
        AnimationInterface & animation=*(a.get());
        animation.updateAnimation();
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






