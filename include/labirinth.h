#ifndef LABIRINTH_H
#define LABIRINTH_H

#include "grid.h"
#include <vector>
#include <math.h>
#include <time.h>
#include <cstdlib>
using namespace std;

class Labirinth{

    public:
        Labirinth(int screenWidth, int screenHeight, int escala);
        virtual ~Labirinth();

        grid *g;

    protected:

    private:
        int escala;
};

#endif // LABIRINTH_H
