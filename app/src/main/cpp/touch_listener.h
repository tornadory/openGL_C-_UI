//
// Created by cheng on 2017/1/16.
//

#pragma  once

struct TouchListener {

    virtual void touchMove(float i_dx, float i_dy)=0;
    virtual void touchDown(float i_dx, float i_dy)=0;
    virtual void touchUp(float i_dx, float i_dy)=0;

};


