#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QEvent>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <vector>

#include "tetromino.h"
#include "constants.h"
#include "pausescene.h"

class Game : public QObject
{
    Q_OBJECT

public:
    explicit Game(QGraphicsScene*     scene,
                  QGraphicsTextItem*  scoreText,
                  QObject*            parent = nullptr);

    void start();
    void update();
    bool eventFilter(QObject* obj, QEvent* event) override;
    void showGameOver();
    bool isGameOver() const;

public slots:
    void moveLeft();
    void moveRight();
    void rotate();
    void drop();
    void landCurrentTetromino();
    void resetGame();
    void pauseGame();
    void resumeGame();
    void quitGame();

signals:
    void tryAgainRequested();

private:
    void spawnTetromino();
    void clearFullRows();
    QGraphicsScene* gameScene;
    QGraphicsTextItem* scoreTextItem;
    PauseScene* pauseScene{nullptr};
    Tetromino* currentTetromino{nullptr};
    Tetromino* nextTetromino{nullptr};
    std::vector<std::vector<QGraphicsRectItem*>> blockItems;
    std::vector<std::vector<int>>                board;
    QTimer* timer{nullptr};
    bool gameOverFlag{false};
    bool paused{false};
    int score{0};
    QMediaPlayer* bgmPlayer{nullptr};
    QAudioOutput* bgmOutput{nullptr};
    QGraphicsRectItem* pauseOverlay{nullptr};
    QList<QGraphicsItem*> pauseOverlayItems;
};

#endif // GAME_H



