#include "tetromino.h"
#include "constants.h"
#include <vector>


Tetromino::Tetromino(QGraphicsScene* scene)
    : scene(scene), posX(cols / 2), posY(0)
{
    spawn();
}

void Tetromino::spawn() {
    clearBlocks();
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


    switch (type) {
    case 0: color = QColor(255, 85, 85); break;
    case 1: color = QColor(255, 215, 0); break;
    case 2: color = QColor(138, 43, 226); break;
    case 3: color = QColor(50, 205, 50); break;
    case 4: color = QColor(255, 140, 0); break;
    case 5: color = QColor(30, 144, 255); break;
    case 6: color = QColor(255, 20, 147); break;
    }


    for (int i = 0; i < 4; ++i) {
        QGraphicsRectItem* block = new QGraphicsRectItem();
        block->setBrush(color);
        block->setRect(0, 0, TILE_SIZE, TILE_SIZE);
        scene->addItem(block);
        blocks.append(block);
    }

    posX = cols / 2;
    posY = 0;
    updateBlockPositions();
}

void Tetromino::clearBlocks() {
    for (auto block : blocks) {
        scene->removeItem(block);
        delete block;
    }
    blocks.clear();
}

void Tetromino::updateBlockPositions() {
    for (int i = 0; i < 4; ++i) {
        int x = (posX + shape[i][0]) * TILE_SIZE;
        int y = (posY + shape[i][1]) * TILE_SIZE;
        blocks[i]->setPos(x, y);
    }
}

bool Tetromino::checkCollision(int dx, int dy, const std::vector<std::vector<int>>& board) {
    for (int i = 0; i < 4; ++i) {
        int newX = posX + shape[i][0] + dx;
        int newY = posY + shape[i][1] + dy;
        if (newX < 0 || newX >= cols || newY < 0 || newY >= rows)
            return true;
        if (board[newY][newX] != 0)
            return true;
    }
    return false;
}

bool Tetromino::moveDown(const std::vector<std::vector<int>>& board) {
    if (!checkCollision(0, 1, board)) {
        posY += 1;
        updateBlockPositions();
        return true;
    }
    return false;
}

void Tetromino::moveLeft(const std::vector<std::vector<int>>& board) {
    if (!checkCollision(-1, 0, board)) {
        posX -= 1;
        updateBlockPositions();
    }
}

void Tetromino::moveRight(const std::vector<std::vector<int>>& board) {
    if (!checkCollision(1, 0, board)) {
        posX += 1;
        updateBlockPositions();
    }
}

void Tetromino::rotate(const std::vector<std::vector<int>>& board) {
    int temp[4][2];

    for (int i = 0; i < 4; ++i) {
        temp[i][0] = -shape[i][1];
        temp[i][1] = shape[i][0];
    }

    for (int i = 0; i < 4; ++i) {
        int newX = posX + temp[i][0];
        int newY = posY + temp[i][1];

        if (newX < 0 || newX >= cols || newY >= rows)
            return;

        if (newY >= 0 && board[newY][newX] != 0)
            return;
        if (posY < 1)
            return;
    }

    for (int i = 0; i < 4; ++i) {
        shape[i][0] = temp[i][0];
        shape[i][1] = temp[i][1];
    }

    updateBlockPositions();
}

void Tetromino::resetPosition() {
    posX = cols / 2;
    posY = 0;
    updateBlockPositions();
}

void Tetromino::previewAt(int x, int y) {
    clearBlocks();

    for (int i = 0; i < 4; ++i) {
        int blockX = x + shape[i][0] * TILE_SIZE;
        int blockY = y + shape[i][1] * TILE_SIZE;

        QGraphicsRectItem* block = scene->addRect(0, 0, TILE_SIZE, TILE_SIZE);
        block->setPos(blockX, blockY);
        block->setBrush(QBrush(color));
        blocks.append(block);
    }
}
