#include "NoteLetter.h"

const char* NoteLetter::name() const
{
   static const char* names[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
   return names[*this];
}
