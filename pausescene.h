#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include <QGraphicsScene>
#include <QPushButton>

class PauseScene : public QGraphicsScene {
    Q_OBJECT
public:
    PauseScene(int w, int h, QGraphicsView* parentView, QObject* parent = nullptr);
    ~PauseScene() override;

signals:
    void resumeRequested();
    void quitRequested();

private:
    QPushButton* resumeBtn{nullptr};
    QPushButton* quitBtn{nullptr};
};

#endif // PAUSESCENE_H
