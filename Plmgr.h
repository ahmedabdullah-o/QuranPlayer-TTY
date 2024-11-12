#ifndef PLMGR_H
#define PLMGR_H
#include<filesystem>
#include<iostream>
#include<fstream>
#include<vector>

class Plmgr {
    private:
        std::vector<std::string> paths;
    
    public:
        Plmgr(){
            // Loading the file or loading if it doesn't exist
            std::ifstream custom_file("./custom");
            if (!custom_file) { // If the file doesn't exist
                std::cout<<"Generating custom path file...\n";
                std::ofstream custom_file_out("./custom");
            } else {
                
                std::cout<<"Loading custom paths.\n";
                std::string path = "";
                while (std::getline(custom_file, path)){
                    if (path.length() > 0 && path[0] != '#'){
                        if (std::filesystem::exists(path)){
                            paths.push_back(path);
                        } else {
                            int choice = 0;
                            std::cout<<"Wrong path detected:\n"<<path<<"\n[1] Fix Path.\n[2] Delete Path.\n";
                            while (true){
                                if (choice == 1){
                                    std::string buffer = "";
                                    std::cout<<"Enter the right path: "; cin>>buffer;
                                    if (std::filesystem::exists(buffer)){
                                        break;
                                    } else {
                                        std::cout<<buffer<<" is a wrong path!\n";
                                    }
                                } else if (choice == 2){
                                    //deletion
                                }
                            }
                        }
                    }
                }   
            }
        }
};

#endif