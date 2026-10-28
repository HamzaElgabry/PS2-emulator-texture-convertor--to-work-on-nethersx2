#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QCoreApplication>
#include <filesystem>
#include <regex>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("PS2 Texure convertor");
    resize(1000, 600);

    setMinimumSize(1000, 600);

    // Set the top-left window icon 
    setWindowIcon(QIcon("images/window.jpg"));

    //  Create and name the central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    setCentralWidget(centralWidget);

    // Stylesheet with background image
    QString ps2RetroStyle = R"(
        QWidget#centralWidget {
            /* Replace with your resource path (e.g., ':/images/crt_bg.png')
               or a relative path to your compiled folder ('background.png') */
            border-image: url(images/bkg.jpg) 0 0 0 0 stretch stretch;
        }
        QWidget {
            background: transparent; /* Ensures child widgets don't block the background */
            color: #d0d7e5;
            font-family: 'Consolas', 'Segoe UI', monospace;
            font-size: 13px;
        }
        QLabel {
            color: #00e5ff;
            font-weight: bold;
        }
        QLineEdit {
            background-color: #171a26;
            color: #00e5ff;
            border: 2px solid #2a324b;
            border-radius: 0px;
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
            border-radius: 2px;
            padding: 6px 12px;
            font-weight: bold;
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
            border-radius: 0px;
            border: 2px solid #3b4252;
            background: #171a26;
        }
        QRadioButton::indicator:checked {
            background: #27ae60;
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

    this->setStyleSheet(ps2RetroStyle);

    // Set up your layout 
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Set spacing between your rows so they aren't squished together
    mainLayout->setSpacing(20);

    // Add a stretch at the TOP to push the UI down from the ceiling
    mainLayout->addStretch();





    // Source Folder Row
    QHBoxLayout *sourceLayout = new QHBoxLayout();
    sourceLineEdit = new QLineEdit(this);
    sourceLineEdit->setPlaceholderText("Select source texture folder...");
    QPushButton *sourceBtn = new QPushButton("Browse...", this);
    sourceLayout->addWidget(new QLabel("Source:", this));
    sourceLayout->addWidget(sourceLineEdit);
    sourceLayout->addWidget(sourceBtn);
    mainLayout->addLayout(sourceLayout);

    // Destination Folder Row
    QHBoxLayout *destLayout = new QHBoxLayout();
    destLineEdit = new QLineEdit(this);
    destLineEdit->setPlaceholderText("Select output folder...");
    QPushButton *destBtn = new QPushButton("Browse...", this);
    destLayout->addWidget(new QLabel("Dest:", this));
    destLayout->addWidget(destLineEdit);
    destLayout->addWidget(destBtn);
    mainLayout->addLayout(destLayout);

    // Mode Selection (Forward / Reverse)
    QHBoxLayout *modeLayout = new QHBoxLayout();
    forwardRadio = new QRadioButton("convert to old format(to work on nethersx2)", this);
    reverseRadio = new QRadioButton("convert to new format(to work on pcsx2)", this);
    forwardRadio->setChecked(true);
    modeLayout->addWidget(forwardRadio);
    modeLayout->addWidget(reverseRadio);
    mainLayout->addLayout(modeLayout);

    // Convert Button
    convertButton = new QPushButton("START CONVERSION", this);
    convertButton->setStyleSheet(
        "background-color: #005b96;"
        "color: #ffffff;"
        "font-weight: bold;"
        "font-size: 14px;"
        "padding: 10px;"
        "border: 2px solid #00e5ff;"
        "border-radius: 2px;"
        );
    mainLayout->addWidget(convertButton);

    // Progress bar & Status
    progressBar = new QProgressBar(this);
    progressBar->setValue(0);
    mainLayout->addWidget(progressBar);

    statusLabel = new QLabel("Ready.", this);
    mainLayout->addWidget(statusLabel);

    // (or stays nicely balanced when full-screened)
    mainLayout->addStretch();

    setCentralWidget(centralWidget);

    // Connect signals and slots
    connect(sourceBtn, &QPushButton::clicked, this, &MainWindow::browseSource);
    connect(destBtn, &QPushButton::clicked, this, &MainWindow::browseDest);
    connect(convertButton, &QPushButton::clicked, this, &MainWindow::startConversion);
}

