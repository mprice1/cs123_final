#ifndef STRUCTS_CPP
#define STRUCTS_CPP

struct Translation{
    double x;
    double y;
    double z;
};

struct Rotation{
    double x;
    double y;
    double z;
    double angle;
};

struct Shapes{
    Translation t;
    Rotation r;
    int rectx;
    int recty;
    double polr;
    double polt;
};

#endif
