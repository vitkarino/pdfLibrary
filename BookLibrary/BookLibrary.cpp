#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>

namespace fs = std::filesystem;
std::string currentPath;
std::vector<fs::path> filePaths;

void addPath();
void menu();
void showFiles();
void actions();
void clearScreen();

void menu()
{
    clearScreen();
    std::cout << "Welcome to pdfLibrary!\n";
    std::cout << "\n1. Change library path\n";
    std::cout << "2. List all files in library\n";
    std::cout << "3. Exit the program\n";

    int choice;

    std::cout << "\nEnter your choice: ";

    while (!(std::cin >> choice)) {
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the incorrect input
        std::cout << "\nInvalid input. Please enter a number: ";
    }

    std::cin.ignore();

    switch (choice)
    {
        case 1:
			addPath();
			break;
        case 2:
            showFiles();
			break;
        case 3:
            std::cout << "\nExiting...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            exit(0);
            break;
        default:
            menu();
    }

}

void savePathToFile(const std::string& path) {
    std::ofstream out("path.txt");
    if (out.is_open()) {
        out << path;
        out.close();
    }
}

bool loadPathFromFile(std::string& path) {
    std::ifstream in("path.txt");
    if (in.is_open()) {
        getline(in, path);
        in.close();
        return true;
    }
    return false;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
#endif
}

void showFiles() {

    clearScreen();
    filePaths.clear(); 

    if (currentPath.empty()) {
        std::cout << "\nPath has not been set. Please set the path first.\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        menu();
        return;
    }

    std::cout << "Files found in " << currentPath << ":\n\n";
    int fileNumber = 1;

    for (const auto& entry : fs::directory_iterator(currentPath)) {
        if (entry.path().extension() == ".pdf") {
            std::cout << fileNumber << ". " << entry.path().filename() << std::endl;
            filePaths.push_back(entry.path()); 
            fileNumber++;
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    actions(); 
}

void actions() {
    std::cout << "\nActions:\n";
    std::cout << "\n1. Open file\n";
    std::cout << "2. Delete file\n";
    std::cout << "3. Back\n";

    int choice;

    std::cout << "\nEnter your choice: ";

    while (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nInvalid input. Please enter a number: ";
    }

    switch (choice) {
    case 1:
    case 2:
    {
        if (filePaths.empty()) {
            std::cout << "\nNo files listed. Please list files first.\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            showFiles();
            return;
        }

        std::cout << "\nEnter the file number: ";
        int fileNumber;
        std::cin >> fileNumber;
        std::cin.ignore();

        if (fileNumber < 1 || fileNumber > filePaths.size()) {
            std::cout << "\nInvalid file number.\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            actions();
            return;
        }

        fs::path selectedFile = filePaths[fileNumber - 1];

        if (choice == 1) {
            std::string command = "start \"\" \"" + selectedFile.string() + "\"";
            system(command.c_str());
            std::cout << "\nOpening file...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            clearScreen();
            showFiles();
            actions();
        }
        else if (choice == 2) {
            try {
                fs::remove(selectedFile);
                std::cout << "\nFile deleted successfully.\n";
            }
            catch (const fs::filesystem_error& e) {
                std::cout << "\nError deleting file: " << e.what() << '\n';
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));
            clearScreen();
            showFiles();
            actions();
        }
    }
    break;
    case 3:
        menu();
        return;
    default:
        actions();
    }
}


void addPath()
{
    clearScreen();
    while (true) {
		
		std::string inputPath;
		std::cout << "Please choose a path for the library. Example: C:\\Users\\Username\\Downloads\n" << "Path: ";

		std::getline(std::cin, inputPath);
        if (inputPath.empty()) {
            clearScreen();
            continue;
        }
        if (!fs::exists(inputPath)) {
            clearScreen();
			std::cout << "\nPath not found." << std::endl;
		}
        else {
            currentPath = inputPath;
            savePathToFile(currentPath);
			std::cout << "\nPath found successfully " << inputPath << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			menu();
			return;
		}
	}
}

int main()
{
    loadPathFromFile(currentPath);
	menu();
	return 0;
}