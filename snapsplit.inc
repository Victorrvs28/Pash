std::filesystem::path listPath = std::filesystem::path(pathstr) / "SNAPLIST.tmp";
std::filesystem::path scriptPath = std::filesystem::path(pathstr) / "snap.sh";
std::string cmd = "snap list > '" + listPath.string() + "'";
if (std::system(cmd.c_str()) != 0) {
    qDebug() << "snap-mark failed";
}
else {
    std::ifstream listFile(listPath);
    std::ofstream List(scriptPath);


    if (!listFile.is_open() || !List.is_open()) {
        qDebug() << "could not open snap.sh";
    }
    else {
        List << "#!/bin/bash\n";
        List << "sudo apt snap install -y ";
        std::string line;
        while (std::getline(listFile, line)) {
            List << line << '\n';

        }
        qDebug() << "finished writing snap.sh";
    }

    listFile.close();
    List.close();
    std::filesystem::remove(listPath);
}
