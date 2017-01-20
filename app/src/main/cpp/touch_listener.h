//
// Created by cheng on 2017/1/16.
//

#pragma  once

struct TouchListener {

    virtual void touchMove(int i_point_id, float i_dx, float i_dy) =0;
    virtual void touchDown(int i_point_id, float i_dx, float i_dy) =0;
    virtual void touchUp(int i_point_id, float i_dx, float i_dy) =0;

};


