#include "FretboardScene.h"
#include "GuitarNote.h"
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>

FretboardScene::FretboardScene(QObject *parent)
   : QGraphicsScene{parent}, m_noteToPoints(12), m_tonic(0), m_isNoteChosen(12, false)
{
   qreal fretboardLength = 2048;
   qreal height = 400;
   int fretCount = 25;
   qreal ellipseSize = 16;

   qreal indentation = height / 20;
   qreal heightForStrings = height - 2 * indentation;
   qreal move = heightForStrings / 5;

   QVector<int> stringsNotes = {E, B, G, D, A, E};
   QVector<qreal> stringsFrequencies = {329.63, 246.94, 196.00, 146.82, 110.00, 82.41};
   // frequency = sqrt(tension / linearDensity) / (2 * length) = stringConstant / (2 * length)
   QVector<qreal> stringsConstants;
   for (int i = 0; i < stringsFrequencies.size(); ++i)
   {
      stringsConstants.push_back(stringsFrequencies[i] * 2 * fretboardLength);
   }

   // fretboard wood painting
   QLinearGradient woodGradient(0, 0, 0, height);
   woodGradient.setColorAt(0.0, QColor(120, 80, 50));
   woodGradient.setColorAt(1.0, QColor(70, 45, 30));
   addRect(0, 0, fretboardLength, height, QPen(), QBrush(woodGradient));

   // frets painting
   qreal currentFrequency = stringsFrequencies[0];
   qreal previousX = 0, length, x;
   qreal fretNumbersY = -indentation * 2;
   for (int fretNumber = 1; fretNumber < fretCount; ++fretNumber)
   {
      currentFrequency = semitoneUp(currentFrequency);
      length = stringsConstants[0] / (2 * currentFrequency);
      x = fretboardLength - length;
      QLinearGradient fretGradient(x, 0, x, height);
      fretGradient.setColorAt(0.0, QColor(240, 240, 240));
      fretGradient.setColorAt(0.3, QColor(190, 190, 190));
      fretGradient.setColorAt(0.7, QColor(140, 140, 140));
      fretGradient.setColorAt(1.0, QColor(100, 100, 100));
      addLine(x, 0, x, height, QPen(QBrush(fretGradient), 2));
      auto textItem = addText(QString::number(fretNumber));
      textItem->setPos((x + previousX) / 2 - textItem->boundingRect().width() / 2, fretNumbersY);
      if (hasFretMarker(fretNumber))
      {
         addEllipse((x + previousX) / 2 - ellipseSize / 2, height / 2 - ellipseSize / 2, ellipseSize, ellipseSize, QPen(), Qt::black);
      }
      else if (fretNumber == 12)
      {
         qreal ellipseY = indentation + move / 2;
         addEllipse((x + previousX) / 2 - ellipseSize / 2, ellipseY - ellipseSize / 2, ellipseSize, ellipseSize, QPen(), Qt::black);
         addEllipse((x + previousX) / 2 - ellipseSize / 2, height - ellipseY - ellipseSize / 2, ellipseSize, ellipseSize, QPen(), Qt::black);
      }
      previousX = x;
   }
   currentFrequency = semitoneUp(semitoneUp(semitoneUp(semitoneUp(stringsFrequencies[5]))));
   length = stringsConstants[4] / (2 * currentFrequency);
   x = fretboardLength - length;
   auto textItem = addText("0");
   textItem->setPos(x / 2 - textItem->boundingRect().width() / 2, fretNumbersY);
   addRect(x, 0, -x, height);
   qreal x_ = x;

   // strings paintings
   qreal currentStringHeight = indentation;
   for (int i = 0; i < 6; ++i)
   {
      addLine(0, currentStringHeight, fretboardLength, currentStringHeight, QPen(QColor(180, 180, 180), i));
      currentStringHeight += move;
   }

   // note's points creating
   currentStringHeight = indentation;
   for (int stringNumber = 0; stringNumber < stringsFrequencies.size(); ++stringNumber)
   {
      int currentNote = stringsNotes[stringNumber];
      m_noteToPoints[currentNote].append(
         addEllipse(x_ / 2 - ellipseSize / 2, currentStringHeight - ellipseSize / 2, ellipseSize, ellipseSize, QPen(), Qt::red)
      );

      currentFrequency = stringsFrequencies[stringNumber];
      previousX = 0;
      for (int fretNumber = 1; fretNumber < fretCount; ++fretNumber)
      {
         currentFrequency = semitoneUp(currentFrequency);
         currentNote = (currentNote + 1) % 12;
         length = stringsConstants[stringNumber] / (2 * currentFrequency);
         x = fretboardLength - length;
         m_noteToPoints[currentNote].append(
            addEllipse((x + previousX) / 2 - ellipseSize / 2, currentStringHeight - ellipseSize / 2, ellipseSize, ellipseSize, QPen(), Qt::red)
         );
         auto textItem = addText(noteNames[currentNote]);
         textItem->setPos(x - textItem->boundingRect().width(), currentStringHeight - textItem->boundingRect().height());
         previousX = x;
      }

      currentStringHeight += move;
   }
   hideAllPoints();
}

void FretboardScene::changeTonic(int tonic)
{
   if (m_tonic == tonic)
   {
      return;
   }
   hideAllPoints();
   m_tonic = tonic;
   for (int noteNumber_ = 0; noteNumber_ < m_isNoteChosen.size(); ++noteNumber_)
   {
      if (m_isNoteChosen[noteNumber_])
      {
         int noteNumber = (noteNumber_ + m_tonic) % 12;
         for (auto it : m_noteToPoints[noteNumber])
         {
            it->show();
         }
      }
   }
}

void FretboardScene::changeNote(int note, bool add)
{
   if (m_isNoteChosen[note] == add)
   {
      return;
   }
   m_isNoteChosen[note] = add;
   int realNote = (note + m_tonic) % 12;
   for (auto it : m_noteToPoints[realNote])
   {
      it->setVisible(add);
   }
}

qreal FretboardScene::semitoneUp(qreal freq) {
   return freq * SEMITONE_RATIO;
}

bool FretboardScene::hasFretMarker(int fretNumber)
{
   constexpr uint32_t FRET_MARKS = (1<<3) | (1<<5) | (1<<7) | (1<<9) | (1<<15) | (1<<17) | (1<<19);
   return FRET_MARKS & (1u << fretNumber);
}

void FretboardScene::hideAllPoints()
{
   for (auto& v : m_noteToPoints)
   {
      for (auto it : v)
      {
         it->hide();
      }
   }
}
