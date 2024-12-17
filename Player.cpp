#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <Windows.h>
#include <mmsystem.h>
#include <limits>

#pragma comment(lib, "winmm.lib") // Linker directive to winmm

namespace fs = std::filesystem;

int main() {
    fs::path directory_path = "Songs"; // "Songs" | Folder of music files, WAV only
    std::vector<std::string> songs; // Vector container which stores strings of file names 

    for (const auto& entry : fs::directory_iterator(directory_path)) { // Iterates through the entries in the directory
        if (entry.is_regular_file() && // Makes sure only actual files are considered
            (entry.path().extension() == ".wav")) { // Checks for the .wav file extension
            songs.push_back(entry.path().filename().string()); // If the file is a wav, convert the file name to a string and push it to the end
        }
    }

    while (true) {
        std::vector<std::string> matched_songs; // Stores songs that match the search
        int match_count = 0; // Count of matches, will incrament
        int choice = -1; // Stores the users choice, starts at -1 to make sure a song isnt selected in the beginning of the loop

        while (true) { 
            matched_songs.clear(); // Resets the matched songs so the user can search again
            match_count = 0; // Resets the count

            std::string searchedSong; // Declears searchedSong variable as a string
            std::cout << "Enter the song you would like to play: "; // Prompts the user for input
            std::getline(std::cin, searchedSong);  // Input as searchedSong
            std::cout << "The song you searched for is: " << searchedSong << "\n"; // Returns the song the user searched for

            int song_num = 1; // Starts at one to display next to each song that will be returned with the search
            for (const auto& song : songs) { // Iterates through each song in the songs vector
                if (song.find(searchedSong) != std::string::npos) { // Checks if searchedSong is found in song from songs
                    matched_songs.push_back(song); // Adds the match the matched_songs vector
                    std::cout << song_num << ". " << song << std::endl; // Prints "#. Song Name" for each
                    ++song_num; // Increment song number
                    ++match_count; // Increment amount of matcches
                }
            }

            if (match_count == 0) { // If no mathces
                std::cout << "No matches found. Please try again.\n"; // Return no match line
                continue; // Restarts the search
            } else {
                std::cout << "\nTotal matches found: " << match_count << std::endl; // Displays amount of matches
            }

            while (true) { // Runs until a valid input is received
                std::cout << "Enter the number of the song you want to play (1-" << matched_songs.size() << "): "; // Propmt a user to type a number, dynamiclly displays the range
                std::cin >> choice; // Stores the input

                if (std::cin.fail() || choice < 1 || choice > matched_songs.size()) { // If there was any input readying the input | Checks if the number is greater than one but less than the size of matches
                    std::cin.clear(); // Clears the error the so the user can restart if an invalid input is entered
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discards remaining input
                    std::cout << "Invalid input. Please enter a number between 1 and " << matched_songs.size() << ".\n"; // Re prompt user
                } else {
                    break; // Break if input is valid
                }
            }

            std::cout << "You selected: " << matched_songs[choice - 1] << "\n"; // Returns the song and index based on the users choice

            std::string selected_song = matched_songs[choice - 1];  // Stored in selected_song
            std::string full_path = directory_path.string() + "\\" + selected_song; // Constructs the full path of the selected song

            std::cout << "Playing music: " << selected_song << "\n"; // Returns the currently playing song
            PlaySound(full_path.c_str(), NULL, SND_FILENAME | SND_SYNC); // Plays the full_path, specifies argument filename, SYNC will make sure program will wait until the song is over
            std::cout << "Music over\n"; // After song ends

            break; // Exits while true
        }

        char play_again; // var to store input
        std::cout << "Do you want to search and play another song? (y/n): "; // prompt for input
        std::cin >> play_again; // recieves input

        if (play_again != 'y' && play_again != 'Y') { // Checks if user returned YES
            std::cout << "Goodbye!\n"; // Exit message
            break; // Exits while true
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clears input buffer
    }    
    return 0;
}