#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

// ANSI color codes
const string COLOR_RESET = "\x1b[0m";
const string COLOR_GREEN = "\x1b[32m";
const string COLOR_CYAN = "\x1b[36m";
const string COLOR_YELLOW = "\x1b[33m";
const string COLOR_RED = "\x1b[31m";

class Question {
public:
    string question;
    vector<string> options;
    int correctOption;

    Question(const string& q, const vector<string>& opts, int correct)
        : question(q), options(opts), correctOption(correct) {}
};

class LeaderboardEntry {
public:
    string name;
    float score;

    LeaderboardEntry(const string& n, float s) : name(n), score(s) {}
};

class QuizGame {
private:
    vector<Question> questions;
    vector<LeaderboardEntry> leaderboard;

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void initializeLeaderboard() {
        ifstream inFile("leaderboard.txt");
        if (!inFile) {
            ofstream outFile("leaderboard.txt");
            if (outFile) {
                outFile << "Ale 5.0\nAnto 4.0\nSara 3.5\n";
            }
        }
    }

    void loadQuestions() {
        questions = {
            {"What is the capital of Spain?", {"Berlin", "Madrid", "Paris", "Rome"}, 1},
            {"Which planet is the closest to the sun?", {"Earth", "Mercury", "Jupiter", "Venus"}, 1},
            {"Who was the Ancient Greek God of the Sun?", {"Ares", "Poseidon", "Apollo", "Artemis"}, 2},
            {"How many dots appear on a pair of dice?", {"50", "46", "38", "42"}, 3},
            {"What city is known as The Eternal City?", {"Roma", "Bucharest", "London", "Amsterdam"}, 0},
            {"On which continent would you find the world's largest desert?", {"Africa", "Australia", "Antarctica", "Asia"}, 2},
            {"What is the capital of Ireland?", {"Dublin", "London", "Paris", "Rome"}, 0},
            {"How many European capitals does the Danube flow through?", {"5", "6", "3", "4"}, 3},
            {"On which continent would you find the city of Baku?", {"Europe", "Asia", "America", "Australia"}, 1},
            {"In what year did World War II end?", {"1943", "1944", "1945", "1946"}, 2},
            {"Which country has won the most World Cups?", {"Brazil", "UK", "France", "Spain"}, 0},
            {"What color are Mickey Mouse's shoes?", {"Red", "Black", "Blue", "Yellow"}, 3},
            {"How many colors are in the rainbow?", {"8", "6", "7", "9"}, 2},
            {"What is the name of the dog who was one of the first animals in space?", {"Rosco", "Laika", "Layla", "Leo"}, 1},
            {"When was the Romanian Revolution?", {"1989", "1990", "1998", "1988"}, 0}
        };
    }

