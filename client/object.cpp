#include "object.h"

Object::Object(int _id, int _type, double _x, double _y, double _direction) {
    x=_x;
    y=_y;

    direction=_direction;
    id=_id;
    type=_type;
}

