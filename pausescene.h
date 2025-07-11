#ifndef PAUSESCENE_H
#define PAUSESCENE_H

#include <QGraphicsScene>
#include <QPushButton>
#include <QSoundEffect>

class QGraphicsView;

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
    QPushButton* pausedBtn;
    QSoundEffect* resumeEffect{nullptr};
    QSoundEffect* quitEffect{nullptr};
    QSoundEffect* clickEffect{nullptr};
};

#endif // PAUSESCENE_H
