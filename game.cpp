#include "game.h"
#include <QEvent>
#include <QKeyEvent>
#include "tetromino.h"
#include "constants.h"
#include <vector>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>

Game::Game(QGraphicsScene* scene, QGraphicsTextItem* scoreText, QObject* parent)
    : QObject(parent),
    scene(scene),
    scoreTextItem(scoreText),
    score(0),
    currentTetromino(nullptr),
    nextTetromino(nullptr),
    gameOverFlag(false)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Game::update);

    board.resize(rows, std::vector<int>(cols, 0));
    blockItems.resize(rows, std::vector<QGraphicsRectItem*>(cols, nullptr));

    nextTetromino = new Tetromino(scene);
}

void Game::start() {
    spawnTetromino();
    timer->start(500);
}

void Game::update() {
    if (!currentTetromino)
        return;

    if (currentTetromino->moveDown(board))
        return;

    landCurrentTetromino();
    clearFullRows();

    score += 5;
    scoreTextItem->setPlainText(QString::number(score));

    if (isGameOver()) {
        timer->stop();
        showGameOver();
        return;
    }

    currentTetromino = nullptr;
    spawnTetromino();
}

bool Game::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);

        if (gameOverFlag &&
            (key->key() == Qt::Key_Left ||
             key->key() == Qt::Key_Right ||
             key->key() == Qt::Key_Up ||
             key->key() == Qt::Key_Down ||
             key->key() == Qt::Key_Space)) {
            return true;
        }

        switch (key->key()) {
        case Qt::Key_Left: moveLeft(); return true;
        case Qt::Key_Right: moveRight(); return true;
        case Qt::Key_Up: rotate(); return true;
        case Qt::Key_Down: update(); return true;
        case Qt::Key_Space: drop(); return true;
        default: break;
        }
    }
    return QObject::eventFilter(obj, event);
}

void Game::moveLeft() {
    if (currentTetromino)
        currentTetromino->moveLeft(board);
}
void Game::moveRight() {
    if (currentTetromino)
        currentTetromino->moveRight(board);
}

void Game::rotate() {
    if (currentTetromino)
        currentTetromino->rotate(board);
}

void Game::drop() {
    if (!currentTetromino)
        return;
    while (currentTetromino && currentTetromino->moveDown(board)) {}
    update();
}

void Game::spawnTetromino() {
    if (!nextTetromino)
        nextTetromino = new Tetromino(scene);

    currentTetromino = nextTetromino;
    currentTetromino->resetPosition();

    nextTetromino = new Tetromino(scene);

    if (nextTetromino) {
        nextTetromino->previewAt(12 * TILE_SIZE + 47, 160);
    }
}

void Game::landCurrentTetromino() {
    if (!currentTetromino)
        return;

    for (int i = 0; i < 4; ++i) {
        int x = currentTetromino->posX + currentTetromino->shape[i][0];
        int y = currentTetromino->posY + currentTetromino->shape[i][1];
        if (y >= 0 && y < rows && x >= 0 && x < cols) {
            board[y][x] = 1;
            if (i < currentTetromino->blocks.size()) {
                blockItems[y][x] = currentTetromino->blocks[i];
            }
        }
    }
}

bool Game::isGameOver() const {
    for (int col = 0; col < cols; ++col) {
        if (board[0][col] != 0)
            return true;
    }
    return false;
}
void Game::showGameOver() {
    gameOverFlag = true;
    QGraphicsTextItem* gameOverText = new QGraphicsTextItem("Game over");
    QFont font("Arial", 35, QFont::Bold);
    gameOverText->setFont(font);
    gameOverText->setDefaultTextColor(Qt::red);
    QRectF textRect = gameOverText->boundingRect();
    QGraphicsRectItem* background = new QGraphicsRectItem(textRect);
    background->setBrush(QBrush(Qt::black));
    background->setOpacity(0.7);
    background->setPen(Qt::NoPen);
    QGraphicsItemGroup* group = new QGraphicsItemGroup;
    group->addToGroup(background);
    group->addToGroup(gameOverText);
    QRectF groupRect = group->boundingRect();
    group->setPos(scene->width() / 2 - groupRect.width() / 2,
                  scene->height() / 2 - groupRect.height() / 2);
    scene->addItem(group);
}
void Game::clearFullRows() {
    std::vector<int> fullRows;
    for (int row = 0; row < rows; ++row) {
        bool isFull = true;
        for (int col = 0; col < cols; ++col) {
            if (board[row][col] == 0) {
                isFull = false;
                break;
            }
        }
        if (isFull)
            fullRows.push_back(row);
    }

    if(fullRows.empty())
        return;

    score += 40 * fullRows.size();
    scoreTextItem->setPlainText(QString::number(score));
    std::vector<std::vector<int>> newBoard(rows, std::vector<int>(cols, 0));
    std::vector<std::vector<QGraphicsRectItem*>> newBlockItems(rows, std::vector<QGraphicsRectItem*>(cols, nullptr));

    int newRow = rows - 1;
    for (int row = rows - 1; row >= 0; --row) {

        bool isFull = false;
        for (int i = 0; i < fullRows.size(); ++i) {
            if (row == fullRows[i]) {
                isFull = true;

                for (int col = 0; col < cols; ++col) {
                    if (blockItems[row][col]) {
                        scene->removeItem(blockItems[row][col]);
                        delete blockItems[row][col];
                        blockItems[row][col] = nullptr;
                    }
                }
                break;
            }
        }
        if (!isFull) {

            newBoard[newRow] = board[row];
            newBlockItems[newRow] = blockItems[row];


            for (int col = 0; col < cols; ++col) {
                if (newBlockItems[newRow][col]) {
                    newBlockItems[newRow][col]->setPos(col * TILE_SIZE, newRow * TILE_SIZE);
                }
            }
            newRow--;
        }
    }
    for (int row = newRow; row >= 0; --row) {
        for (int col = 0; col < cols; ++col) {
            newBoard[row][col] = 0;
            newBlockItems[row][col] = nullptr;
        }
    }
    board = newBoard;
    blockItems = newBlockItems;
}
