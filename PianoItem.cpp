#include "PianoItem.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QBrush>

PianoItem::PianoItem(Note startNote, Note endNote, QGraphicsItem* parent)
   : QGraphicsObject(parent), m_startNote(startNote), m_endNote(endNote)
{
   qreal whiteWidth = 64;
   qreal whiteHeight = whiteWidth * 4;
   qreal blackWidth = whiteWidth * 0.6;
   qreal blackHeight = whiteHeight * 0.7;
   QRectF whiteButtonRect = QRectF(-whiteWidth / 2, 0, whiteWidth, whiteHeight);
   QRectF blackButtonRect = QRectF(-blackWidth / 2, 0, blackWidth, blackHeight);
   int x = 0;
   for (auto note = startNote; note <= endNote; ++note)
   {
      if (note.isButtonOnPianoWhite())
      {
         m_pianoButtons.append(new QGraphicsRectItem(whiteButtonRect, this));
         m_pianoButtons.back()->setX(x);
         m_pianoButtons.back()->setBrush(Qt::white);
         x += whiteButtonRect.width();
         m_pianoButtons.back()->setZValue(0);
         auto textItem = new QGraphicsTextItem(note.noteLetter().name(), m_pianoButtons.back());
         textItem->setX(-textItem->boundingRect().width() / 2);
         textItem->setY(m_pianoButtons.back()->boundingRect().height() - textItem->boundingRect().height());
      }
      else
      {
         m_pianoButtons.append(new QGraphicsRectItem(blackButtonRect, this));
         m_pianoButtons.back()->setX(x - whiteWidth / 2);
         m_pianoButtons.back()->setBrush(Qt::black);
         m_pianoButtons.back()->setZValue(1);
         auto textItem = new QGraphicsTextItem(note.noteLetter().name(), m_pianoButtons.back());
         textItem->setX(-textItem->boundingRect().width() / 2);
         textItem->setY(m_pianoButtons.back()->boundingRect().height() - textItem->boundingRect().height());
         textItem->setDefaultTextColor(Qt::white);
      }
   }
}

QRectF PianoItem::boundingRect() const
{
   QRectF totalRect;
   for (auto item : m_pianoButtons) {
       totalRect |= item->sceneBoundingRect();
   }
   return totalRect;
}

void PianoItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void PianoItem::setPressed(Note note, bool pressed)
{
   m_pianoButtons[note - m_startNote]->setBrush(pressed ? Qt::green : (note.isButtonOnPianoWhite() ? Qt::white : Qt::black));
}

void PianoItem::setPressed(NoteLetter noteLetter, bool pressed)
{
   for (auto note = m_startNote; note <= m_endNote; ++note)
   {
      if (note % NoteLetter::count == noteLetter)
      {
         m_pianoButtons[note - m_startNote]->setBrush(pressed ? Qt::green : (note.isButtonOnPianoWhite() ? Qt::white : Qt::black));
      }
   }
}

void PianoItem::clearPressed()
{
   for (auto note = m_startNote; note <= m_endNote; ++note)
   {
      m_pianoButtons[note - m_startNote]->setBrush(note.isButtonOnPianoWhite() ? Qt::white : Qt::black);
   }
}
