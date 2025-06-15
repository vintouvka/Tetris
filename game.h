#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QVector>
#include <vector>
#include "tetromino.h"
#include "constants.h"

class Game : public QObject
{
    Q_OBJECT

public:
    explicit Game(QGraphicsScene* scene, QGraphicsTextItem* scoreText, QObject* parent = nullptr);
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
private:
    std::vector<std::vector<QGraphicsRectItem*>> blockItems;
    QGraphicsTextItem* scoreTextItem;
    bool gameOverFlag;
    std::vector<std::vector<int>> board;
    QGraphicsScene* scene;
    Tetromino* currentTetromino;
    Tetromino* nextTetromino;
    QTimer* timer;
    void spawnTetromino();
    int score;
    void clearFullRows();

};

#endif // GAME_H
