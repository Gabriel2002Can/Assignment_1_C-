#include <iostream>
#include <fstream>
#include <string>
#include <exception>

using namespace std;

struct MyFileException : public exception {

    const string what() {
        return "Name of file invalid\n";
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

    try {
        do {

            cout << "Please enter the path of the input file" << endl;

            //fileInputName = "../file/OriginalCPP.cpp";
            getline(cin, fileInputName);
            inStream.open(fileInputName);

            //If input file is open do
            if (inStream.is_open()) {
                string line;

                do {

                    cout << "\nPlease enter the path of the output file" << endl;

                    //fileOutputName = "../file/output.html";
                    getline(cin, fileOutputName);
                    outStream.open(fileOutputName, ios::app);

                    //If output file is open do
                    if (outStream.is_open()) {

                        outStream << "<PRE>" << endl;

                        //Until the file finishes do
                        while (!inStream.eof()) {

                            //Get line and add content of line to line variable
                            getline(inStream, line);

                            //Replace the necessary values and add them to outputString variable
                            outputString = replace(line);

                            //Write outputString value into output file
                            outStream << outputString << endl;
                        }
                        outStream << "</PRE>";
                        inStream.close();
                        outStream.close();
                        break;
                    }

                    //If output file doesn't open
                    cout << "Output file name invalid" << endl << "Please try again\n" << endl;

                } while (true);
                break;
            }

            //If input file doesn't open
            cout << "Unable to open input file" << endl << "Please try again\n" << endl;

        } while (true);

        //Making sure that the files are closed
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
