#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FretboardScene;
class QLabel;

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();

private slots:
   void on_comboBox_currentIndexChanged(int index);

private:
   void updateLabels(int tonic);

   Ui::MainWindow *ui;
   FretboardScene* scene;
   QVector<QString> m_noteNames;
   QVector<QLabel*> m_labels;
};
