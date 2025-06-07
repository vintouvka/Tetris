#ifndef TETROMINO_H
#define TETROMINO_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QVector>
#include <QColor>
#include <QRandomGenerator>

const int TILE_SIZE = 18;
const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 20;

class Tetromino {
public:
    Tetromino(QGraphicsScene* scene);
    void spawn();

private:
    QGraphicsScene* scene;
    QVector<QGraphicsRectItem*> blocks;
    int shape[4][2];
    QColor color;

    void createBlock(int x, int y);
};

#endif