MainWindow::~MainWindow() {}

void MainWindow::browseSource() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Source Folder");
    if (!dir.isEmpty()) {
        sourceLineEdit->setText(dir);
        // Auto-suggest destination folder name
        if (destLineEdit->text().isEmpty()) {
            destLineEdit->setText(dir + "_converted");
        }
    }
}

void MainWindow::browseDest() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select output Folder");
    if (!dir.isEmpty()) {
        destLineEdit->setText(dir);
    }
}

void MainWindow::startConversion() {
    QString sourceStr = sourceLineEdit->text();
    QString destStr = destLineEdit->text();

    if (sourceStr.isEmpty() || destStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select both source and destination folders.");
        return;
    }

    bool reverseMode = reverseRadio->isChecked();
    runConversionLogic(sourceStr, destStr, reverseMode);
}

void MainWindow::runConversionLogic(const QString &sourceStr, const QString &destStr, bool reverseMode) {
    fs::path sourceDir(sourceStr.toStdString());
    fs::path destDir(destStr.toStdString());

    if (!fs::exists(sourceDir)) {
        QMessageBox::critical(this, "Error", "Source directory does not exist.");
        return;
    }

    convertButton->setEnabled(false);
    statusLabel->setText("Analyzing and converting files...");

    std::regex pattern("-([0-9a-fA-F]{8})(\\.(?:png|dds))$");

    // Count files first for progress bar
    int totalFiles = 0;
    for (auto const& entry : fs::recursive_directory_iterator(sourceDir)) {
        if (fs::is_regular_file(entry)) totalFiles++;
    }

    if (totalFiles == 0) {
        QMessageBox::information(this, "Notice", "No files found in the source directory.");
        convertButton->setEnabled(true);
        statusLabel->setText("Ready.");
        return;
    }

    progressBar->setMaximum(totalFiles);
    int processedCount = 0;
    int convertedCount = 0;

    for (auto const& entry : fs::recursive_directory_iterator(sourceDir)) {
        if (!fs::is_regular_file(entry)) continue;
        processedCount++;

        fs::path filePath = entry.path();
        std::string fileName = filePath.filename().string();
        std::smatch match;
        std::string newName = fileName;

        if (std::regex_search(fileName, match, pattern)) {
            std::string hexStr = match[1].str();
            std::string extStr = match[2].str();
            
            uint64_t currentVal = std::stoull(hexStr, nullptr, 16);
            uint64_t newVal = currentVal;
            bool shouldProcess = false;

            if (!reverseMode) {
                if (((currentVal >> 12) & 0xFULL) <= 2) {
                    newVal = (currentVal + 0x4000ULL) & 0xFFFFFFFFULL;
                    shouldProcess = true;
                }
            } else {
                if (currentVal >= 0x4000ULL) {
                    uint64_t testVal = (currentVal - 0x4000ULL) & 0xFFFFFFFFULL;
                    if (((testVal >> 12) & 0xFULL) <= 2) {
                        newVal = testVal;
                        shouldProcess = true;
                    }
                }
            }

            if (shouldProcess) {
                std::stringstream ss;
                ss << std::hex << std::setw(8) << std::setfill('0') << newVal;
                std::string prefix = match.prefix().str();
                newName = prefix + "-" + ss.str() + extStr;
                convertedCount++;
            }
        }

        fs::path relativePath = fs::relative(filePath, sourceDir);
        fs::path targetFile = destDir / relativePath.parent_path() / newName;

        try {
            fs::create_directories(targetFile.parent_path());
            fs::copy_file(filePath, targetFile, fs::copy_options::overwrite_existing);
        } catch (...) {
            // Ignore individual file copy errors to keep process running
        }

        progressBar->setValue(processedCount);
        QCoreApplication::processEvents(); // Keeps GUI responsive
    }

    convertButton->setEnabled(true);
    statusLabel->setText(QString("Complete! Processed: %1 | Converted: %2").arg(totalFiles).arg(convertedCount));
    QMessageBox::information(this, "Success", QString("Conversion finished successfully!\nConverted %1 of %2 files.").arg(convertedCount).arg(totalFiles));
}
