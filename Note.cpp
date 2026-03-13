#include "Note.h"

Note::Note(NoteLetter letter, int octaveNumber): m_letter(letter), m_octaveNumber(octaveNumber) {}

Note::Note(int noteNumber): m_letter(noteNumber), m_octaveNumber(noteNumber / NoteLetter::count) {}

NoteLetter Note::noteLetter() const
{
   return m_letter;
}

int Note::octaveNumber() const
{
   return m_octaveNumber;
}

Note::operator int() const
{
   return m_octaveNumber * NoteLetter::count + m_letter;
}

Note &Note::operator++() {
   *this = *this + 1;
   return *this;
}

Note Note::operator++(int)
{
   Note old = *this;
   *this = *this + 1;
   return old;
}

bool Note::isButtonOnPianoWhite() const
{
   static int mask = 0b010110101101;
   int bitNumber = this->operator int() % NoteLetter::count;
   int res1 = (mask >> bitNumber);
   int res2 = (res1 & 1);
   return 0 != res2;
}
