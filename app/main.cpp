#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QPropertyAnimation>
#include <QScreen>




int main(int argc, char *argv[]) {
    QApplication app(argc, argv);


    // 1. Create a custom 800x400 splash window (Frameless & Always on Top)
    QWidget *splash = new QWidget(nullptr, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    splash->setFixedSize(800, 400);

    // Center the splash window on the screen
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    splash->move((screenGeometry.width() - 800) / 2, (screenGeometry.height() - 400) / 2);

    // Load image and scale it precisely to 800x400
    QPixmap pixmap("images/logo.jpg");
    QLabel *label = new QLabel(splash);
    label->setPixmap(pixmap.scaled(800, 400, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    label->setGeometry(0, 0, 800, 400);
    label->setAlignment(Qt::AlignCenter);

    splash->show();

    // --- PS2 RETRO TECH STYLE SHEET ---
    QString ps2RetroStyle = R"(
    QMainWindow {
        background-color: #0f111a;
    }
    QWidget {
        background-color: #0f111a;
        color: #d0d7e5;
        font-family: 'Consolas', 'Segoe UI', monospace;
        font-size: 13px;
    }
    QLabel {
        color: #00e5ff; /* Classic PS2 / Terminal Teal Cyan */
        font-weight: bold;
    }
    QLineEdit {
        background-color: #171a26;
        color: #00e5ff;
        border: 2px solid #2a324b;
        border-radius: 0px; /* Sharp retro corners */
        padding: 6px;
    }
    QLineEdit:focus {
        border: 2px solid #00e5ff;
        background-color: #1c2233;
    }
    QPushButton {
        background-color: #1a202c;
        color: #e2e8f0;
        border: 2px solid #3b4252;
        border-radius: 2px; /* Slightly sharp buttons */
        padding: 6px 12px;
        font-weight: bold;
        text-transform: uppercase;
    }
    QPushButton:hover {
        background-color: #242c3d;
        border-color: #00e5ff;
        color: #ffffff;
    }
    QPushButton:pressed {
        background-color: #005b96;
        border-color: #00e5ff;
        color: #ffffff;
    }
    QRadioButton {
        color: #d0d7e5;
        spacing: 8px;
    }
    QRadioButton::indicator {
        width: 14px;
        height: 14px;
        border-radius: 0px; /* Square retro radio box */
        border: 2px solid #3b4252;
        background: #171a26;
    }
    QRadioButton::indicator:checked {
        background: #27ae60; /* Retro Matrix/Terminal Green */
        border: 2px solid #2ecc71;
    }

    QProgressBar {
        background-color: #171a26;
        border: 2px solid #3b4252;
        border-radius: 0px;
        text-align: center;
        color: #ffffff;
        font-weight: bold;
    }
    QProgressBar::chunk {
        background-color: #27ae60;
        border-radius: 0px;
    }
    QMessageBox {
        background-color: #0f111a;
    }
)";

    app.setStyleSheet(ps2RetroStyle);
    // ----------------------------------------

    MainWindow window;


    //  Wait 2 seconds, show main window, and smoothly fade out the custom splash screen
    QTimer::singleShot(2000, [splash, &window]() {
        // 1. Set main window opacity to 0 and show it invisibly
        window.setWindowOpacity(0.0);
        window.show();

        // Fade out the splash screen
        QPropertyAnimation *splashAnim = new QPropertyAnimation(splash, "windowOpacity");
        splashAnim->setDuration(800);
        splashAnim->setStartValue(1.0);
        splashAnim->setEndValue(0.0);

        // Simultaneously fade in the main window
        QPropertyAnimation *windowAnim = new QPropertyAnimation(&window, "windowOpacity");
        windowAnim->setDuration(800);
        windowAnim->setStartValue(0.0);
        windowAnim->setEndValue(1.0);

        // Clean up the splash screen memory when the fade finishes
        QObject::connect(splashAnim, &QPropertyAnimation::finished, [splash, splashAnim]() {
            splash->close();
            splash->deleteLater();
            splashAnim->deleteLater();
        });

        // Start both animations at the exact same time
        splashAnim->start();
        windowAnim->start();
    });




    return app.exec();
}
