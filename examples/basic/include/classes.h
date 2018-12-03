namespace v1 {

struct A {
    int a, b, c;
    float x, y, z;
    int q, t, v;
};

struct B {
    int a, b, c;
    float x, y, z;
    int q, t, v;
};

struct s_1 {
    int x,y,z;
};

struct s_2 {
    float q,y,t;
};

struct s_3 {
    s_1 s1;
    s_2 s2;
};

}

namespace v2 {

struct C {
    v1::B b;
    v1::s_3 s3;
    int x;
    float q;

    static ::v2::C make_random() {
        return {{1,1,1, 1.2, 1.2, 1.2, 3, 3, 3}, 
                {{1,2,3}, {1.0, 2.0, 3.0}},
                10, 10.5
               };
    }
};

}
