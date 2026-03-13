#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "FretboardScene.h"
#include "NoteLetter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QVector>
#include <QString>
#include <QFile>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow), m_labels(12)
{
   ui->setupUi(this);

   m_checkboxes = QVector<QAbstractButton*>(
      {
         ui->checkBox, ui->checkBox_2, ui->checkBox_3, ui->checkBox_4,
         ui->checkBox_5, ui->checkBox_6, ui->checkBox_7, ui->checkBox_8,
         ui->checkBox_9, ui->checkBox_10, ui->checkBox_11, ui->checkBox_12
      }
   );
   for (int i = 0; i < 12; ++i)
   {
      ui->buttonGroup->setId(m_checkboxes[i], i);
   }

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

   loadModes();
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
   for (int i = 0; i < NoteLetter::count; ++i)
   {
      m_labels[i]->setText(NoteLetter(tonic + i).name());
   }
}

QMap<QString, QVector<int>> MainWindow::loadModesFromJson(const QJsonDocument& doc)
{
   QMap<QString, QVector<int>> modesMap;

   if (!doc.isObject()) return modesMap;

   QJsonObject obj = doc.object();
   if (!obj.contains("modes")) return modesMap;

   QJsonArray modesArray = obj["modes"].toArray();

   for (const QJsonValue& modeValue : modesArray) {
      if (!modeValue.isObject()) continue;

      QJsonObject modeObj = modeValue.toObject();
      QString name = modeObj["name"].toString();
      QJsonArray notesArray = modeObj["notes"].toArray();

      QVector<int> notes;
      for (const QJsonValue& noteValue : notesArray) {
         notes.append(noteValue.toInt());
      }

      modesMap[name] = notes;
      QPushButton* modeButton = new QPushButton(name, ui->modesGroupBox);
      ui->modesGroupBox->layout()->addWidget(modeButton);

      connect(modeButton, &QPushButton::clicked, this, [this, name]() {
         setMode(name);
      });
   }

   return modesMap;
}

void MainWindow::loadModes()
{
   QFile file(":/modes.json");
   if (file.open(QIODevice::ReadOnly)) {
      QByteArray data = file.readAll();
      QJsonDocument doc = QJsonDocument::fromJson(data);
      m_modes = loadModesFromJson(doc);
      file.close();
   }
}


void MainWindow::setMode(const QString& modeName)
{
   on_pushButton_clicked();

   QVector<int> modeNotes = m_modes[modeName];
   for (int noteIndex : modeNotes)
   {
      if (noteIndex < 12)
      {
         ui->buttonGroup->button(noteIndex)->setChecked(true);
         scene->changeNote(noteIndex, true);
      }
   }
}


void MainWindow::on_pushButton_clicked()
{
   for (int i = 0; i < m_checkboxes.size(); ++i)
   {
      m_checkboxes[i]->setChecked(false);
      scene->changeNote(i, false);
   }
}

