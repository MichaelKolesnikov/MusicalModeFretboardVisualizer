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
#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QPlainTextEdit>

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

      QVector<int> currentMode = {};
      for (int i = 0; i < m_checkboxes.size(); ++i)
      {
         if (m_checkboxes[i]->isChecked())
         {
            currentMode.append(i);
         }
      }
      bool unknownMode = true;
      for (const auto& modeName : m_modes.keys())
      {
         if (m_modes[modeName] == currentMode)
         {
            ui->modeLabel->setText(modeName);
            unknownMode = false;
            break;
         }
      }
      if (unknownMode)
      {
         ui->modeLabel->setText("UnknownMode");
      }
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

      m_sortedModeNames.push_back(name);
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

QSet<NoteLetter> MainWindow::modeNotes(NoteLetter tonic, const QVector<int> &mode)
{
   QSet<NoteLetter> modeNotes;
   for (auto distance : mode)
   {
      modeNotes.insert(tonic + distance);
   }
   return modeNotes;
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
   ui->modeLabel->setText(modeName);
}


void MainWindow::on_pushButton_clicked()
{
   for (int i = 0; i < m_checkboxes.size(); ++i)
   {
      m_checkboxes[i]->setChecked(false);
      scene->changeNote(i, false);
   }
   ui->modeLabel->setText("Empty");
}


void MainWindow::on_actionShowEqualModeTable_triggered()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Equal Mode Table");
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    auto modeNames = m_sortedModeNames;
    modeNames.resize(7);

    QSet<QSet<QPair<NoteLetter, int>>> equalTable;

    for (int note = 0; note < NoteLetter::count; ++note)
    {
       for (int modeNumber = 0; modeNumber < modeNames.size(); ++modeNumber)
       {
          QSet<QPair<NoteLetter, int>> curSet;
          auto notes = modeNotes(note, m_modes[modeNames[modeNumber]]);
          curSet.insert({note, modeNumber});

          for (int note = 0; note < NoteLetter::count; ++note)
          {
             for (int modeNumber = 0; modeNumber < modeNames.size(); ++modeNumber)
             {
                auto secondNotes = modeNotes(note, m_modes[modeNames[modeNumber]]);
                if (notes == secondNotes)
                {
                   curSet.insert({note, modeNumber});
                }
             }
          }

          if (curSet.size() > 1)
          {
             equalTable.insert(curSet);
          }
       }
    }

   QList<QSet<QPair<NoteLetter, int>>> sortedTable = equalTable.values();

   std::sort(
      sortedTable.begin(),
      sortedTable.end(),
      [](const QSet<QPair<NoteLetter, int>>& a, const QSet<QPair<NoteLetter, int>>& b)
      {
         int minNoteA = 12;
         int minNoteB = 12;

         for (const auto& pair : a) {
             if (int(pair.first) < minNoteA) {
                 minNoteA = int(pair.first);
             }
         }

         for (const auto& pair : b) {
             if (int(pair.first) < minNoteB) {
                 minNoteB = int(pair.first);
             }
         }

         if (minNoteA != minNoteB) return minNoteA < minNoteB;

         int minModeA = 7;
         int minModeB = 7;

         for (const auto& pair : a) {
             if (int(pair.first) == minNoteA && pair.second < minModeA) {
                 minModeA = pair.second;
             }
         }

         for (const auto& pair : b) {
             if (int(pair.first) == minNoteB && pair.second < minModeB) {
                 minModeB = pair.second;
             }
         }

         return minModeA < minModeB;
      }
   );

   QString fullText;

   for (const auto& set : sortedTable)
   {
      QStringList namedEntries;
      QStringList numberedEntries;

      QList<QPair<NoteLetter, int>> sortedPairs = set.values();
      std::sort(sortedPairs.begin(), sortedPairs.end(),
          [](const QPair<NoteLetter, int>& a, const QPair<NoteLetter, int>& b) {
              if (a.first != b.first) return a.first < b.first;
              return a.second < b.second;
      });

      for (const auto& pair : sortedPairs)
      {
          NoteLetter note = pair.first;
          int modeIdx = pair.second;
          QString modeName = modeNames[modeIdx];

          namedEntries << QString("%1 (%2)").arg(note.name()).arg(modeName);

          numberedEntries << QString("%1(%2)").arg(int(note)).arg(modeIdx);
      }

      fullText += namedEntries.join(" = ") + "\n";
      fullText += numberedEntries.join(" = ") + "\n\n";
   }


   QPlainTextEdit *textEdit = new QPlainTextEdit(dialog);
   textEdit->setPlainText(fullText);
   textEdit->setReadOnly(true);
   textEdit->setWordWrapMode(QTextOption::WordWrap);

   QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
   mainLayout->addWidget(textEdit);

   dialog->resize(800, 600);
   dialog->show();
}

