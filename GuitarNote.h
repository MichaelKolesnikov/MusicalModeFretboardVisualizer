#pragma once
#include "GuitarNoteLetter.h"

class GuitarNote 
{
public:
    explicit GuitarNote(GuitarNoteLetter letter, unsigned int octaveNumber): letter(letter), octaveNumber(octaveNumber) {}

    GuitarNote(unsigned int guitarNoteNumber): letter(GuitarNoteLetter(guitarNoteNumber % guitarNoteLetterCount)), octaveNumber(guitarNoteNumber / guitarNoteLetterCount) {}

    GuitarNoteLetter getGuitarNoteLetter() const 
    {
        return letter;
    }

    unsigned int getOctaveNumber() const 
    {
        return octaveNumber;
    }

    unsigned int toInt() const 
    {
        return octaveNumber * guitarNoteLetterCount + letter;
    }

    GuitarNote operator+(const GuitarNote& other) const 
    {
        return toInt() + other.toInt();
    }

    GuitarNote operator+(const int& other) const 
    {
        return toInt() + other;
    }
private:
    GuitarNoteLetter letter;
    unsigned int octaveNumber;
};