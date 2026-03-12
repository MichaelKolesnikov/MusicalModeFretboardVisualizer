#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include "FretboardScene.h"

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   scene = new FretboardScene(this);
   ui->graphicsView->setScene(scene);

   ui->buttonGroup->setExclusive(false);
   connect(ui->buttonGroup, &QButtonGroup::idClicked, this, [this](int clickedId) {
      int noteNumber = -clickedId - 2;
      bool isNoteChosen = ui->buttonGroup->button(clickedId)->isChecked();
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
}

