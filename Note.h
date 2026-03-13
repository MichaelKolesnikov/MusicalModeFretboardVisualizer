#pragma once
#include "NoteLetter.h"

class Note
{
public:
   explicit Note(NoteLetter letter, int octaveNumber);

   Note(int noteNumber);

   NoteLetter noteLetter() const;

   int octaveNumber() const;

   operator int() const;

   Note& operator++();

   Note operator++(int);

   bool isButtonOnPianoWhite() const;

private:
    NoteLetter m_letter;
    int m_octaveNumber;
};