    void useFiftyFifty(const Question& q, vector<bool>& remainingOptions) {
        int removed = 0;
        for (size_t i = 0; i < remainingOptions.size(); ++i) {
            if (i != static_cast<size_t>(q.correctOption) {
                remainingOptions[i] = false;
                if (++removed >= 2) break;
            }
        }
    }

    string readUserName() {
        string name;
        cout << COLOR_CYAN << "Enter your name: " << COLOR_RESET;
        cin >> name;
        return name;
    }

    bool askQuestion(const Question& q, int qNumber, float& score) {
        vector<bool> remainingOptions(4, true);
        int choice, use5050;

        cout << "\n" << COLOR_YELLOW << "Question " << qNumber << ": " << q.question << COLOR_RESET << endl;
        for (size_t j = 0; j < q.options.size(); ++j) {
            if (remainingOptions[j])
                cout << (j + 1) << ". " << q.options[j] << endl;
        }

        do {
            cout << COLOR_CYAN << "Use 50/50? (1 = Yes, 0 = No): " << COLOR_RESET;
            cin >> use5050;
        } while (use5050 != 0 && use5050 != 1);

        if (use5050) {
            useFiftyFifty(q, remainingOptions);
            cout << "\nAfter 50/50 lifeline:\n";
            for (size_t j = 0; j < q.options.size(); ++j) {
                if (remainingOptions[j])
                    cout << (j + 1) << ". " << q.options[j] << endl;
            }
        }

        do {
            cout << COLOR_CYAN << "Your answer (1-4): " << COLOR_RESET;
            cin >> choice;
        } while (choice < 1 || choice > 4 || !remainingOptions[choice - 1]);

        if (choice - 1 == q.correctOption) {
            cout << COLOR_GREEN << "Correct!\n" << COLOR_RESET;
            score += use5050 ? 0.5 : 1.0;
            return true;
        } else {
            cout << COLOR_RED << "Wrong! The correct answer was: " 
                 << q.options[q.correctOption] << COLOR_RESET << endl;
            return false;
        }
    }

    void saveScore(const string& name, float score) {
        ofstream leaderboardFile("leaderboard.txt", ios::app);
        if (leaderboardFile) {
            leaderboardFile << name << " " << score << "\n";
        }

        ofstream outputFile("output.txt");
        if (outputFile) {
            time_t now = time(nullptr);
            char timestamp[26];
            ctime_s(timestamp, sizeof(timestamp), &now);

            outputFile << "˗ˏˋ ★ ˎˊ˗ Game Session Summary ˗ˏˋ ★ ˎˊ˗\n"
                       << "Player Name:「 ✦ " << name << " ✦ 」\n"
                       << "Final Score: " << score << "\n"
                       << "Date & Time: " << timestamp;
        }
    }

    void loadLeaderboard() {
        leaderboard.clear();
        ifstream file("leaderboard.txt");
        string name;
        float score;

        while (file >> name >> score) {
            leaderboard.emplace_back(name, score);
        }

        sort(leaderboard.begin(), leaderboard.end(),
            [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
                return a.score > b.score;
            });

        ofstream outFile("leaderboard.txt");
        for (const auto& entry : leaderboard) {
            outFile << entry.name << " " << entry.score << "\n";
        }
    }

public:
    QuizGame() {
        initializeLeaderboard();
        loadQuestions();
    }

    void startGame() {
        clearScreen();
        cout << "▄▀▄▀▄ " << COLOR_GREEN << "START GAME" << COLOR_RESET << " ▄▀▄▀▄\n";
        float score = 0;
        string name = readUserName();

        for (size_t i = 0; i < questions.size(); ++i) {
            if (!askQuestion(questions[i], i + 1, score)) {
                cout << COLOR_RED << "\nGame over! You answered incorrectly.\n" << COLOR_RESET;
                break;
            }
        }

        saveScore(name, score);
        cout << "\n" << COLOR_YELLOW << "Your final score: " << score << "\n" << COLOR_RESET;
        waitForEnter();
    }

    void viewLeaderboard() {
        clearScreen();
        cout << "▄▀▄▀▄ " << COLOR_GREEN << "LEADERBOARD" << COLOR_RESET << " ▄▀▄▀▄\n";
        loadLeaderboard();

        if (leaderboard.empty()) {
            cout << "No scores recorded yet.\n";
        } else {
            for (const auto& entry : leaderboard) {
                cout << COLOR_YELLOW << entry.name << COLOR_RESET << " - " << entry.score << " points\n";
            }
        }
        waitForEnter();
    }

    void viewGameHistory() {
        clearScreen();
        cout << "▄▀▄▀▄ " << COLOR_GREEN << "GAME HISTORY" << COLOR_RESET << " ▄▀▄▀▄\n";
        string searchName;
        cout << COLOR_CYAN << "Enter username: " << COLOR_RESET;
        cin >> searchName;

        loadLeaderboard();
        vector<LeaderboardEntry> userEntries;

        for (const auto& entry : leaderboard) {
            if (entry.name == searchName) {
                userEntries.push_back(entry);
            }
        }

        if (userEntries.empty()) {
            cout << COLOR_RED << "No records found for " << searchName << ".\n" << COLOR_RESET;
        } else {
            for (const auto& entry : userEntries) {
                cout << COLOR_YELLOW << entry.name << COLOR_RESET << " - " << entry.score << " points\n";
            }
        }
        waitForEnter();
    }

    void waitForEnter() {
        cout << "Press Enter to return to the main menu...\n";
        cin.ignore();
        cin.get();
    }

    void displayMenu() {
        int choice;
        do {
            clearScreen();
            cout << "'*•.¸♡ " << COLOR_GREEN << "QUIZ GAME MENU" << COLOR_RESET << " ♡¸.•*'\n"
                 << "1. Start Game\n"
                 << "2. View Leaderboard\n"
                 << "3. View Game History\n"
                 << "4. Exit\n"
                 << COLOR_CYAN << "Enter your choice (1-4): " << COLOR_RESET;

            while (!(cin >> choice) || choice < 1 || choice > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << COLOR_RED << "Invalid input. Enter a number between 1 and 4: " << COLOR_RESET;
            }

            switch (choice) {
                case 1: startGame(); break;
                case 2: viewLeaderboard(); break;
                case 3: viewGameHistory(); break;
                case 4: 
                    clearScreen();
                    cout << COLOR_YELLOW << "Exiting game...\n" << COLOR_RESET;
                    exit(0);
            }
        } while (true);
    }
};

int main() {
    QuizGame game;
    game.displayMenu();
    return 0;
}