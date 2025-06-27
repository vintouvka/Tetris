#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include <QGraphicsScene>
#include <QPushButton>

class GameOverScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameOverScene(int finalScore, QObject* parent = nullptr);

signals:
    void tryAgainClicked();
};

#endif // GAMEOVERSCENE_H
