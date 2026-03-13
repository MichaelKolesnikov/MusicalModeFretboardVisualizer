#pragma once
#include <QGraphicsObject>
#include "Note.h"

class PianoItem : public QGraphicsObject
{
public:
   explicit PianoItem(Note startNote, Note endNote, QGraphicsItem* parent = nullptr);

   QRectF boundingRect() const;
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

   void setPressed(Note note, bool pressed);
   void setPressed(NoteLetter noteLetter, bool pressed);
   void clearPressed();

private:
   Note m_startNote, m_endNote;
   QRectF m_rect;
   QVector<QGraphicsRectItem*> m_pianoButtons;
};

