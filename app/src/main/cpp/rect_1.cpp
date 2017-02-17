//
// Created by cheng on 2016/12/22.
//

#include "common.h"
#include "rect_1.h"
#include "transform_mat.h"

void Rect::setRotate(float i_x, float i_y)noexcept
{
    _rotate[0]=i_x;
    _rotate[1]=i_y;
}

void Rect::setScale(float i_kx, float i_ky)noexcept
{
    _scale[0]=i_kx;
    _scale[1]=i_ky;
}

void Rect::setTranslate(float i_x, float i_y) noexcept
{
    _translate[0]=i_x;
    _translate[1]=i_y;
}

void Rect::setInitVertex(float i_x, float i_y) noexcept
{
    _translate[0]=i_x+_center[0]*_width;
    _translate[1]=i_y+_center[1]*_height;
}

void Rect::setWidth(float i_width) noexcept
{
    _width=i_width;
}

void Rect::setHeight(float i_height) noexcept
{
    _height=i_height;
}

void Rect::setCenter(float i_x, float i_y)noexcept
{
    _center[0]=i_x;
    _center[1]=i_y;
}

float Rect::getWidth() noexcept
{
    return _width;
}

float Rect::getHeight() noexcept
{
    return _height;
}

float Rect::getCenterX() noexcept
{
    return _center[0];
}

float Rect::getCenterY() noexcept
{
    return _center[1];
}

void Rect::setColor(array<float, 4> i_color) noexcept
{
    _color=i_color;
}

void Rect::setTouchListener(TouchListener *i_touchListener) noexcept
{
    _touchListener=i_touchListener;
}

float Rect::getTranslateX() noexcept
{
    return _translate[0];
}

float Rect::getTranslateY() noexcept
{
    return _translate[1];
}

float Rect::getScaleX() noexcept
{
    return _scale[0];
}

float Rect::getScaleY() noexcept
{
    return _scale[1];
}

float Rect::getCos() noexcept
{
    return _rotate[0];
}

float Rect::getSin()noexcept
{
    return _rotate[1];
}

void Rect::addRectChild(shared_ptr<Rect> i_rect)noexcept
{
    _rectChildren.push_back(i_rect);
}
void Rect::draw(RenderContext &i_rendContext, Matrix3X2 i_matix) noexcept
{

    /**
      * buffer
      * {
      *  0.0f,0.0f,
      *  0.0f,1.0f,
      *  1.0f,0.0f,
      *  1.0f,1.0f
      * }
      */
    Matrix3X2 mat  //buffer *mat 变成 四个buffer对用的屏幕坐标系的坐标
            {
                    _width,0.0f,
                    0.0f,_height,
                    0, 0

            };

    mat=mat.translate(-_width*_center[0],-_height*_center[1]); //平移到原点，再进行缩放和旋转动画

    Matrix3X2 mat2=Matrix3X2::scaling(_scale[0],_scale[1]); //缩放动画
    mat2=mat2.rotate(_rotate[0],_rotate[1]); //旋转动画
    mat2=mat2.translate(_translate[0],_translate[1]);//位移动画

    //
    mat2=mat2*(i_matix);

    i_rendContext.getBuffer2().bind();

    i_rendContext.getGLProgam().bindPos(2,0);
    i_rendContext.getGLProgam().setColor(_color);
    i_rendContext.getGLProgam().setTransform(mat*mat2);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    for(auto i:_rectChildren)
    {
        i->draw(i_rendContext,mat2);
    }
}

array<float, 2> Rect::transformPosition(float i_x, float i_y) noexcept
{
    return { -_width * _center[0] + i_x, -_height * _center[1] + i_y };

}

