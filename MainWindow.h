#pragma once

#include <QMainWindow>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FretboardScene;
class QLabel;
class QAbstractButton;

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

private slots:
   void on_comboBox_currentIndexChanged(int index);

   void on_pushButton_clicked();

private:
   void updateLabels(int tonic);
   QMap<QString, QVector<int>> loadModesFromJson(const QJsonDocument &doc);
   void loadModes();

   Ui::MainWindow *ui;
   FretboardScene* scene;
   QVector<QString> m_noteNames;
   QVector<QLabel*> m_labels;
   QVector<QAbstractButton*> m_checkboxes;
   QMap<QString, QVector<int>> m_modes;
   void setMode(const QString &modeName);
};
