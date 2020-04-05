#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QTextEdit>
#include <QMap>
#include <QScrollBar>

#include "peer.h"


class ChatWidget : public QTextEdit
{
    Q_OBJECT
public:
    ChatWidget(QWidget * widget);
    void addMsg(QString msg, Qt::Alignment alignment = Qt::AlignLeft, const QColor &color = Qt::black, const Peer *colorByPeer = nullptr);
    void addSenderNameAndMSG(const Peer *who, QString msg, QString customText = "");

    QColor getColorByPeer(const Peer *peer);

private:
    QColor setNewColorToPeer(const Peer *peer);

    const QColor colorList[14] = {  Qt::black, Qt::blue, Qt::darkBlue, Qt::green, Qt::darkGreen,
                                    Qt::darkCyan, Qt::red, Qt::darkRed,
                                    Qt::magenta, Qt::darkMagenta, Qt::darkYellow,
                                    QColor::fromRgb(150,78,0), QColor::fromRgb(100,50,0), QColor::fromRgb(102, 0, 51) };
    size_t colorCounter;
    QMap<const Peer*, QColor> colorMap;

};

#endif // CHATWIDGET_H
