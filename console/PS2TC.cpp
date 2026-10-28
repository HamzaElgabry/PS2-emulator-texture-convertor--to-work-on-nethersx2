#include <iostream>
#include <filesystem>
#include <regex>
#include <string>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

void convertTextures(const fs::path& sourceDir, const fs::path& destDir, bool reverseMode) {
    if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
        std::cerr << "Error: Source directory does not exist or is not a directory.\n";
        return;
    }

    std::regex pattern("-([0-9a-fA-F]{8})(\\.(?:png|dds))$");

    int totalFiles = 0;
    int convertedCount = 0;

    std::cout << "\nScanning and processing textures (" << (reverseMode ? "Reverse Mode" : "Forward Mode") << ")...\n";

    for (auto const& entry : fs::recursive_directory_iterator(sourceDir)) {
        if (!fs::is_regular_file(entry)) continue;
        totalFiles++;

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
                // Forward Logic: Original -> NetherSX2
                if (((currentVal >> 12) & 0xFULL) <= 2) {
                    newVal = (currentVal + 0x4000ULL) & 0xFFFFFFFFULL;
                    shouldProcess = true;
                }
            } else {
                // Reverse Logic: NetherSX2 -> Original (Subtract 0x4000)
                // We check if subtracting keeps it valid or matches the shifted range
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
                std::string newHex = ss.str();

                std::string prefix = match.prefix().str();
                newName = prefix + "-" + newHex + extStr;
                convertedCount++;
            }
        }

        fs::path relativePath = fs::relative(filePath, sourceDir);
        fs::path targetFile = destDir / relativePath.parent_path() / newName;

        try {
            fs::create_directories(targetFile.parent_path());
            fs::copy_file(filePath, targetFile, fs::copy_options::overwrite_existing);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to copy/rename file " << fileName << ": " << e.what() << "\n";
        }
    }

    std::cout << "\n==============================\n";
    std::cout << "Conversion Complete!\n";
    std::cout << "Total files processed: " << totalFiles << "\n";
    std::cout << "Total files converted: " << convertedCount << "\n";
    std::cout << "==============================\n";
}

int main() {
    std::string sourcePath, destPath;
    int modeChoice = 0;

    std::cout << "=== Nether Texture Converter (Two-Way Console) ===\n";
    std::cout << "Select Mode:\n";
    std::cout << "  [1] (convert textures to work for nethersx2)\n";
    std::cout << "  [2] Reverse (convert textures to workfor pcsx2)\n";
    std::cout << "Enter choice (1 or 2): ";
    std::cin >> modeChoice;
    std::cin.ignore(); // Clear newline buffer

    bool reverseMode = (modeChoice == 2);

    std::cout << "Enter path to source texture folder: ";
    std::getline(std::cin, sourcePath);
    
    std::cout << "Enter path to output folder: ";
    std::getline(std::cin, destPath);

    convertTextures(sourcePath, destPath, reverseMode);
    
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}