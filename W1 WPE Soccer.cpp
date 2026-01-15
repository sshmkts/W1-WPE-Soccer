// Weekly Performance Evaluator - Week 1
// I added a class (TrainingLog) 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

// Constants 
const double PRO_TRAIN_HRS = 6.0;
const double INT_TRAIN_HRS = 3.0;
const double SLEEP_MIN_OK = 7.0;
const double SLEEP_GOOD_MIN = 7.5;
const double SLEEP_GOOD_MAX = 9.0;
const double W_TRAIN = 10.0;
const double W_SLEEP = 5.0;

const int INPUT_FLUSH = 10000;
const int MAX_SESSIONS = 5;
const double SHIFT_MIN = 10.0;

const int MIN_AGE = 1;
const double MIN_TRAINING_HOURS = 0.0;
const double MIN_SLEEP_HOURS = 0.01;

const int MENU_MIN_CHOICE = 0;
const int MENU_MAX_CHOICE = 3;

const double PRO_TECH_MINS = 50.0;
const double PRO_COND_MINS = 30.0;
const double INT_TECH_MINS = 45.0;
const double INT_COND_MINS = 25.0;
const double BEG_TECH_MINS = 35.0;
const double BEG_COND_MINS = 20.0;

const int REST_DAYS_HIGH = 2;
const int REST_DAYS_MODERATE = 1;
const int REST_DAYS_LOW = 0;

// Enum for player level 
enum PlayerLevel { LEVEL_AMATEUR, LEVEL_SEMI_PRO, LEVEL_PRO };

// Struct that stores all weekly data 
struct PlayerWeek
{
    string name;
    int age;
    int sessionCount;
    double totalTraining;
    double avgTraining;
    double sleepHours;
    PlayerLevel level;
    double readinessScore;
    string advice;
};

// Function prototypes 
void setConsoleColor();
void showBanner();

int getValidInt(const string& prompt, int minValue);
double getValidDouble(const string& prompt, double minValue);

void collectPlayerBasics(PlayerWeek& pw);
void fillTrainingSessions(double sessions[], int& sessionCount, int maxSessions);
void computeTrainingStats(const double sessions[], int sessionCount, PlayerWeek& pw);
void evaluateLevel(PlayerWeek& pw);

int getMenuChoice();
void printSessionsTable(const double sessions[], int sessionCount);

void handleLevelReport(const PlayerWeek& pw, const double sessions[], int sessionCount);
void handleTrainingPlanReport(const PlayerWeek& pw);
void handleRecoveryReport(const PlayerWeek& pw);

string levelToString(PlayerLevel level);

// ===============================================
// CLASS TrainingLog 
// ===============================================
// I made this class very simple. It only organizes steps.

class TrainingLog
{
private:
    PlayerWeek playerWeek;              // stores one player's weekly results
    double trainingSessions[MAX_SESSIONS]; // array for training hours

public:
    // Constructor: I set all values to safe defaults
    TrainingLog()
    {
        playerWeek.name = "";
        playerWeek.age = 0;
        playerWeek.sessionCount = 0;
        playerWeek.totalTraining = 0.0;
        playerWeek.avgTraining = 0.0;
        playerWeek.sleepHours = 0.0;
        playerWeek.readinessScore = 0.0;
        playerWeek.level = LEVEL_AMATEUR;
        playerWeek.advice = "";

        for (int i = 0; i < MAX_SESSIONS; ++i)
            trainingSessions[i] = 0.0;
    }

    // This function replaces the old main() input section.
    void setup()
    {
        collectPlayerBasics(playerWeek);
        fillTrainingSessions(trainingSessions, playerWeek.sessionCount, MAX_SESSIONS);
        computeTrainingStats(trainingSessions, playerWeek.sessionCount, playerWeek);


        playerWeek.sleepHours = getValidDouble("Avg sleep hours per night: ",
            MIN_SLEEP_HOURS);

        evaluateLevel(playerWeek);
    }

    // Menu loop (same as Week 7 menu)
    void runMenu()
    {
        int choice;
        do
        {
            choice = getMenuChoice();

            switch (choice)
            {
            case 1:
                showLevelReport();
                break;

            case 2:
                handleTrainingPlanReport(playerWeek);
                break;

            case 3:
                handleRecoveryReport(playerWeek);
                break;

            case 0:
                cout << "\nExiting...\n";
                break;
            }

        } while (choice != 0);
    }

    void showLevelReport() const
    {
        handleLevelReport(playerWeek, trainingSessions, playerWeek.sessionCount);
    }
};



void setConsoleColor()
{
    system("color 1E");  //  changing color on Windows
}

void showBanner()
{
    cout << "===================================\n";
    cout << "    Weekly Performance Evaluator\n";
    cout << "===================================\n\n";
}

