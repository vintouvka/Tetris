#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QList>




class StartScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit StartScene(QObject* parent = nullptr);
    bool startClicked = false;

signals:
    void startGame();

private slots:
    void animateBlocks();

private:
    QGraphicsRectItem* startButton;
    QList<QGraphicsRectItem*> decorativeBlocks;
    QList<int> blockDirections;
    QTimer* animationTimer;


    void createTitle();
    void createStartButton();
    void createDecorativeBlocks();


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // STARTSCENE_H
