#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QProgressBar>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void browseSource();
    void browseDest();
    void startConversion();

private:
    QLineEdit *sourceLineEdit;
    QLineEdit *destLineEdit;
    QRadioButton *forwardRadio;
    QRadioButton *reverseRadio;
    QPushButton *convertButton;
    QLabel *statusLabel;
    QProgressBar *progressBar;

    void runConversionLogic(const QString &source, const QString &dest, bool reverseMode);
};
