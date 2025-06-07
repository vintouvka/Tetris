#include "tetromino.h"

Tetromino::Tetromino(QGraphicsScene* scene) : scene(scene) {}

void Tetromino::spawn()
{

    for (auto block : blocks) {
        scene->removeItem(block);
        delete block;
    }
    blocks.clear();


    QVector<QColor> softColors = {
        QColor(150, 190, 230),
        QColor(190, 150, 230)
    };
    color = softColors[QRandomGenerator::global()->bounded(softColors.size())];


    int type = QRandomGenerator::global()->bounded(7);

    switch (type) {
    case 0:
        shape[0][0] = -1; shape[0][1] = 0;
        shape[1][0] = 0;  shape[1][1] = 0;
        shape[2][0] = 1;  shape[2][1] = 0;
        shape[3][0] = 2;  shape[3][1] = 0;
        break;
    case 1:
        shape[0][0] = 0; shape[0][1] = 0;
        shape[1][0] = 1; shape[1][1] = 0;
        shape[2][0] = 0; shape[2][1] = 1;
        shape[3][0] = 1; shape[3][1] = 1;
        break;
    case 2:
        shape[0][0] = -1; shape[0][1] = 0;
        shape[1][0] = 0;  shape[1][1] = 0;
        shape[2][0] = 1;  shape[2][1] = 0;
        shape[3][0] = 0;  shape[3][1] = 1;
        break;
    case 3:
        shape[0][0] = 0;  shape[0][1] = 0;
        shape[1][0] = 1;  shape[1][1] = 0;
        shape[2][0] = -1; shape[2][1] = 1;
        shape[3][0] = 0;  shape[3][1] = 1;
        break;
    case 4:
        shape[0][0] = -1; shape[0][1] = 0;
        shape[1][0] = 0;  shape[1][1] = 0;
        shape[2][0] = 0;  shape[2][1] = 1;
        shape[3][0] = 1;  shape[3][1] = 1;
        break;
    case 5:
        shape[0][0] = -1; shape[0][1] = 0;
        shape[1][0] = 0;  shape[1][1] = 0;
        shape[2][0] = 1;  shape[2][1] = 0;
        shape[3][0] = 1;  shape[3][1] = 1;
        break;
    case 6:
        shape[0][0] = -1; shape[0][1] = 0;
        shape[1][0] = 0;  shape[1][1] = 0;
        shape[2][0] = 1;  shape[2][1] = 0;
        shape[3][0] = -1; shape[3][1] = 1;
        break;
    }

    int startX = FIELD_WIDTH / 2;
    int startY = 0;

    for (int i = 0; i < 4; ++i) {
        int x = shape[i][0] + startX;
        int y = shape[i][1] + startY;
        createBlock(x, y);
    }
}

void Tetromino::createBlock(int x, int y)
{
    if (x < 0||  x >= FIELD_WIDTH||  y >= FIELD_HEIGHT)
        return;

    QGraphicsRectItem* block = scene->addRect(
        x * TILE_SIZE, y * TILE_SIZE,
        TILE_SIZE, TILE_SIZE,
        Qt::NoPen, QBrush(color)
        );

    blocks.append(block);
}