//Rect * Rect::onPointerDown(map<int,array<float,4>> &i_point_map, int i_point_id, float i_x, float i_y)noexcept
//{
//    auto mat = Matrix3X2::translation(-this->_translate[0],-this->_translate[1]).rotate(this->_rotate[0],-this->_rotate[1]).scale(1/this->_scale[0],1/this->_scale[1]);
//
//    this->_inverseMatrix=mat;
//
//    auto p = mat.transformPoint({i_x, i_y});
//
//    float x = p[0] + _center[0] * _width;
//    float y = p[1] + _center[1] * _height;
//
//    if(x >= 0 && x <= _width && y >= 0 && y <= _height)
//    {
//
//        for(auto &i : _rectChildren)
//        {
//
//            if (i->onPointerDown(i_point_map, i_point_id, p[0], p[1]))
//            {
//                return i.get();
//            }
//        }
//
////        this->pointerDown(x, y);
//
//
//        if(_touchListener!= nullptr)
//        {
//            dbglog("=====onPointerDown=%d=%f==%f===%f==",i_point_id,_width,p[0],p[1]);
//
//            array<float,4>& a=i_point_map.find(i_point_id)->second;
//            a[2]=p[0];
//            a[3]=p[1];
//
//            this->_touchListener->touchEvent(1,i_point_id);
//
//        }
//
//        return this;
//
//    } else
//    {
//        return nullptr;
//    }
//
//}
//
//void Rect::onPointerUp(int i_point_id)noexcept
//{
////    auto p=this->_inverseMatrix.transformPoint({i_x, i_y});
//
//    if(_touchListener!= nullptr)
//    {
//        _touchListener->touchEvent(3,i_point_id);
//
//        dbglog("=====onPointerUp=%d=%f==",i_point_id,_width);
//    }
//}
//
//void Rect::onPointerMoved(map<int,array<float,4>> &i_point_map)noexcept
//{
//
//    if(_touchListener!= nullptr)
//    {
//
//        for(auto i=i_point_map.begin();i!=i_point_map.end();++i)
//        {
//            auto p=this->_inverseMatrix.transformPoint({i->second[0], i->second[1]});
//
//            i->second[2]=p[0];
//            i->second[3]=p[1];
//
//            dbglog("=====onPointerMoved=%d=%f==%f===%f==",i->first,_width,p[0],p[1]);
//        }
//
//        _touchListener->touchEvent(2);
//
//    }
//}

TouchListener *Rect::getTouchListener()noexcept
{
    return _touchListener;
}

Matrix3X2 Rect::getInverseMatrix()noexcept
{
    return _inverseMatrix;
}

void Rect::setInverseMatrix(Matrix3X2 i_matrix)noexcept
{
    _inverseMatrix=i_matrix;
}


//4-4-6 事件分发
/**
 * 根据 x,y 得到从 根路径到最上层的所有的rect
 */
bool Rect::getRectAt(shared_ptr<Rect> &i_rect, float i_x, float i_y,
                     vector<shared_ptr<Rect>> &i_route)noexcept
{

    auto mat = Matrix3X2::translation(-this->_translate[0],-this->_translate[1]).rotate(this->_rotate[0],-this->_rotate[1]).scale(1/this->_scale[0],1/this->_scale[1]);

    auto p = mat.transformPoint({i_x, i_y});

    float x = p[0] + _center[0] * _width;
    float y = p[1] + _center[1] * _height;

    if(x >= 0 && x <= _width && y >= 0 && y <= _height)
        {

        i_route.push_back(i_rect);
        for(auto &i : _rectChildren)
        {

            if (i->getRectAt(i, p[0], p[1], i_route))
            {
                return true;
            }
        }

        return true;

    } else
    {
        return false;
    }
}

/**
 * 是否拦截事件，默认不拦截
 */
bool Rect::onInterceptTouchEvent(map<int, array<float, 4>> &i_point_map, int i_point_id,
                                 int i_event_type)noexcept
{
    return false;
}

/**
 *
 */
bool Rect::touchEvent(map<int,array<float, 4>> &i_point_map, int i_event_type, int i_point_id) noexcept
{

    if(this->getWidth()==200)
    {
        return false;
    }
    return true;
}































