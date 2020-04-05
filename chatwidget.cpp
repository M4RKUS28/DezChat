#include "chatwidget.h"

ChatWidget::ChatWidget(QWidget *widget)
    : QTextEdit(widget), colorCounter(0)
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setReadOnly(true);

    //Save "Black" to nullptr
    setNewColorToPeer(nullptr);

}

void ChatWidget::addMsg(QString msg, Qt::Alignment alignment, const QColor &color, const Peer *colorByPeer)
{
    if (msg.isEmpty())
        return;

    //Wichtig, da sonst die ausgewählte stelle gefärbt und verschoben wird
    this->moveCursor(QTextCursor::End);

    //save old color + set new color
    QColor color_ = this->textColor();
    this->setTextColor( (colorByPeer) ? getColorByPeer(colorByPeer) : color );

    //print msg + set aligment
    this->append(msg);
    this->setAlignment( alignment );

    //set old color + scroll down
    this->setTextColor(color_);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());
}

void ChatWidget::addSenderNameAndMSG(const Peer *who, QString msg, QString cusomMsg)
{
    if (msg.isEmpty())
        return;

    ///---->Name + Time
    QColor color_ = this->textColor();
    this->setTextColor( getColorByPeer(who) );

    //print msg + set aligment
    this->append(QTime::currentTime().toString("hh:mm") + ((who) ? (" ~" + who->getName()) : " Du") + cusomMsg +":");
    this->setAlignment( (who) ? Qt::AlignLeft : Qt::AlignRight );

    this->moveCursor(QTextCursor::End);
    QTextCursor cursor(this->textCursor());
    cursor.select(QTextCursor::SelectionType::LineUnderCursor);

    QTextCharFormat charFormat = cursor.charFormat();
    charFormat.setFont(QFont(/*"Times"*/ "Open Sans", 8, QFont::Bold));
    cursor.setCharFormat(charFormat);

    //set old color + scroll down
    this->setTextColor(color_);
    ///<----

    ///---> Message
    //print msg + set aligment
    this->append(msg);

    //set default QTextFormat
    this->moveCursor(QTextCursor::End);
    cursor.select(QTextCursor::SelectionType::LineUnderCursor);
    cursor.setCharFormat(QTextCharFormat());

    ///<----

    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());
}


QColor ChatWidget::getColorByPeer(const Peer *peer)
{
    return ( colorMap.contains(peer) ) ? colorMap[peer] : setNewColorToPeer(peer);
}

QColor ChatWidget::setNewColorToPeer(const Peer *peer)
{
    return colorMap.insert(peer, colorList[ ( ( colorCounter ++ ) % (sizeof(colorList)/sizeof(*colorList)) ) ] ).value();
}


