#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include "FretboardScene.h"
#include "GuitarNoteLetter.h"

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow), m_noteNames(12), m_labels(12)
{
   ui->setupUi(this);

   auto v = QVector<QAbstractButton*>(
      {
         ui->checkBox, ui->checkBox_2, ui->checkBox_3, ui->checkBox_4,
         ui->checkBox_5, ui->checkBox_6, ui->checkBox_7, ui->checkBox_8,
         ui->checkBox_9, ui->checkBox_10, ui->checkBox_11, ui->checkBox_12
      }
   );
   for (int i = 0; i < 12; ++i)
   {
      ui->buttonGroup->setId(v[i], i);
   }

   m_noteNames[A] = "A";
   m_noteNames[Ad] = "A#";
   m_noteNames[B] = "B";
   m_noteNames[C] = "C";
   m_noteNames[Cd] = "C#";
   m_noteNames[D] = "D";
   m_noteNames[Dd] = "D#";
   m_noteNames[E] = "E";
   m_noteNames[F] = "F";
   m_noteNames[Fd] = "F#";
   m_noteNames[G] = "G";
   m_noteNames[Gd] = "G#";
   m_labels[0] = ui->label;
   m_labels[1] = ui->label_2;
   m_labels[2] = ui->label_3;
   m_labels[3] = ui->label_4;
   m_labels[4] = ui->label_5;
   m_labels[5] = ui->label_6;
   m_labels[6] = ui->label_7;
   m_labels[7] = ui->label_8;
   m_labels[8] = ui->label_9;
   m_labels[9] = ui->label_10;
   m_labels[10] = ui->label_11;
   m_labels[11] = ui->label_12;
   updateLabels(0);

   scene = new FretboardScene(this);
   ui->graphicsView->setScene(scene);

   ui->buttonGroup->setExclusive(false);
   connect(ui->buttonGroup, &QButtonGroup::idClicked, this, [this](int noteNumber) {
      bool isNoteChosen = ui->buttonGroup->button(noteNumber)->isChecked();
      scene->changeNote(noteNumber, isNoteChosen);
   });
}

MainWindow::~MainWindow()
{
   delete ui;
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
   scene->changeTonic(index);
   updateLabels(index);
}

void MainWindow::updateLabels(int tonic)
{
   for (int i = 0; i < 12; ++i)
   {
      m_labels[i]->setText(m_noteNames[(tonic + i) % 12]);
   }
}

