#ifndef ROWS_H
#define ROWS_H

template<class T>
class Knot {
public:
    int x, y;
    double F;
    bool sup;
    };

template<class T>
class Row {
public:
    double L, A, E, Sig, q;
    };


#endif // ROWS_H
