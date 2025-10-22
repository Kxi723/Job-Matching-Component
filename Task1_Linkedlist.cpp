#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include "linkedlist.hpp"

using namespace std;
using namespace std::chrono;

// Define maximum number of keywords
const int MAX_KEYWORDS = 50;

bool fileLoaded(const string& fileName) {
    // Read file
    ifstream file(fileName);
    string header;

    if (!file.is_open()) {
        cerr << "<ERROR> File unfound: " << fileName << endl;
        return false;
    }
    
    getline(file, header);
    cout << "Loaded " << header << ".csv file." << endl;
    file.close();

    return true;
}

void updateKeywordFile(keywordLinkedList& keywords, const string& filename) {
    // Write data
    ofstream csvFile(filename);
    
    if (!csvFile.is_open()) {
        cerr << "<ERROR> Unable to open " << filename << "." << endl;
        return;
    }
    
    // Write header
    csvFile << "keyword" << endl;
    
    weightNode* current = keywords.getHead();
    while (current != nullptr) {
        // Write data into file
        csvFile << current->keywords << ", " << current->weightScore << endl;
        current = current->nextAddress;
    }
    
    csvFile.close();
}

void loadKeywordsFromFile(keywordLinkedList& keywordsList, const string& filename, int& numberOfKeywords) {
    // Read file
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "<ERROR> Keywords file not found. Using default keywords..." << endl;
        
        // Default keywords if file not exist
        keywordsList.insertAtEnd("SQL", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("Excel", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("Power BI", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("Reporting", 1, numberOfKeywords++);
        keywordsList.insertAtEnd("Data Cleaning", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("Tableau", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("Statistics", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("Pandas", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("Machine Learning", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("NLP", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("Deep Learning", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("Python", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("System Design", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("Agile", 1, numberOfKeywords++);
        keywordsList.insertAtEnd("Git", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("Docker", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("Java", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("Computer Vision", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("Keras", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("MLOps", 3, numberOfKeywords++);
        keywordsList.insertAtEnd("TensorFlow", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("REST APIs", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("Spring Boot", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("Stakeholder Management", 1, numberOfKeywords++);
        keywordsList.insertAtEnd("User Stories", 1, numberOfKeywords++);
        keywordsList.insertAtEnd("Product Roadmap", 1, numberOfKeywords++);
        keywordsList.insertAtEnd("PyTorch", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("Cloud", 2, numberOfKeywords++);
        keywordsList.insertAtEnd("Scrum", 1, numberOfKeywords++);
        
        // Save default keywords into file
        updateKeywordFile(keywordsList, filename);
        return;
    }
    
    string line;
    // Skip header
    getline(file, line);
    string header = line;

    // Load keywords 
    while (getline(file, line)) {
        stringstream ss(line);
        string keywordReaded, weightScoreReaded;

        if (getline(ss, keywordReaded, ',') && getline(ss, weightScoreReaded)) {
            // Ensure non-repeated & non-empty keyword
            if (numberOfKeywords < MAX_KEYWORDS && !keywordReaded.empty() && keywordsList.find(keywordReaded) == nullptr) {
                // To be safe, convert string to int
                int score = 0;

                stringstream score_ss(weightScoreReaded);
                score_ss >> score;

                keywordsList.insertAtEnd(keywordReaded, score, numberOfKeywords++);
            }
        }
    }
    
    cout << "Loaded " << header << ".csv file with " << keywordsList.getSize() << " keywords." << endl;
    file.close();
}

void filterResumeData(const string& resumeFile, resumeLinkedList& resumeList, keywordLinkedList& keywordList, resumeLinkedList& wrongResumeList) {
    // Read file
    ifstream fileStream(resumeFile);
    string line, originalLine;
    int fullStopIndex, inIndex, resumeID = 1;

    // Skip header
    getline(fileStream, originalLine);
    // Looping every line
    while (getline(fileStream, originalLine)) {
        line = originalLine;

        // Filter redundant data after fullstop
        fullStopIndex = line.find('.');
        line = line.substr(0, fullStopIndex);

        // Filter redundant data before and include 'in'
        inIndex = line.find("in");
        line = line.substr(inIndex + 2, (line.size() - inIndex));
        
        // Filter punctuation marks but retain commas and spaces
        for (char &c : line) {
            if (ispunct(c) && c != ',') {
                c = ' ';
            }
        }

        stringstream ss(line);
        string skill, skillsMatched;

        // Temporary array to store found keyword IDs
        int tempArr[MAX_KEYWORDS];
        int idCount = 0;
        
        // Split skills by commas in first filtered line
        while (getline(ss, skill, ',')) {
            // Trim leading & trailing spaces
            size_t start = skill.find_first_not_of(" ");
            size_t end = skill.find_last_not_of(" ");
            
            // If they arent end of the line
            if (start != string::npos && end != string::npos) {
                skill = skill.substr(start, end - start + 1); // +1 include itself index
                
                weightNode* foundKeyword = keywordList.find(skill);
                
                // Save matched keyword ID & skills
                if (foundKeyword && idCount < MAX_KEYWORDS) {
                    // If not first skill found, add comma
                    if (!skillsMatched.empty()) {
                        skillsMatched += ", ";
                    }

                    skillsMatched += skill;
                    tempArr[idCount++] = foundKeyword->index;
                }
            }
        }
        
        // Save filtered resume into linkedlist
        if (!skillsMatched.empty()) {
            // Create a dynamic array of the exact size
            int* idArr = new int[idCount];

            for(int i = 0; i < idCount; ++i) {
                idArr[i] = tempArr[i];
            }
            resumeList.insertAtEnd(resumeID, skillsMatched, idArr, idCount);
        }
        else { // Save excluded resume into another linkedlist
            wrongResumeList.insertAtEnd(resumeID, originalLine, nullptr, 0);
        }
        // Keep generate new ID
        resumeID++;
    }
    
    fileStream.close();
}

void filterJobData(const string& jobFile, jobLinkedList& jobList, keywordLinkedList& keywordList) {
    // Read file
    ifstream fileStream(jobFile);
    string line, position, requirements;
    int fullStopIndex, positionIndex, inIndex, jobID = 1;

    // Skip header
    getline(fileStream, line);
    
    while (getline(fileStream, line)) {

        // Filter redundant data after fullstop
        fullStopIndex = line.find('.');
        line = line.substr(0, fullStopIndex);

        // Filter job position
        positionIndex = line.find("needed");
        position = line.substr(1, positionIndex - 2); // Didnt change 'line' value, separete to 'position'

        // Filter redundant data before and include 'in'
        inIndex = line.find("experience");
        requirements = line.substr(inIndex + 14, (line.size() - inIndex));
        
        // Filter punctuation marks but retain commas and spaces
        for (char &c : requirements) {
            if (ispunct(c) && c != ',') {
                c = ' ';
            }
        }

        for (char &c : position) {
            if (ispunct(c) && c != ',') {
                c = ' ';
            }
        }

        stringstream ss(requirements);
        string skill, validRequirements;

        // Temporary array to store found keyword IDs
        int tempArr[MAX_KEYWORDS];
        int idCount = 0, totalScore = 0;
        
        // Split skills by commas in first filtered line
        while (getline(ss, skill, ',')) {
            // Trim leading & trailing spaces
            size_t start = skill.find_first_not_of(" ");
            size_t end = skill.find_last_not_of(" ");
            
            // If they arent end of the line
            if (start != string::npos && end != string::npos) {
                skill = skill.substr(start, end - start + 1); // +1 include itself index
                
                weightNode* foundKeyword = keywordList.find(skill);

                // Save matched keyword ID & keyword & total score  
                if (foundKeyword && idCount < MAX_KEYWORDS) {
                    // If not first requirement found, add comma
                    if (!validRequirements.empty()) {
                        validRequirements += ", ";
                    }
                    validRequirements += skill;

                    tempArr[idCount++] = foundKeyword->index;
                    totalScore += foundKeyword->weightScore;
                }
            }
        }

        if(!validRequirements.empty()) {
            // Create a dynamic array of the exact size
            int* idArr = new int[idCount];

            for(int i = 0; i < idCount; ++i) {
                idArr[i] = tempArr[i];
            }

            jobList.insertAtEnd(jobID, position, validRequirements, idArr, idCount, totalScore);
        }
        
        jobID++;
    }
    
    fileStream.close();
}

int calcMatchScore(const int* resumeSkillID, int resumeSkillCount, const int* jobRequirementID, int jobRequirementCount, const int keywordScores[]) {
    int score = 0;

    // Loop every skill
    for (int i = 0; i < resumeSkillCount; ++i) {

        // Loop every job requirement
        for (int j = 0; j < jobRequirementCount; ++j) {

            // If found, calculate matched skills score, and move next
            if (resumeSkillID[i] == jobRequirementID[j]) {
                score += keywordScores[resumeSkillID[i]];
                break;
            }
        }
    }
    return score;
}

bool booleanMatching(const string& requirements, const string& requiredSkills) {
    stringstream ss(requiredSkills);
    string skill;
    
    while (getline(ss, skill, ',')) {
        size_t start = skill.find_first_not_of(" ");
        size_t end = skill.find_last_not_of(" ");
        
        if (start != string::npos && end != string::npos) {
            skill = skill.substr(start, end - start + 1);
            
            // If any skill is missing, find return npos
            if (!skill.empty() && requirements.find(skill) == string::npos) {
                return false;
            }
        }
    }
    return true;
}

void searchingJobs(jobLinkedList& jobsFiltered) {
    string positionWanted, skills;
    
    cout << endl << " - - - - - - - - Boolean Job Search - - - - - - - - -" << endl;
    cout << "Please provide the position you apply for: ";
    cin.ignore();
    getline(cin, positionWanted);

    cout << "Please provide required skills you look for: ";
    getline(cin, skills);
    
    int resultCount = 0;

    jobNode* jobPtr = jobsFiltered.getHead();
    while (jobPtr && resultCount < 10) {
        // Check if position keywords are in job description
        bool positionMatch = false;

        stringstream ss(positionWanted);
        string position;

        while (getline(ss, position, ',')) {
            size_t start = position.find_first_not_of(" ");
            size_t end = position.find_last_not_of(" ");

            if (start != string::npos && end != string::npos) {
                position = position.substr(start, end - start + 1);

                if (jobPtr->position.find(position) != string::npos) {
                    positionMatch = true;
                    break;
                }
            }
        }
        
        // Checking all required skills are there
        bool skillsMatch = booleanMatching(jobPtr->requirements, skills);
        
        if (positionMatch && skillsMatch) {
            resultCount++;

            cout << endl << "[" << resultCount << "] Job ID: " << jobPtr->jobID << endl;
            cout << "    Skills: " << jobPtr->requirements << endl;
        }
        
        jobPtr = jobPtr->nextAddress;
    }
    
    if (resultCount == 0) {
        cout << "No jobs found matching the criteria." << endl;
    }
}

void searchingResume(resumeLinkedList& resumesFiltered) {
    string skills;
    
    cout << endl << " - - - - - - - Boolean Resume Search - - - - - - - - -" << endl;
    cout << "Please provide required skills: ";
    cin.ignore();
    getline(cin, skills);
    
    cout << endl << " - - - - - - - - Results - - - - - - - -\n";
    
    resumeNode* resumePtr = resumesFiltered.getHead();
    int count = 0;
    int resultCount = 0;
    
    while (resumePtr && resultCount < 10) {
        bool skillsMatch = booleanMatching(resumePtr->skills, skills);
        
        if (skillsMatch) {
            resultCount++;

            cout << endl << "[" << resultCount << "] Resume ID: " << resumePtr->resumeID << endl;
            cout << "    Skills: " << resumePtr->skills << endl;
        }
        
        count++;
        resumePtr = resumePtr->nextAddress;
    }
    
    if (resultCount == 0) {
        cout << "No resumes found matching the criteria." << endl;
    }
}

int displayMenu() {
    int choice = 0;
    cout << endl << "= = = = = = = = = = = = = MENU = = = = = = = = = = = = =" << endl;
    cout << "1. Top 10 best-matched jobs" << endl; // Rule-based + weight scoring
    cout << "2. Top 10 worst-matched jobs" << endl; // Rule-based + weight scoring
    cout << "3. Top 10 suitable jobs for your resume" << endl; // Rule-based + weight scoring
    cout << "4. Top 10 suitable resumes for your job" << endl; // Rule-based + weight scoring
    cout << "5. Search for jobs" << endl;
    cout << "6. Search for resumes" << endl;
    cout << "7. Show all keywords" << endl;
    cout << "8. Show invalid resumes" << endl;
    cout << "9. Show memory usage" << endl;
    cout << "10. Exit" << endl;
    cout << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    return choice;
}

void topTenMatchedJobs(resumeLinkedList& resumesFiltered, jobLinkedList& jobsFiltered, resumeLinkedList& wrongResumeList, const int keywordScores[], bool showBest = true) {
   
    // Create linkedList to store results
    MatchResultLinkedList matchResults;;

    int totalResumes = resumesFiltered.getSize();
    int totalWrongResumes = wrongResumeList.getSize();
    
    // Loop every jobs
    jobNode* jobPtr = jobsFiltered.getHead();
    while (jobPtr != nullptr) {
        // Get full marks of job requirements
        float jobMaxScore = jobPtr->totalWeightScore; 
        float totalScore = 0;

        // Loop every resumes
        resumeNode* resumePtr = resumesFiltered.getHead();
        while (resumePtr) {
            // Calc resume get how many marks
            float skillScore = calcMatchScore(resumePtr->keywordID, resumePtr->keywordCount, jobPtr->keywordID, jobPtr->keywordCount, keywordScores);
            // If resume fulfill all, get 1 mark
            totalScore += (skillScore/jobMaxScore);
            
            resumePtr = resumePtr->nextAddress;
        }

        // Store result
        matchResults.insertAtEnd(jobPtr->jobID, totalScore, jobPtr->requirements);
        jobPtr = jobPtr->nextAddress;
    }
    
    matchResults.sort(showBest);
    
    cout << endl << " - - - - Top 10 " << (showBest ? "Best" : "Worst") << " Matched Jobs - - - - -" << endl;
    
    // Display top 10 results
    MatchNode* matchPtr = matchResults.getHead();
    int count = 1;
    
    while (matchPtr != nullptr && count < 11) {
        float matchRate = (matchPtr->weightScoreObtained * 100.0 / (totalResumes+totalWrongResumes));

        cout << "[" << count << "] Job ID: " << matchPtr->jobID << endl;
        cout << "    Score: " << matchPtr->weightScoreObtained << " / " << totalResumes << "+" << totalWrongResumes << " (" << fixed << setprecision(2) << matchRate << "%)" << endl;
        cout << "    Requirements: " << matchPtr->data << endl << endl;

        matchPtr = matchPtr->nextAddress;
        count++;
    }
}

void resumeAssist(resumeLinkedList& resumesFiltered, jobLinkedList& jobsFiltered, const int keywordScores[]) {
    int resumeID;
    cout << endl << "Please provide your resume id: ";
    cin >> resumeID;
    
    // Get whole data from linkedlist if found
    resumeNode* targetResume = resumesFiltered.findByID(resumeID);
    
    if (targetResume == nullptr) {
        cout << "<ERROR> Resume ID: " << resumeID << " not found." << endl;
        return;
    }
    
    cout << "Valid skills after filtering: " << targetResume->skills << endl;
    
    // Create LinkedList to store match results
    MatchResultLinkedList matchResults;
    
    jobNode* jobPtr = jobsFiltered.getHead();
    while (jobPtr) {
        int jobMaxScore = jobPtr->totalWeightScore;
        int skillScore = calcMatchScore(targetResume->keywordID, targetResume->keywordCount, jobPtr->keywordID, jobPtr->keywordCount, keywordScores);
            
        // Only save perfect match
        if (skillScore == jobMaxScore) {
            matchResults.insertAtEnd(jobPtr->jobID, skillScore, jobPtr->requirements);
        }
        jobPtr = jobPtr->nextAddress;
    }

    int matchCount = matchResults.getSize();
    // All partial
    if (matchCount == 0) {
        cout << endl << " - - - - - - - - None Perfect Matching Job - - - - - - - - -" << endl; 
    }

    // At least 1 perfect match
    if (matchCount > 0) {
        cout << endl << " - - - - - - - - - Perfect Matching Jobs - - - - - - - - - -" << endl; 
        
        matchResults.sort(true);      
        matchResults.showJobTop10(matchCount);
    } 

    // Doesnt have 10 perfect match, show the rest
    if (matchCount < 10) {
        int partialCount = 1;

        cout << " - - - - - - - - - Showing Partial Match - - - - - - - - - -" << endl; 
        
        // Loop again 
        jobPtr = jobsFiltered.getHead();
        while (jobPtr && (matchCount + partialCount) < 11) {
            int jobMaxScore = jobPtr->totalWeightScore;
            int skillScore = calcMatchScore(targetResume->keywordID, targetResume->keywordCount, jobPtr->keywordID, jobPtr->keywordCount, keywordScores);
        
            if (skillScore != jobMaxScore && skillScore > 0) {
                cout << "[" << matchCount + partialCount << "] Job ID: " << jobPtr->jobID << " | Score: " << skillScore << " / " << jobMaxScore << endl;
                cout << "    " << jobPtr->requirements << endl << endl;

                partialCount++;
            }
            jobPtr = jobPtr->nextAddress;
        }
    }
}

void jobAssist(resumeLinkedList& resumesFiltered, jobLinkedList& jobsFiltered, const int keywordScores[]) {
    int jobID;
    cout << endl << "Please provide your job id: ";
    cin >> jobID;
    
    // Get whole data from linkedlist if found
    jobNode* targetJob = jobsFiltered.findByID(jobID);
    
    if (targetJob == nullptr) {
        cout << "<ERROR> Job ID: " << jobID << " not found." << endl;
        return;
    }
    
    cout << "Requirements: " << targetJob->requirements << endl;
    
    // Create LinkedList to store match results
    MatchResultLinkedList matchResults;

    // Get job max score first
    int jobMaxScore = targetJob->totalWeightScore;
    
    if (jobMaxScore == 0) {
        cout << "This job has no valid requirements." << endl;
        return;
    }
    
    resumeNode* resumePtr = resumesFiltered.getHead();
    while (resumePtr) {
        int skillScore = calcMatchScore(resumePtr->keywordID, resumePtr->keywordCount, targetJob->keywordID, targetJob->keywordCount, keywordScores);
        
        // Only save perfect match
        if (skillScore == jobMaxScore) {
            matchResults.insertAtEnd(resumePtr->resumeID, skillScore, resumePtr->skills);
        }
        resumePtr = resumePtr->nextAddress;
    }
    
    int matchCount = matchResults.getSize();
    // All partial
    if (matchCount == 0) {
        cout << endl << " - - - - - - - - None Perfect Matching Job - - - - - - - - -" << endl; 
    }

    // At least 1 perfect match
    if (matchCount > 0) {
        cout << endl << " - - - - - - - - - Perfect Matching Jobs - - - - - - - - - -" << endl; 
        
        matchResults.sort(true);      
        matchResults.showResumeTop10(matchCount);
    } 

    // Doesnt have 10 perfect match, show the rest
    if (matchCount < 10) {
        int partialCount = 1;

        cout << " - - - - - - - - - Showing Partial Match - - - - - - - - - -" << endl; 
        
        //Loop again 
        resumePtr = resumesFiltered.getHead();
        while (resumePtr && (matchCount + partialCount) < 11) { 
            int skillScore = calcMatchScore(resumePtr->keywordID, resumePtr->keywordCount, targetJob->keywordID, targetJob->keywordCount, keywordScores);
        
            if (skillScore != jobMaxScore && skillScore > 0) {
                cout << "[" << partialCount << "] Resume ID: " << resumePtr->resumeID << " | Score: " << skillScore << " / " << jobMaxScore << endl;
                cout << "    " << resumePtr->skills << endl << endl;

                partialCount++;
            }
            resumePtr = resumePtr->nextAddress;
        }
    }
}
                
int main() {
    resumeLinkedList resumesFiltered, wrongResumeList;
    keywordLinkedList keywordsList;
    jobLinkedList jobsFiltered;
    string resumeFile = "resume.csv";
    string jobFile = "job_description.csv";
    string keywordFile = "keywords.csv";

    // No file cannot run, but keywords we have backup so nevermind
    if (!fileLoaded(resumeFile) || !fileLoaded(jobFile)) {
        return 0;
    }

    auto startLoad = high_resolution_clock::now();

    // Create an array to hold keyword scores for fast lookup by index.
    int keywordScores[MAX_KEYWORDS] = {0};
    int numberOfKeywords = 0;

    loadKeywordsFromFile(keywordsList, keywordFile, numberOfKeywords);

    weightNode* keywordPtr = keywordsList.getHead();

    // Load keyword linkedlist, get weightScore save into array
    while(keywordPtr) {
        if(keywordPtr->index < MAX_KEYWORDS) {
            keywordScores[keywordPtr->index] = keywordPtr->weightScore;
        }
        keywordPtr = keywordPtr->nextAddress;
    }

    filterResumeData(resumeFile, resumesFiltered, keywordsList, wrongResumeList);
    filterJobData(jobFile, jobsFiltered, keywordsList);
    cout << "After filtering, total " << resumesFiltered.getSize() << " valid resumes and " << jobsFiltered.getSize() << " valid jobs." << endl;

    auto endLoad = high_resolution_clock::now();
    auto durationLoad = duration_cast<milliseconds>(endLoad - startLoad).count();

    cout << "Data loading and pre-processing completed in " << durationLoad << " ms" << endl;

    int choice = 0;
    do {
        choice = displayMenu();
        auto startOperation = high_resolution_clock::now();

        switch(choice) {
            case 1: {
                topTenMatchedJobs(resumesFiltered, jobsFiltered, wrongResumeList, keywordScores, true);
                break;
            }
            case 2:
                topTenMatchedJobs(resumesFiltered, jobsFiltered, wrongResumeList, keywordScores, false);
                break;
                
            case 3:
                resumeAssist(resumesFiltered, jobsFiltered, keywordScores);
                break;
                
            case 4:
                jobAssist(resumesFiltered, jobsFiltered, keywordScores);
                break;
                
            case 5:
                searchingJobs(jobsFiltered);
                break;
                
            case 6:
                searchingResume(resumesFiltered);
                break;
                
            case 7:
                cout << endl <<  "- - - - - - Valid Keywords - - - - - - - -\n";
                keywordsList.show();
                break;

            case 8:
                cout << endl << " - - - - - - - - - - - Invalid Resumes - - - - - - - - - - - - - -" << endl;
                wrongResumeList.show();
                break;

            case 9: {
                cout << endl << "- - - - - - - Memory Usage - - - - - - - -" << endl;

                cout << "Job_Description List: " << fixed << setprecision(2) << jobsFiltered.getTotalMemoryUsage() / 1024.0 << " KB" << endl;

                cout << "Valid Resume List: " << resumesFiltered.getTotalMemoryUsage() / 1024.0 << " KB" << endl;
                
                cout << "Invalid Resume List: " << wrongResumeList.getTotalMemoryUsage() / 1024.0 << " KB" << endl;
                
                cout << "Keyword List: " << keywordsList.getTotalMemoryUsage() / 1024.0 << " KB" << endl;
                
                break;
            }
            case 10:
                cout << "Exiting program." << endl;
                break;
                
            default:
                cout << endl << "Invalid choice! Please enter 1-10." << endl;
                break;
        }

        // Only print execution time for the main matching functions
        if (choice > 0 && choice < 5) {
             auto endOperation = high_resolution_clock::now();
             auto duration = duration_cast<milliseconds>(endOperation - startOperation).count();

             cout << "Operation completed in " << duration << " ms" << endl;
        }
        
    } while(choice != 10);

    return 0;
}