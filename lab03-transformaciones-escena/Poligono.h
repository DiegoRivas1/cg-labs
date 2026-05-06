//
// Created by DIEGO on 04/05/2026.
//


#ifndef CG_LABS_POLIGONO_H
#define CG_LABS_POLIGONO_H
class Poligono {
    private:
        int numeroDeVertices;
    public:
        Poligono();
        Poligono(int numeroVertices);
        ~Poligono();
        void dibujar() const;
};
#endif //CG_LABS_POLIGONO_H