// Basic int validation
int getValidInt(const string& prompt, int minValue)
{
    int value;
    cout << prompt;

    // loop until user enters a good number
    while (!(cin >> value) || value < minValue)
    {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid input. " << prompt;
    }

    cin.ignore(INPUT_FLUSH, '\n');
    return value;
}

// Basic double validation
double getValidDouble(const string& prompt, double minValue)
{
    double value;
    cout << prompt;

    while (!(cin >> value) || value < minValue)
    {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid input. " << prompt;
    }

    cin.ignore(INPUT_FLUSH, '\n');
    return value;
}

// Collect name + age
void collectPlayerBasics(PlayerWeek& pw)
{
    cout << "Enter player's full name: ";
    getline(cin, pw.name);

    pw.age = getValidInt("Enter player's age: ", MIN_AGE);

    cout << fixed << showpoint << setprecision(2);
}

// Fill training hours array
void fillTrainingSessions(double sessions[], int& sessionCount, int maxSessions)
{
    cout << "\nHow many training sessions did you have this week (1-"
        << maxSessions << ")? ";

    while (!(cin >> sessionCount) || sessionCount < 1 || sessionCount > maxSessions)
    {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid. Enter a number between 1 and "
            << maxSessions << ": ";
    }

    cin.ignore(INPUT_FLUSH, '\n');

    cout << "\nEnter training hours for each session:\n";

    for (int i = 0; i < sessionCount; ++i)
    {
        sessions[i] = getValidDouble("  Session " + to_string(i + 1) + ": ",
            MIN_TRAINING_HOURS);
    }

    // fill any unused spots with zero
    for (int i = sessionCount; i < maxSessions; ++i)
        sessions[i] = 0.0;
}

// Compute total + average training
void computeTrainingStats(const double sessions[], int sessionCount, PlayerWeek& pw)
{
    pw.totalTraining = 0.0;

    for (int i = 0; i < sessionCount; ++i)
        pw.totalTraining += sessions[i];

    pw.avgTraining = (sessionCount > 0 ? pw.totalTraining / sessionCount : 0.0);
    pw.sessionCount = sessionCount;
}

// Determine level 
void evaluateLevel(PlayerWeek& pw)
{
    pw.readinessScore = (pw.avgTraining * W_TRAIN)
        + ((pw.sleepHours - SLEEP_MIN_OK) * W_SLEEP);

    if (pw.avgTraining >= PRO_TRAIN_HRS &&
        (pw.sleepHours >= SLEEP_GOOD_MIN && pw.sleepHours <= SLEEP_GOOD_MAX))
    {
        pw.level = LEVEL_PRO;
    }
    else if (pw.avgTraining >= INT_TRAIN_HRS && pw.sleepHours >= SLEEP_MIN_OK)
    {
        pw.level = LEVEL_SEMI_PRO;
    }
    else
    {
        pw.level = LEVEL_AMATEUR;
    }

    // choosing advice text
    if (pw.sleepHours < SLEEP_MIN_OK || pw.avgTraining < INT_TRAIN_HRS)
        pw.advice = "Build base: 3 sessions + 8h sleep.";
    else if (pw.avgTraining >= PRO_TRAIN_HRS &&
        !(pw.sleepHours >= SLEEP_GOOD_MIN && pw.sleepHours <= SLEEP_GOOD_MAX))
        pw.advice = "Improve sleep routine.";
    else
        pw.advice = "Good balance.";
}

// Menu
int getMenuChoice()
{
    int choice;

    cout << "\nMenu:\n";
    cout << "  1) Evaluate Level\n";
    cout << "  2) Training Plan\n";
    cout << "  3) Recovery\n";
    cout << "  0) Quit\n";
    cout << "Enter choice: ";

    while (!(cin >> choice) || choice < MENU_MIN_CHOICE || choice > MENU_MAX_CHOICE)
    {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid. Enter " << MENU_MIN_CHOICE << "-"
            << MENU_MAX_CHOICE << ": ";
    }

    cin.ignore(INPUT_FLUSH, '\n');
    return choice;
}

// Print sessions on screen
void printSessionsTable(const double sessions[], int sessionCount)
{
    cout << "\nSession Breakdown:\n";
    cout << left << setw(10) << "#"
        << setw(12) << "Hours" << "\n";

    for (int i = 0; i < sessionCount; ++i)
    {
        cout << left << setw(10) << (i + 1)
            << setw(12) << sessions[i] << "\n";
    }
}

