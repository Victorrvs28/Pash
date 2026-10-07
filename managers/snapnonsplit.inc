std::filesystem::path listPath = std::filesystem::path(pathstr) / "SNAPLIST.tmp";
std::filesystem::path scriptPath = std::filesystem::path(pathstr) / "installEverything.sh";
std::string cmd = "snap list | awk 'NR>1 {print $1}'  > '" + listPath.string() + "'";
if (std::system(cmd.c_str()) != 0) {
    qDebug() << "snap-mark failed";
}
else {

    std::ifstream listFile(listPath);
    std::ofstream List;
    List.open(scriptPath, std::ios::out|std::ios::app);


    if (!listFile.is_open() || !List.is_open()) {
        qDebug() << "could not open installEverything.sh";
    }
    else {

        List << "#!/bin/bash\n";
        std::string line;
        List << "sudo snap install ";

        while (std::getline(listFile, line)) {
            List << line << ' ';

        }
    }

    listFile.close();
    List.close();
    std::filesystem::remove(listPath);
}
