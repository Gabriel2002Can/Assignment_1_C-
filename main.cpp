#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <regex>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

struct MyFileException : public exception {

    const string what() {
        return "File Name/Path is invalid\n";
    }
};

string replace(string line) {
    string resultString = "";

    for (char i : line) {
        string fileChar = string(1, i);

        if (fileChar == "<") {
            resultString += "&lt;";
        } else if (fileChar == ">") {
            resultString += "&gt;";
        } else {
            resultString += fileChar;
        }
    }
    return resultString;
}

int main() {

    ofstream outStream;
    ifstream inStream;

    string outputString;
    string fileInputName;
    string fileOutputName;

    //The regex is a mix of two regex masks, such as, the file name validator https://regexpattern.com/valid-filename/, and the path validator https://learn.microsoft.com/en-us/answers/questions/862484/regex-to-validate-a-valid-windows-folder-directory
    //I also added the validation for reserved words, such as CON PRN.
    //Then I added the character number limitation manually based on the character limitation https://learn.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=registry
    //Was used for verification https://regex101.com/
    regex regexMask(R"(^(?![a-zA-Z]:\\)(?!\\)(?!.*[<>:"|?*])(?!.*\.$)(?!.*\.\.$)(?!.*\b(CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])\b)[^<>:"|?*]{1,255}(\\[^<>:"|?*]{1,255})*$)");

    try {
        do {

            cout << "Please enter the path of the input file" << endl;

            //fileInputName = "../file/OriginalCPP.cpp";
            getline(cin, fileInputName);
            inStream.open(fileInputName);

            //https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c

            if (regex_match(fileInputName,regexMask) && fs::exists(fileInputName)) {
                string line;

                do {

                    cout << "\nPlease enter the path of the output file" << endl;

                    //fileOutputName = "../file/output.html";
                    getline(cin, fileOutputName);
                    outStream.open(fileOutputName, ios::app);

                    //https://forkful.ai/en/cpp/files-and-io/checking-if-a-directory-exists/ reference
                    fs::path outputPath(fileOutputName);
                    fs::path parentDir = outputPath.parent_path();

                    if (regex_match(fileOutputName, regexMask) && (parentDir.empty() || fs::exists(parentDir))) {

                        if (!inStream.is_open() || !outStream.is_open()) {
                            throw MyFileException();
                        }

                        outStream << "<PRE>" << endl;

                        while (!inStream.eof()) {

                            getline(inStream, line);

                            outputString = replace(line);

                            outStream << outputString << endl;
                        }
                        outStream << "</PRE>";

                        inStream.close();
                        outStream.close();
                        break;
                    }

                    cout << "Output file name invalid" << endl << "Please try again\n" << endl;

                } while (true);
                break;
            }

            cout << "Unable to open input file" << endl << "Please try again\n" << endl;

        } while (true);

        if (inStream.is_open()) {
            inStream.close();
        }
        if (outStream.is_open()) {
            outStream.close();
        }

    }catch (MyFileException& e) {
        cout << e.what() << endl;
    }catch (ios_base::failure& e) {
        cout << "An unexpected error occurred with your files"; //https://en.cppreference.com/w/cpp/io/ios_base/failure Used as reference
    }catch (exception& e) {
        cout << "An unexpected error occurred: " << endl;
    }

    return 0;
}
