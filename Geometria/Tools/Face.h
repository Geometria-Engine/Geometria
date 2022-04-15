#ifndef FACE_H
#define FACE_H

struct Face
{
    Vector3 a1;
    Vector3 b2;
    Vector3 c3;

    friend bool operator==(Face& l, Face& r)
    {
        return 
        l.a1 == r.a1 &&
        l.b2 == r.b2 &&
        l.c3 == r.c3;
    }

    friend bool operator!=(Face& l, Face& r)
    {
        return 
        l.a1 != r.a1 &&
        l.b2 != r.b2 &&
        l.c3 != r.c3;
    }
};


#endif