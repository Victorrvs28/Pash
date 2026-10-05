#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <QString>
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCommandLineParser parser;

    parser.addOption({"just-graph", "Just graphical programs "});
    parser.addOption({"non-graph", "Just non graphical programs"});
    parser.addOption({"split", "splits in two automations (programs|non programs)"});
    parser.addOption({ {"directory", "dir"}, "directory wich the automations will be touched", "directory"});

    parser.parse(app.arguments());

    bool splitted = parser.isSet("split");
    bool nonProg = parser.isSet("non-graph");
    bool justProg = parser.isSet("just-graph");
    QString path = parser.value("directory");
    string pathstr = path.toStdString();

    if(splitted == true){ // it will be in the documentation when support more package managers.
        if(std::filesystem::exists("/usr/bin/apt")){

            string touch = "touch '" + pathstr + "apt.sh'";
            qDebug()<<"touching apt automation";
            int commandReturn = system(touch.c_str());
            if(commandReturn != 0){
                qDebug()<<"error on touching apt automation: "<<commandReturn;
                                   }
            else{
            qDebug()<<"finished touching apt automation"
                                                   "";
                }

        }
        qDebug()<<"Okey will split it in two automations.";
    }
    else{
        if(nonProg == true && justProg == false){
            qDebug()<<"leaved as just non-programs.";

        }
        else if(justProg == true && nonProg == false){
            qDebug()<<"leaved as just programs. ";
        }

    }

    if(splitted == false && nonProg == false && justProg == false){
        string touch = "touch installEverything.sh";
        system(touch.c_str());
        std::ofstream output("installEverything.sh");
        if (!output.is_open()) {
            qDebug() << "was not possible make installEverything.sh";
            return 1;
        }
        if(std::filesystem::exists("/usr/bin/apt")){
            qDebug()<<"apt exists.";
            string list = "apt-mark showmanual > APTLIST.tmp";
            system(list.c_str());
            std::string line;
            std::ifstream listFile("APTLIST.tmp");
            if (!listFile.is_open()) {
                qDebug() << "Was not possible open APTLIST.tmp";
                return 1;
            }

            while(std::getline(listFile, line)){

                    output << "sudo apt install " << line << '\n';

            }
            listFile.close();
            output.close();

        }
    }
    return 0;
}
