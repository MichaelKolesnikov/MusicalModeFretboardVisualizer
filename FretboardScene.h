#pragma once
#include <QGraphicsScene>

class QGraphicsEllipseItem;

class FretboardScene : public QGraphicsScene
{
   Q_OBJECT
public:
   explicit FretboardScene(QObject *parent = nullptr);

   void changeTonic(int tonic);
   void changeNote(int note, bool add = true);

private:
   static constexpr qreal SEMITONE_RATIO = 1.0594630943592953;
   static double semitoneUp(double freq);
   static bool hasFretMarker(int fretNumber);
   void hideAllPoints();

   QVector<QVector<QGraphicsEllipseItem*>> m_noteToPoints;
   int m_tonic;
   QVector<bool> m_isNoteChosen; // regarding tonic
};
