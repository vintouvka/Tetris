#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsView>
class GameOverScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit GameOverScene(int finalScore, QObject* parent = nullptr);
signals:
    void tryAgainClicked();
private:
    QPushButton*    tryAgainButton = nullptr;
    QGraphicsView*  view           = nullptr;
    void setupTryAgainButton();
};

#endif // GAMEOVERSCENE_H
