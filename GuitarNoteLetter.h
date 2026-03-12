#pragma once
#include <QVector>

enum GuitarNoteLetter {
    A,
    Ad,
    B,
    C,
    Cd,
    D,
    Dd,
    E,
    F,
    Fd,
    G,
    Gd
};

static int guitarNoteLetterCount = 12;

static QVector<QString> noteNames = {
   "A", "A#", "B", "C", "C#", "D", "D#",
   "E", "F", "F#", "G", "G#"
};
