#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <QString>
#include <string>
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCommandLineParser parser;


    parser.addOption({"split", "splits in two automations (programs|non programs)"});
    parser.addOption({ {"directory", "dir"}, "directory wich the automations will be touched", "directory"});

    parser.parse(app.arguments());

    bool splitted = parser.isSet("split");
    QString path = parser.value("directory");
    string pathstr = path.toStdString();

    if(splitted == true){
        if(std::filesystem::exists("/usr/bin/apt")){
            #include "aptsplit.inc"
        }
        //ADD MORE PACKAGE MANAGERS HERE!!
    }


    if(splitted == false){
        std::string touch = "touch "+ pathstr +"installEverything.sh";
        std::system(touch.c_str());
        std::ofstream output(pathstr + "installEverything.sh");
        if (!output.is_open()) {
            qDebug() << "was not possible make installEverything.sh";
            return 1;
        }
        if(std::filesystem::exists("/usr/bin/apt")){
            #include "aptnonsplit.inc"
        }
    //ADD PACKAGE MANAGERS HERE!!!
    } // splitted
    return 0;
}
