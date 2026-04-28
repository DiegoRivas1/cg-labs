
#ifndef CG_LABS_SHAPES_H
#define CG_LABS_SHAPES_H


// Ejercicio 1 Cuadrado centrado en (cx, cy) con arista de longitud 'side'
void drawSquare(float cx, float cy, float side);

// Ejercicio 2 Círculo centrado en (cx, cy) con 'radius' y 'segments' lados
void drawCircle(float cx, float cy, float radius, int segments);

// Secuencias

// Ejercicio 3 Círculos concéntricos, cada uno reduce su radio por reductionPct (0.0 - 1.0)
void drawCircleSequenceConcentric(float cx, float cy, float radius, int count, float reductionPct, int segments);

// Ejercicio 3b: Igual pero todos comparten el borde izquierdo en 'leftEdge'
void drawCircleSequenceConcentricLeft(float leftEdge, float cy, float radius, int count, float reductionPct, int segments);

// Ejercicio 3c: Todos comparten el borde derecho en 'rightEdge'
void drawCircleSequenceConcentricRight(float rightEdge, float cy, float radius, int count, float reductionPct, int segments);

// Ejercicio 3d: Todos comparten el borde superior en 'topEdge'
void drawCircleSequenceConcentricTop(float cx, float topEdge, float radius, int count, float reductionPct, int segments);

// Ejercicio 3e: Todos comparten el borde inferior en 'bottomEdge'
void drawCircleSequenceConcentricBottom(float cx, float bottomEdge, float radius, int count, float reductionPct, int segments);

// Ejercicio 4 Círculos alineados horizontalmente, cada uno a la derecha del anterior
void drawCircleSequenceHorizontal(float cx, float cy, float radius, int count, float reductionPct, int segments);

// Ejercicio 5: Círculos en espiral, cada uno rotado 'angleDeg' grados respecto al anterior
void drawCircleSequenceSpiral(float cx, float cy, float radius, int count, float reductionPct, float angleDeg, int segments);
//Ejercicio 5a: ciruclos alineados en una recta
void drawCircleSequenceLine(float cx, float cy, float radius, int count, float reductionPct, float angleDeg, int segments);

#endif //CG_LABS_SHAPES_H