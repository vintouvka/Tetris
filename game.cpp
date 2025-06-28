#include "game.h"
#include "gameoverscene.h"
#include "pausescene.h"
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
#include <windows.h>
#include <mmsystem.h>
#include <QGraphicsView>

#pragma comment(lib, "winmm.lib")

extern QGraphicsView* view;
Game::Game(QGraphicsScene* sceneArg,
           QGraphicsTextItem* scoreText,
           QObject* parent)
    : QObject(parent),
    gameScene(sceneArg),
    scoreTextItem(scoreText),
    score(0),
    currentTetromino(nullptr),
    nextTetromino(nullptr),
    gameOverFlag(false),
    paused(false)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Game::update);

    board.resize(rows, std::vector<int>(cols, 0));
    blockItems.resize(rows, std::vector<QGraphicsRectItem*>(cols, nullptr));
    nextTetromino = new Tetromino(gameScene);
}

void Game::start() {
    mciSendString(
        L"open \"C:\\Users\\natal\\Downloads\\duriggame.wav\" type mpegvideo alias bgmusic",
        nullptr, 0, nullptr
        );
    mciSendString(L"play bgmusic repeat", nullptr, 0, nullptr);

    spawnTetromino();
    timer->start(500);
}

void Game::update() {
    if (!currentTetromino) return;

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
        auto key = static_cast<QKeyEvent*>(event);
        if (gameOverFlag &&
            (key->key() == Qt::Key_Left ||
             key->key() == Qt::Key_Right ||
             key->key() == Qt::Key_Up ||
             key->key() == Qt::Key_Down ||
             key->key() == Qt::Key_Space)) {
            return true;
        }
        switch (key->key()) {
        case Qt::Key_Left:  moveLeft();  return true;
        case Qt::Key_Right: moveRight(); return true;
        case Qt::Key_Up:    rotate();    return true;
        case Qt::Key_Down:  update();    return true;
        case Qt::Key_Space: drop();      return true;
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
    if (!currentTetromino) return;
    while (currentTetromino && currentTetromino->moveDown(board)) {}
    update();
}

void Game::spawnTetromino() {
    if (!nextTetromino)
        nextTetromino = new Tetromino(gameScene);

    currentTetromino = nextTetromino;
    currentTetromino->resetPosition();

    nextTetromino = new Tetromino(gameScene);
    if (nextTetromino) {
        nextTetromino->previewAt(12 * TILE_SIZE + 47, 160);
    }
}

void Game::landCurrentTetromino() {
    if (!currentTetromino) return;

    for (int i = 0; i < 4; ++i) {
        int x = currentTetromino->posX + currentTetromino->shape[i][0];
        int y = currentTetromino->posY + currentTetromino->shape[i][1];
        if (y >= 0 && y < rows && x >= 0 && x < cols) {
            board[y][x] = 1;
            if (i < currentTetromino->blocks.size())
                blockItems[y][x] = currentTetromino->blocks[i];
        }
    }
   PlaySoundW(L"C:/Users/natal/Downloads/click-47609.wav", nullptr, SND_FILENAME | SND_ASYNC);
}

bool Game::isGameOver() const {
    for (int col = 0; col < cols; ++col)
        if (board[0][col] != 0)
            return true;
    return false;
}
void Game::showGameOver() {
    if (auto btn = view->findChild<QPushButton*>("pauseButton"))
        btn->deleteLater();
    view->setScene(gameScene);

    mciSendString(L"stop bgmusic", nullptr, 0, nullptr);
    mciSendString(L"close bgmusic", nullptr, 0, nullptr);

    gameOverFlag = true;
    gameScene->clear();
    auto bg = new QGraphicsRectItem(0, 0, gameScene->width(), gameScene->height());
    bg->setBrush(QBrush(QColor(10, 10, 60)));
    bg->setPen(Qt::NoPen);
    gameScene->addItem(bg);

    auto gameOver = new GameOverScene(score);
    connect(gameOver, &GameOverScene::tryAgainClicked,
            this,      [this]() { emit tryAgainRequested(); });
    for (auto item : gameOver->items())
        gameScene->addItem(item);

    PlaySoundW(L"C:/Users/natal/Downloads/gameover.wav", nullptr, SND_FILENAME | SND_ASYNC);
}

void Game::resetGame() {
    gameScene->clear();

    board.assign(rows, std::vector<int>(cols, 0));
    blockItems.assign(rows, std::vector<QGraphicsRectItem*>(cols, nullptr));
    score = 0;
    scoreTextItem->setPlainText("0");
    gameOverFlag = false;

    delete currentTetromino; currentTetromino = nullptr;
    delete nextTetromino;    nextTetromino    = nullptr;

    gameScene->addItem(scoreTextItem);
    nextTetromino = new Tetromino(gameScene);
    start();
}

void Game::clearFullRows() {
    std::vector<int> fullRows;
    for (int r = 0; r < rows; ++r) {
        bool isFull = true;
        for (int c = 0; c < cols; ++c)
            if (board[r][c] == 0) { isFull = false; break; }
        if (isFull) fullRows.push_back(r);
    }
    if (fullRows.empty()) return;

    score += 40 * fullRows.size();
    scoreTextItem->setPlainText(QString::number(score));
    PlaySoundW(L"C:/Users/natal/Downloads/clearrow.wav", nullptr, SND_FILENAME | SND_ASYNC);

    std::vector<std::vector<int>> newBoard(rows, std::vector<int>(cols, 0));
    std::vector<std::vector<QGraphicsRectItem*>> newBlocks(rows, std::vector<QGraphicsRectItem*>(cols, nullptr));

    int newRow = rows - 1;
    for (int r = rows - 1; r >= 0; --r) {
        bool isFull = std::find(fullRows.begin(), fullRows.end(), r) != fullRows.end();
        if (isFull) {
            for (int c = 0; c < cols; ++c) {
                if (blockItems[r][c]) {
                    gameScene->removeItem(blockItems[r][c]);
                    delete blockItems[r][c];
                }
            }
        } else {
            newBoard[newRow]     = board[r];
            newBlocks[newRow]    = blockItems[r];
            for (int c = 0; c < cols; ++c)
                if (newBlocks[newRow][c])
                    newBlocks[newRow][c]->setPos(c * TILE_SIZE, newRow * TILE_SIZE);
            --newRow;
        }
    }
    board      = std::move(newBoard);
    blockItems = std::move(newBlocks);
}

void Game::resumeGame() {
    if (!paused) return;
    paused = false;

    delete pauseScene;
    pauseScene = nullptr;

    if (auto btn = view->findChild<QPushButton*>("pauseButton"))
        btn->show();

    view->setScene(gameScene);
    timer->start(500);
}

void Game::pauseGame() {
    if (paused) return;
    paused = true;
    timer->stop();
    if (auto btn = view->findChild<QPushButton*>("pauseButton"))
        btn->hide();

    pauseScene = new PauseScene(fieldWidth + panelWidth, fieldHeight, view, this);
    connect(pauseScene, &PauseScene::resumeRequested, this, &Game::resumeGame);
    connect(pauseScene, &PauseScene::quitRequested,   this, &Game::quitGame);

    view->setScene(pauseScene);
}

void Game::quitGame() {

    if (paused) {
        paused = false;
        delete pauseScene;
        pauseScene = nullptr;
        view->setScene(gameScene);
    }
    timer->stop();
    if (auto btn = view->findChild<QPushButton*>("pauseButton"))
        btn->deleteLater();

    showGameOver();
}


