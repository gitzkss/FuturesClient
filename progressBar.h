#pragma once
#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#endif // PROGRESSBAR_H
#include <QProgressBar>
#include <QPainter>
#include <QStyleOptionProgressBar>
class CustomProgressBar : public QProgressBar
{
protected:
    void paintEvent(QPaintEvent *event) override;
};
