#ifndef S_FILEHANDLER_H
#define S_FILEHANDLER_H

class filehandler{
private:
    std::string path;
    std::string user;

public:
    filehandler(std::string);
    ~filehandler();
    void create_usr_dir(std::string, std::string);

};


#endif /* S_FILEHANDLER.H*/