#include "chatwidget.h"

ChatWidget::ChatWidget(QWidget *widget)
    : QTextEdit(widget), colorCounter(0)
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setReadOnly(true);

    //Save "Black" to nullptr
    setNewColorToPeer(nullptr);

}


void ChatWidget::addMsgOldStyle(QString msg, const Peer *who, Qt::Alignment alignment, const QColor &color)
{
    if (msg.isEmpty())
        return;

    //Wichtig, da sonst die ausgewählte stelle gefärbt und verschoben wird
    this->moveCursor(QTextCursor::End);

    //save old color + set new color
    QColor color_ = this->textColor();
    this->setTextColor( (color == Qt::white) ? getColorByPeer(who) : color );

    //print msg + set aligment
    this->append(msg);
    this->setAlignment( alignment );

    //set old color + scroll down
    this->setTextColor(color_);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());

}

void ChatWidget::addMsg(QString msg, Qt::Alignment alignment, const QColor &color)
{
    if (msg.isEmpty())
        return;

    //Wichtig, da sonst die ausgewählte stelle gefärbt und verschoben wird
    this->moveCursor(QTextCursor::End);


    //print msg + set aligment
    this->append(msg);
    this->setAlignment( alignment );

    //set default QTextFormat
    this->moveCursor(QTextCursor::End);
    QTextCursor cursor(this->textCursor());
    cursor.select(QTextCursor::SelectionType::LineUnderCursor);
    cursor.setCharFormat(QTextCharFormat());

    this->moveCursor(QTextCursor::End);
    moveCursor(QTextCursor::MoveOperation::Up);

    //save old color + set new color
    QColor color_ = this->textColor();
    this->setTextColor( color );

    moveCursor(QTextCursor::MoveOperation::End);

    //set old color + scroll down
    this->setTextColor(color_);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());

}

void ChatWidget::addSenderName(const Peer *who, QString customText)
{
    this->moveCursor(QTextCursor::End);
    QColor color_ = this->textColor();
    this->setTextColor( getColorByPeer(who) );

    //print msg + set aligment
    this->append(QTime::currentTime().toString("hh:mm") + ((who) ? (" ~" + who->getName()) : " Du") + customText +":");
    this->setAlignment( (who) ? Qt::AlignLeft : Qt::AlignRight );

    this->moveCursor(QTextCursor::End);
    QTextCursor cursor(this->textCursor());
    cursor.select(QTextCursor::SelectionType::LineUnderCursor);

    QTextCharFormat charFormat = cursor.charFormat();
    charFormat.setFont(QFont(/*"Times"*/ "Open Sans", 8, QFont::Bold));
    cursor.setCharFormat(charFormat);

    //set old color + scroll down
    this->setTextColor(color_);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());
}


QColor ChatWidget::getColorByPeer(const Peer *peer)
{
    if( ! colorMap.contains(peer) )
        setNewColorToPeer(peer);
    return colorMap[peer];
}

void ChatWidget::setNewColorToPeer(const Peer *peer)
{
    size_t ColorListLength = (sizeof(colorList)/sizeof(*colorList));
    colorMap.insert(peer, colorList[ ( colorCounter % ColorListLength ) ] );
    colorCounter++;
}


