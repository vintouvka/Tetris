#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QPushButton>
#include <QGraphicsView>
#include <QTimer>
#include <QList>

class StartScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit StartScene(QGraphicsView* parentView, QObject* parent = nullptr);
    bool startClicked = false;

signals:
    void startGame();

private slots:
    void animateBlocks();

private:
    QPushButton* startButton = nullptr;
    QGraphicsView* view = nullptr;
    QList<QGraphicsRectItem*> decorativeBlocks;
    QList<int> blockDirections;
    QTimer* animationTimer = nullptr;

    void createTitle();
    void createStartButton();
    void createDecorativeBlocks();
};

#endif // STARTSCENE_H
