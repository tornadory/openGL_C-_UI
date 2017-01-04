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

bool Rect::onPointerDown(float i_x, float i_y, Matrix3X2 i_parent_inverse_matrix)noexcept
{

    i_parent_inverse_matrix=i_parent_inverse_matrix.translate(-this->_translate[0],-this->_translate[1]).rotate(this->_rotate[0],-this->_rotate[1]).scale(1/this->_scale[0],1/this->_scale[1]); //平移到原点

    Matrix3X2 mat2=i_parent_inverse_matrix.translate(this->_width*this->_center[0],this->_height*this->_center[1]);

    float x=i_x*mat2.getElement(0,0)+i_y*mat2.getElement(1,0)+mat2.getElement(2,0);
    float y=i_x*mat2.getElement(0,1)+i_y*mat2.getElement(1,1)+mat2.getElement(2,1);


    if(x>=0&&x<=this->_width&&y>=0&&y<=this->_height)
    {

        for(auto i:this->_rectChildren)
        {

            if (i->onPointerDown(i_x, i_y, i_parent_inverse_matrix))
            {
                return true;
            }
        }

        this->pointerDown(i_x,i_y);
        return true;

    } else
    {
        return false;
    }

}

void Rect::onPointerUp(float i_x, float i_y)noexcept
{

}

void Rect::onPointerMoved(float i_x, float i_y)noexcept
{

}

void Rect::pointerDown(float i_x, float i_y)noexcept
{
    setColor({0.5f,0.5f,0.5f,1.0f});
}

void Rect::pointerUp(float i_x, float i_y)noexcept
{

}

void Rect::pointerMoved(float i_x, float i_y)noexcept
{

}

