// Level report + file output
void handleLevelReport(const PlayerWeek& pw, const double sessions[], int sessionCount)
{
    cout << "\n----- LEVEL REPORT -----\n";
    cout << "Player: " << pw.name << "\n";
    cout << "Age: " << pw.age << "\n";
    cout << "Level: " << levelToString(pw.level) << "\n";
    cout << "Total training this week: " << pw.totalTraining << "\n";
    cout << "Average training per session: " << pw.avgTraining << "\n";
    cout << "Average sleep: " << pw.sleepHours << "\n";
    cout << "Readiness Score: " << pw.readinessScore << "\n";
    cout << "Advice: " << pw.advice << "\n";

    printSessionsTable(sessions, sessionCount);

    ofstream out("report.txt");
    if (out)
    {
        out << "WEEKLY PERFORMANCE REPORT (Week 8)\n";
        out << "Section: LEVEL\n";
        out << "----------------------------------\n";
        out << left << setw(22) << "Player:" << right << setw(20) << pw.name << "\n";
        out << left << setw(22) << "Age:" << right << setw(20) << pw.age << "\n";
        out << left << setw(22) << "Total training:" << right << setw(20) << pw.totalTraining << "\n";
        out << left << setw(22) << "Avg training:" << right << setw(20) << pw.avgTraining << "\n";
        out << left << setw(22) << "Avg sleep:" << right << setw(20) << pw.sleepHours << "\n";
        out << left << setw(22) << "Level:" << right << setw(20) << levelToString(pw.level) << "\n";
        out << left << setw(22) << "Readiness:" << right << setw(20) << pw.readinessScore << "\n";
        out << left << setw(22) << "Advice:" << right << setw(20) << pw.advice << "\n\n";

        out << "Session details:\n";
        out << left << setw(10) << "Session"
            << setw(15) << "Hours" << "\n";

        for (int i = 0; i < sessionCount; ++i)
        {
            out << left << setw(10) << (i + 1)
                << setw(15) << sessions[i] << "\n";
        }
    }
}

// Training Plan
void handleTrainingPlanReport(const PlayerWeek& pw)
{
    string planFocus;
    double techMins;
    double condMins;

    switch (pw.level)
    {
    case LEVEL_PRO:
        techMins = PRO_TECH_MINS;
        condMins = PRO_COND_MINS;
        planFocus = "High Tempo";
        break;
    case LEVEL_SEMI_PRO:
        techMins = INT_TECH_MINS;
        condMins = INT_COND_MINS;
        planFocus = "Balanced";
        break;
    default:
        techMins = BEG_TECH_MINS;
        condMins = BEG_COND_MINS;
        planFocus = "Fundamentals";
        break;
    }

    if (pw.sleepHours < SLEEP_MIN_OK && condMins >= SHIFT_MIN)
    {
        techMins += SHIFT_MIN;
        condMins -= SHIFT_MIN;
    }

    cout << "\n------- TRAINING PLAN -------\n";
    cout << left << setw(20) << "Player:" << right << setw(20) << pw.name << "\n";
    cout << left << setw(20) << "Level:" << right << setw(20) << levelToString(pw.level) << "\n";
    cout << left << setw(20) << "Focus:" << right << setw(20) << planFocus << "\n";
    cout << left << setw(20) << "Technical work:" << right << setw(20) << techMins << " min\n";
    cout << left << setw(20) << "Conditioning:" << right << setw(20) << condMins << " min\n";
}

// Recovery Output
void handleRecoveryReport(const PlayerWeek& pw)
{
    string fatigue = "Low";
    int rest = REST_DAYS_LOW;
    string tip;

    if (pw.sleepHours < SLEEP_MIN_OK)
        fatigue = "High";
    else if (pw.totalTraining >= PRO_TRAIN_HRS ||
        pw.totalTraining < INT_TRAIN_HRS)
        fatigue = "Moderate";

    if (fatigue == "High")
        rest = REST_DAYS_HIGH;
    else if (fatigue == "Moderate")
        rest = REST_DAYS_MODERATE;

    if (pw.sleepHours < SLEEP_MIN_OK)
        tip = "Aim for 8h sleep.";
    else if (pw.totalTraining >= PRO_TRAIN_HRS)
        tip = "Foam roll.";
    else
        tip = "Light stretching.";

    cout << "\n---- RECOVERY ----\n";
    cout << left << setw(18) << "Fatigue:" << right << setw(20) << fatigue << "\n";
    cout << left << setw(18) << "Rest Days:" << right << setw(20) << rest << "\n";
    cout << left << setw(18) << "Tip:" << right << setw(20) << tip << "\n";
}

string levelToString(PlayerLevel level)
{
    switch (level)
    {
    case LEVEL_PRO: return "Pro";
    case LEVEL_SEMI_PRO: return "Semi-Pro";
    case LEVEL_AMATEUR: return "Amateur";
    default: return "Unknown";
    }
}

// ====================
// MAIN
// ====================


int main()
{
    setConsoleColor();
    showBanner();

    TrainingLog tracker;  // I create my class object

    tracker.setup();
    tracker.runMenu();

    return 0;
}

