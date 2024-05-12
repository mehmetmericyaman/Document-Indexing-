#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <regex>
#include <ctime>
#include <dirent.h>
#include <queue>

using namespace std;

class DocumentIndexer {
private:
    unordered_map<string, vector<string>> index;

public:
    void indexDocuments(const string& directory) {
        regex word_regex("[a-z A-Z]+");
        smatch match;

        DIR* dir;
        struct dirent* entry;

        if ((dir = opendir(directory.c_str())) != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type == DT_REG) { // Regular file
                    string filename = entry->d_name;
                    ifstream file(directory + "/" + filename);
                    if (file.is_open()) {
                        string line;
                        while (getline(file, line)) {
                            istringstream iss(line);
                            string word;
                            while (iss >> word) {
                                if (regex_search(word, match, word_regex)) {
                                    string key = match[0];
                                    transform(key.begin(), key.end(), key.begin(), ::tolower);
                                    index[key].push_back(filename);
                                }
                            }
                        }
                        file.close();
                    } else {
                        cout << "Error opening file: " << filename << endl;
                    }
                }
            }
            closedir(dir);
        } else {
            cout << "Error opening directory: " << directory << endl;
        }
    }

    void retrieveDocuments(const string& keyword) {
        if (index.find(keyword) != index.end()) {
            cout << "Documents containing the keyword '" << keyword << "':" << endl;
            for (const auto& doc : index[keyword]) {
                cout << "- " << doc << endl;
            }
        } else {
            cout << "No documents found containing the keyword '" << keyword << "'." << endl;
        }
    }
    void printMostCommonWords() {
        // Öncelik kuyruğu tanımla: sıklığı büyükten küçüğe doğru sıralıyoruz
        priority_queue<pair<int, string>> pq;
        
        // Her kelimenin sıklığını hesapla ve öncelik kuyruğuna ekle
        for (const auto& pair : index) {
            pq.push({pair.second.size(), pair.first});
        }

        // En sık kullanılan 10 kelimeyi ekrana yazdır
        cout << "Top 10 most common words:" << endl;
        for (int i = 0; i < 10 && !pq.empty(); ++i) {
            cout << "- " << pq.top().second << ": " << pq.top().first << " occurrences" << endl;
            pq.pop();
        }
    }


    void printLeastCommonWords() {
        // Öncelik kuyruğu tanımla: sıklığı küçükten büyüğe doğru sıralıyoruz
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        
        // Her kelimenin sıklığını hesapla ve öncelik kuyruğuna ekle
        for (const auto& pair : index) {
            pq.push({pair.second.size(), pair.first});
        }

        // En az kullanılan 10 kelimeyi ekrana yazdır
        cout << "Top 10 least common words:" << endl;
        for (int i = 0; i < 10 && !pq.empty(); ++i) {
            cout << "- " << pq.top().second << ": " << pq.top().first << " occurrences" << endl;
            pq.pop();
        }
    }
};

int main() {
    string directory = "AllDocs"; // Folder name
    DocumentIndexer indexer;
    indexer.indexDocuments(directory);

    int option;
    string keyword;
    do {
        cout << "\nSimple Document Retrieval System (30 Minutes)\n"
                "1. Enter a single keyword to list the document(s)(file names)\n"
                "2. Print the top 10 words that appeared most frequently\n"
                "3. Print the top 10 words that appeared least frequently\n"
                "4. Exit\n"
                "Option: ";
        cin >> option;

        switch (option) {
            case 1:
                cout << "Enter keyword: ";
                cin >> keyword;
                indexer.retrieveDocuments(keyword);
                break;
            case 2:
                indexer.printMostCommonWords();
                break;
            case 3:
                indexer.printLeastCommonWords();
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid option!\n";
        }
    } while (option != 4);

    return 0;
}
