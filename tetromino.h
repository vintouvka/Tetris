#ifndef TETROMINO_H
#define TETROMINO_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QVector>
#include <QColor>
#include <QRandomGenerator>
#include <vector>


const int TILE_SIZE = 24;
const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 20;
class Tetromino {
public:
    Tetromino(QGraphicsScene* scene);
    int posX;
    int posY;
    int shape[4][2];
    void rotate();
    void spawn();
    bool checkCollision(int dx, int dy, const std::vector<std::vector<int>>& board);
    bool moveDown(const std::vector<std::vector<int>>& board);
    void moveLeft(const std::vector<std::vector<int>>& board);
    void moveRight(const std::vector<std::vector<int>>& board);
    void rotate(const std::vector<std::vector<int>>& board);
    QColor color;
    QVector<QGraphicsRectItem*> blocks;
    void resetPosition();
    void previewAt(int x, int y);
private:
    QGraphicsScene* scene;
    void createBlock(int x, int y);
    void applyShape();
    void updateBlockPositions();
    void clearBlocks();

};

#endif // TETROMINO_H
