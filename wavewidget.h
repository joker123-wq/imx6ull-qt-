#ifndef WAVEWIDGET_H
#define WAVEWIDGET_H

#include <QWidget>
#include <QVector>

class WaveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WaveWidget(QWidget *parent = nullptr);

    void addPoint(float value);
    void clearData();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<float> m_values;
    int m_maxPoints;
};

#endif // WAVEWIDGET_H
