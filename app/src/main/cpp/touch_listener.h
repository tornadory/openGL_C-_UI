//
// Created by cheng on 2017/1/16.
//

#pragma  once

struct Rect;

struct TouchListener {

//    virtual void touchMove() =0;
//    virtual void touchDown(int i_point_id) =0;
//    virtual void touchUp(int i_point_id) =0;

      virtual bool touchEvent(int i_event_id, int i_point_id, shared_ptr<Rect> i_rect) =0;

};


