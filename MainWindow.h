#pragma once

#include <QMainWindow>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FretboardScene;
class QLabel;
class QAbstractButton;
class NoteLetter;

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

private slots:
   void on_comboBox_currentIndexChanged(int index);

   void on_pushButton_clicked();

   void on_actionShowEqualModeTable_triggered();

private:
   void updateLabels(int tonic);
   QMap<QString, QVector<int>> loadModesFromJson(const QJsonDocument &doc);
   void loadModes();
   QSet<NoteLetter> modeNotes(NoteLetter tonic, const QVector<int>& mode);

   Ui::MainWindow *ui;
   FretboardScene* scene;
   QVector<QLabel*> m_labels;
   QVector<QAbstractButton*> m_checkboxes;
   QMap<QString, QVector<int>> m_modes;
   QVector<QString> m_sortedModeNames;
   void setMode(const QString &modeName);
};
