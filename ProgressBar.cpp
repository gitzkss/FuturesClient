#include "progressBar.h""
void CustomProgressBar::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    QPainter painter(this);
    QStyleOptionProgressBar option;
    initStyleOption(&option);

    // 设置进度条的背景
    painter.setBrush(Qt::white);
    painter.drawRect(option.rect);

    // 计算红色部分的宽度
    int redWidth = (this->value() * option.rect.width()) / this->maximum();

    // 绘制红色部分
    painter.setBrush(Qt::red);
    painter.drawRect(0, 0, redWidth, option.rect.height());

    // 绘制绿色部分
    painter.setBrush(Qt::green);
    painter.drawRect(redWidth, 0, option.rect.width() - redWidth, option.rect.height());
}
