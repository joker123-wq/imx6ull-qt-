#include "wavewidget.h"
#include <QPainter>
#include <QPaintEvent>

WaveWidget::WaveWidget(QWidget *parent)
    : QWidget(parent), m_maxPoints(200)
{
    setMinimumHeight(260);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("background:white; border:2px solid #D5DEE7; border-radius:12px;");
}

void WaveWidget::addPoint(float value)
{
    if (m_values.size() >= m_maxPoints)
        m_values.removeFirst();

    m_values.append(value);
    update();
}

void WaveWidget::clearData()
{
    m_values.clear();
    update();
}

void WaveWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect r = rect().adjusted(10, 10, -10, -10);

    // 背景网格
    painter.setPen(QPen(QColor("#D0D7DE"), 1, Qt::DashLine));
    for (int i = 0; i <= 5; ++i)
    {
        int y = r.top() + i * r.height() / 5;
        painter.drawLine(r.left(), y, r.right(), y);
    }
    for (int i = 0; i <= 10; ++i)
    {
        int x = r.left() + i * r.width() / 10;
        painter.drawLine(x, r.top(), x, r.bottom());
    }

    painter.setPen(QPen(QColor("#22313F"), 1));
    painter.drawRect(r);

    if (m_values.size() < 2)
    {
        painter.drawText(r, Qt::AlignCenter, "等待数据...");
        return;
    }

    float minVal = m_values[0];
    float maxVal = m_values[0];
    for (float v : m_values)
    {
        if (v < minVal) minVal = v;
        if (v > maxVal) maxVal = v;
    }

    if (maxVal - minVal < 0.01f)
    {
        maxVal += 1.0f;
        minVal -= 1.0f;
    }

    painter.setPen(QPen(QColor("#FF6B35"), 2));

    QPainterPath path;
    for (int i = 0; i < m_values.size(); ++i)
    {
        float ratio = (m_values[i] - minVal) / (maxVal - minVal);
        int x = r.left() + i * r.width() / (m_maxPoints - 1);
        int y = r.bottom() - ratio * r.height();

        if (i == 0)
            path.moveTo(x, y);
        else
            path.lineTo(x, y);
    }

    painter.drawPath(path);

    painter.setPen(QPen(QColor("#22313F"), 1));
    painter.drawText(r.left() + 8, r.top() + 18, QString("Max: %1").arg(maxVal, 0, 'f', 2));
    painter.drawText(r.left() + 8, r.bottom() - 6, QString("Min: %1").arg(minVal, 0, 'f', 2));
}
