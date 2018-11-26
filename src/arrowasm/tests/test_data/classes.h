namespace v1 {

struct A {
    int a, b, c;
    float x, y, z;
    int q, t, v;
};

class B {
    int a, b, c;
    float x, y, z;
    int q, t, v;
};

}

namespace v2 {

class C {
    v1::B b;
    int x;
    float q;
};

}
