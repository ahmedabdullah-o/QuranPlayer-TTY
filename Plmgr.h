#ifndef PLMGR_H
#define PLMGR_H
#include<filesystem>
#include<iostream>
#include<string>
#include<utility>
#include<fstream>
#include<vector>
#include<stdexcept>
using namespace std;
namespace fs = std::filesystem;

//Added feature!
//Files are synced instantly with every edit!
//No need for save and load functions ✔

class Plmgr {
    private:
        //playlists[playlist index][audio file index].
        //NOTE: playlist[n][0] is playlist name.
        vector<pair<string, vector<string>>> playlists;

        int util_get_index(string name){
            for (int i = 0; i < playlists.size(); i++){
                if (playlists[i].second[0] == name)
                    return i;
            }
            return -1;
        }

        void util_populate_playlist(int index){
            ifstream file(playlists[index].first);
            if (!file.is_open()){
                throw runtime_error("File exists but can't open file");
            } else {
                string line = "";
                //Importing audio paths into playlists[n][1, n]
                while(getline(file, line)){
                    if (line[0] != '#' && line.length() > 0){
                        if (fs::exists(line) && fs::is_regular_file(line)){
                            playlists[index].second.push_back(line);
                        }
                    }
                }
            }
        }

        bool util_add_to_playlists (string path, string name){
            for (int i = 0; i < playlists.size(); i++){
                if (name == playlists[i].second[0]){
                    while (true){
                        string choice = 0;
                        cerr<<"A playlist with the same name already exists.\n"
                            <<"[1] Change name\n[2] Skip playlist\n"
                            <<"Choice: "; cin>>choice;
                        if (choice == "1"){
                            string alter_name = "";
                            cout<<"Enter name: "; cin>>alter_name;
                            name = alter_name;
                            if (name != playlists[i].second[0]){
                                playlists.push_back(
                                    {path, vector<string>(0)}
                                );
                                playlists[playlists.size() - 1].second.push_back(name);
                                util_populate_playlist(util_get_index(name));
                                break;
                            }
                        } else if (choice == "2"){
                            return 1;
                        } else {
                            cerr<<"Wrong choice\n";
                        }
                    }
                }
            }
            playlists.push_back(
                {path, vector<string>(0)}
            );
            playlists[playlists.size() - 1].second.push_back(name);
            util_populate_playlist(util_get_index(name));
            return 0;
        }

        void util_load_playlists(){
            //iterate over files in playlists folder
            for (const auto& entry : fs::directory_iterator("./playlists")) {
                //check file extension
                if (fs::is_regular_file(entry)){
                    if (entry.path().extension().string() == ".apl"){
                        //add playlist to the vector
                        util_add_to_playlists(entry.path().string(), entry.path().stem().string());
                    } else {
                        while(true){
                            string choice = "";
                            cerr<<"Found non-playlist file:\n"<<entry<<'\n'
                                <<"Do you want to include as playlist(faulty)? {y, n}:"; cin>>choice;
                            if (choice == "y"){
                                util_add_to_playlists(entry.path().string(), entry.path().stem().string());
                                break;
                            } else if (choice == "n"){
                                break;
                            } else {
                                cerr<<"Wrong choice\n";
                            }
                        }
                    }
                }
            }
        }

        void util_loadfiles(){
            //make sure the playlists folder exists.
            if (fs::exists("./playlists")){
                cout<<"playlists folder found.\n";
                //load playlists.
                util_load_playlists();
            } else {
                //init the dir.
                cerr<<"playlists folder not found.\n";
                if (fs::create_directory("./playlists")){
                    cout<<"Blank playlists folder created successfully.\n";
                    //load playlists
                    util_load_playlists();
                } else {
                    throw runtime_error("Unable to create playlists folder!\n");
                }
            }
        }

    public:
        //Constructor
        Plmgr(){
            util_loadfiles();
        }
        //[1]
        void append_pl(){
            string name = "";
            while(true){
                cout<<"Enter Name:";cin>>name;
                bool skip = 0;
                for (const auto& entry : fs::directory_iterator("./playlists")) {
                    if (name == entry.path().stem().string()){
                        string choice = "";
                        cerr<<"This name already exists!\n"
                            <<"Change Name? (y, n):";cin>>choice;
                        if (choice == "y"){
                            string alter_name = "";
                            cout<<"Name:";cin>>alter_name;
                        } else {
                            skip = true;
                            break;
                        }
                    }
                    if (skip) break;
                }
                if (skip) break;
                else {
                    ofstream file("./playlists/"+name+".apl");
                    util_add_to_playlists("./playlists/"+name+".apl", name);
                    break;
                }
            }
        }
        //[2]
        void append_audio(){
            string playlist_name = "", path = "";
            cout<<"Select playlist (by name):";cin>>playlist_name;
            if (fs::exists("./playlists/"+name+".apl")){
                cout<<"Enter audio path:";cin>>path;
                if (fs::exists(path)){
                    ofstream file("./playlists/"+name+".apl", std::ios::app);
                    file<<path<<endl;
                    playlists[util_get_index(name)].second.push_back(path);
                    break;
                } else {
                    cerr<<"This audio path doesn't exist!\n";
                }
            } else {
                cerr<<"This playlist name doesn't exist!\n";
            }
        }
        //[11]
        void remove_pl(){
            string playlist_name = "";
            if (fs::exists("./playlists"+playlist_name+".apl")){
                string choice = "";
                cerr<<"Are you sure you want to delete that file (non-revertable)? {y, n}:";cin>>choice;
                if (choice == "y"){
                    if (std::filesystem::remove(file_path)){
                        cout<<"File deleted successfully.\n";
                    } else {
                        cerr<<"Unable to delete the file!\n";
                    }
                } else {
                    cout<<"Canceled.\n";
                }
            }
        }
        //[3]
        void remove_audio(){
            
        }
        //[4]
        void order(){

        }
        //[5]
        void print_all_pl(){

        }
        //[6]
        void print_all_audio(){

        }
        //[7]
        void print_from_pl(){

        }
        //[8]
        void play(){

        }
        //[10]
        void load_external(){

        }

};

#endif