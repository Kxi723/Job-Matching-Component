#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include "linkedlist.hpp"

using namespace std;
using namespace std::chrono;

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

void loadKeywordsFromFile(keywordLinkedList& keywordsList, const string& filename) {
    // Read file
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "<ERROR> Keywords file not found. Using default keywords..." << endl;
        
        // Default keywords if file not exist
        keywordsList.insertAtEnd("SQL", 3);
        keywordsList.insertAtEnd("Excel", 2);
        keywordsList.insertAtEnd("Power BI", 2);
        keywordsList.insertAtEnd("Reporting", 1);
        keywordsList.insertAtEnd("Data Cleaning", 3);
        keywordsList.insertAtEnd("Tableau", 2);
        keywordsList.insertAtEnd("Statistics", 3);
        keywordsList.insertAtEnd("Pandas", 2);
        keywordsList.insertAtEnd("Machine Learning", 3);
        keywordsList.insertAtEnd("NLP", 3);
        keywordsList.insertAtEnd("Deep Learning", 3);
        keywordsList.insertAtEnd("Python", 3);
        keywordsList.insertAtEnd("System Design", 3);
        keywordsList.insertAtEnd("Agile", 1);
        keywordsList.insertAtEnd("Git", 2);
        keywordsList.insertAtEnd("Docker", 2);
        keywordsList.insertAtEnd("Java", 3);
        keywordsList.insertAtEnd("Computer Vision", 3);
        keywordsList.insertAtEnd("Keras", 2);
        keywordsList.insertAtEnd("MLOps", 3);
        keywordsList.insertAtEnd("TensorFlow", 2);
        keywordsList.insertAtEnd("REST APIs", 2);
        keywordsList.insertAtEnd("Spring Boot", 2);
        keywordsList.insertAtEnd("Stakeholder Management", 1);
        keywordsList.insertAtEnd("User Stories", 1);
        keywordsList.insertAtEnd("Product Roadmap", 1);
        keywordsList.insertAtEnd("PyTorch", 2);
        keywordsList.insertAtEnd("Cloud", 2);
        keywordsList.insertAtEnd("Scrum", 1);
        
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
        // Trim whitespace
        size_t start = line.find_first_not_of("");
        size_t comma = line.find_last_of(",");
        
        if (start != string::npos && comma != string::npos) {
            // Cut keyword
            string keywordReaded = line.substr(start, comma - start);
            // Cut score
            string weightScoreReaded = line.substr(comma + 2, 1);

            stringstream ss(weightScoreReaded);
            int intWeightScore; 
            ss >> intWeightScore;

            // Ensure non-repeated & non-empty keyword
            if (!keywordReaded.empty() && !keywordsList.found(keywordReaded)) {
                keywordsList.insertAtEnd(keywordReaded, intWeightScore);
            }
        }
    }
    
    cout << "Loaded " << header << ".csv file with " << keywordsList.getSize() << " keywords." << endl;
    file.close();
}

void addNewKeyword(keywordLinkedList& keywordsList, const string& filename) {
    string newKeyword;
    int category = 0;

    cout << endl << "Please enter the keyword you wish to add: ";
    cin.ignore();
    getline(cin, newKeyword);
    
    size_t start = newKeyword.find_first_not_of(" \n");
    size_t end = newKeyword.find_last_not_of(" \n");
    
    if (start != string::npos && end != string::npos) {
        newKeyword = newKeyword.substr(start, end - start + 1);
    }
    
    if (newKeyword.empty()) {
        cout << "<WARNING> Invalid keyword!" << endl;
        return;
    }
    
    if (keywordsList.found(newKeyword)) {
        cout << "<WARNING> Keyword already exists!" << endl;
        return;
    }

    // Pick weight score for it
    cout << "Choose it's category." << endl 
    << "<1> Hard Skills" << endl 
    << "<2> Soft Skills" << endl 
    << "<3> Tools" << endl 
    << "Please enter number: ";
    cin >> category;

    switch(category) {
        case 1:
            keywordsList.insertAtEnd(newKeyword, 3);
            break;
        case 2:
            keywordsList.insertAtEnd(newKeyword, 2);
            break;
        case 3:
            keywordsList.insertAtEnd(newKeyword, 1);
            break;
        default:
            cout << "Invalid number";
            return;
    }

    updateKeywordFile(keywordsList, filename);
    cout << "Keyword \"" << newKeyword << "\" added successfully!" << endl;
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

        stringstream ss(line), buffer;
        string skill, skillsMatched;
        
        // Split skills by commas in first filtered line
        while (getline(ss, skill, ',')) {
            // Trim leading & trailing spaces
            size_t start = skill.find_first_not_of(" ");
            size_t end = skill.find_last_not_of(" ");
            
            // If they arent end of the line
            if (start != string::npos && end != string::npos) {
                skill = skill.substr(start, end - start + 1); // +1 include itself index
                
                // Keep matched keywords
                if (keywordList.found(skill)) {
                    // If has skill added, then add comma
                    if (!skillsMatched.empty()) {
                        skillsMatched += ", ";
                    }
                    skillsMatched += skill;
                }
            }
        }
        
        // Save filtered resume into linkedlist
        if (!skillsMatched.empty()) {
            resumeList.insertAtEnd(resumeID, skillsMatched);
        }
        else { // Save excluded resume into another linkedlist
            wrongResumeList.insertAtEnd(resumeID, originalLine);
        }
        // Keep generate new ID
        resumeID++;
    }
    
    fileStream.close();
}

void filterJobData(const string& jobFile, jobLinkedList& jobList) {
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
        for (char &c : line) {
            if (ispunct(c) && c != ',') {
                c = ' ';
            }
        }

        for (char &c : position) {
            if (ispunct(c) && c != ',') {
                c = ' ';
            }
        }

        jobList.insertAtEnd(jobID, position, requirements);
        jobID++;
    }
    
    fileStream.close();
}

int calcWeightScore(const string& jobRequirement, keywordLinkedList& keywordList) {
    stringstream ss(jobRequirement);
    string skill, keyword;
    int totalWeightScore = 0;

    while (getline(ss, skill, ',')) {
        size_t start = skill.find_first_not_of(" ");
        size_t end = skill.find_last_not_of(" ");
        
        if (start != string::npos && end != string::npos) {
            skill = skill.substr(start, end - start + 1);

            // Reset for every skill
            weightNode* keywordsPtr = keywordList.getHead();
            int weightScore = 0;
            
            // Looping keyword list to find weight score
            while (keywordsPtr) {
                keyword = keywordsPtr->keywords;
                weightScore = keywordsPtr->weightScore;

                // Sum while found
                if (skill == keyword) {
                    totalWeightScore += weightScore;
                }

                keywordsPtr = keywordsPtr->nextAddress;
            }
        }
    }
    return totalWeightScore;
}

int skillMatching(const string& resumeSkill, const string& jobRequirement, keywordLinkedList& keywordList) {
    stringstream ss(resumeSkill);
    string skill, keyword;
    int matchScore = 0;

    while (getline(ss, skill, ',')) {
        size_t start = skill.find_first_not_of(" ");
        size_t end = skill.find_last_not_of(" ");
        
        if (start != string::npos && end != string::npos) {
            skill = skill.substr(start, end - start + 1);
            
            // Resume skills met job requirement, then find weight score in keywordlist
            if (!skill.empty() && jobRequirement.find(skill) != string::npos) {
                weightNode* keywordsPtr = keywordList.getHead();
                int weightScore = 0;
                
                while (keywordsPtr) {
                    keyword = keywordsPtr->keywords;
                    weightScore = keywordsPtr->weightScore;

                    if (skill == keyword) {
                        matchScore += weightScore;
                    }

                    keywordsPtr = keywordsPtr->nextAddress;
                }
            }
        }
    }
    return matchScore;
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
    cout << "8. Add new keyword" << endl;
    cout << "9. Show invalid resumes" << endl;
    cout << "10. Show memory usage" << endl;
    cout << "11. Exit" << endl;
    cout << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    return choice;
}

void topTenMatchedJobs(resumeLinkedList& resumesFiltered, jobLinkedList& jobsFiltered, keywordLinkedList& keywordList, resumeLinkedList& wrongResumeList, bool showBest = true) {
    
    // Create linkedList to store results
    MatchResultLinkedList matchResults;;

    int totalResumes = resumesFiltered.getSize();
    int totalWrongResumes = wrongResumeList.getSize();
    
    // Loop every jobs
    jobNode* jobPtr = jobsFiltered.getHead();
    while (jobPtr != nullptr) {
        // Get full marks of job requirements
        int jobScore = calcWeightScore(jobPtr->requirements, keywordList); 
        float totalScore = 0;

        // Loop every resumes
        resumeNode* resumePtr = resumesFiltered.getHead();
        while (resumePtr) {
            // Calc resume get how many marks
            float skillScore = skillMatching(resumePtr->skills, jobPtr->requirements, keywordList);
            // If resume fulfill all, get 1 mark
            totalScore += (skillScore/jobScore);
            
            resumePtr = resumePtr->nextAddress;
        }

        // Store result
        matchResults.insertAtEnd(jobPtr->jobID, totalScore, jobPtr->requirements);
        jobPtr = jobPtr->nextAddress;
    }
    
    matchResults.sort(showBest);
    
    if (showBest) {
        cout << endl << " - - - - Top 10 Best Matched Jobs - - - - -" << endl;
    } 
    else {
        cout << endl << " - - - - Top 10 Worst Matched Jobs - - - - -" << endl;
    }
    
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

void resumeAssist(resumeLinkedList& resumesFiltered, jobLinkedList& jobsFiltered, keywordLinkedList& keywordList) {
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
        int jobMaxScore = calcWeightScore(jobPtr->requirements, keywordList);
        int skillScore = skillMatching(targetResume->skills, jobPtr->requirements, keywordList);
        
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
            int jobMaxScore = calcWeightScore(jobPtr->requirements, keywordList); 
            int skillScore = skillMatching(targetResume->skills, jobPtr->requirements, keywordList);

            if (skillScore != jobMaxScore && skillScore > 0) {
                cout << "[" << matchCount + partialCount << "] Job ID: " << jobPtr->jobID << " | Score: " << skillScore << " / " << jobMaxScore << endl;
                cout << "    " << jobPtr->requirements << endl << endl;

                partialCount++;
            }
            jobPtr = jobPtr->nextAddress;
        }
    }
}

void jobAssist(resumeLinkedList& resumesFiltered, jobLinkedList& jobsFiltered, keywordLinkedList& keywordList) {
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
    int jobMaxScore = calcWeightScore(targetJob->requirements, keywordList);
    
    resumeNode* resumePtr = resumesFiltered.getHead();
    while (resumePtr) {
        int skillScore = skillMatching(resumePtr->skills, targetJob->requirements, keywordList);
        
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
        
        // Loop again 
        resumePtr = resumesFiltered.getHead();
        while (resumePtr && (matchCount + partialCount) < 11) { 
            int skillScore = skillMatching(resumePtr->skills, targetJob->requirements, keywordList);

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

    loadKeywordsFromFile(keywordsList, keywordFile);
    filterResumeData(resumeFile, resumesFiltered, keywordsList, wrongResumeList);
    filterJobData(jobFile, jobsFiltered);
    cout << "After filtering, total " << resumesFiltered.getSize() << " resumes and " << jobsFiltered.getSize() << " jobs successfully passed." << endl;

    auto endLoad = high_resolution_clock::now();
    auto durationLoad = duration_cast<milliseconds>(endLoad - startLoad).count();

    cout << "Data loading completed in " << durationLoad << " ms" << endl;

    int choice = 0;
    do {
        choice = displayMenu();

        switch(choice) {
            case 1: {
                auto startMatch = high_resolution_clock::now();
                topTenMatchedJobs(resumesFiltered, jobsFiltered, keywordsList, wrongResumeList, true);
                
                auto endMatch = high_resolution_clock::now();
                auto durationMatch = duration_cast<milliseconds>(endMatch - startMatch).count();
                cout << "Matching completed in " << durationMatch << " ms" << endl;

                break;
            }
            case 2:
                topTenMatchedJobs(resumesFiltered, jobsFiltered, keywordsList, wrongResumeList, false);
                break;
                
            case 3:
                resumeAssist(resumesFiltered, jobsFiltered, keywordsList);
                break;
                
            case 4:
                jobAssist(resumesFiltered, jobsFiltered, keywordsList);
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
                addNewKeyword(keywordsList, keywordFile);
                resumesFiltered.clear();
                wrongResumeList.clear();
                filterResumeData(resumeFile, resumesFiltered, keywordsList, wrongResumeList);
                break;

            case 9:
                cout << endl << " - - - - - - - - - - - Invalid Resumes - - - - - - - - - - - - - -" << endl;
                wrongResumeList.show();
                break;

            case 10: {
                size_t jobMemoryBytes = jobsFiltered.getTotalMemoryUsage();
                double jobMemoryKB = static_cast<double>(jobMemoryBytes) / 1024.0;
                cout << endl << "Job_Description Linked List usage: " << jobMemoryKB << " KB" << std::endl;

                size_t resumeMemoryBytes = resumesFiltered.getTotalMemoryUsage();
                double resumeMemoryKB = static_cast<double>(resumeMemoryBytes) / 1024.0;
                cout << "Resume Linked List usage: " << resumeMemoryKB << " KB" << std::endl;

                size_t wresumeMemoryBytes = wrongResumeList.getTotalMemoryUsage();
                double wresumeMemoryKB = static_cast<double>(wresumeMemoryBytes) / 1024.0;
                cout << "WrongResume Linked List usage: " << wresumeMemoryKB << " KB" << std::endl;

                size_t keywordMemoryBytes = keywordsList.getTotalMemoryUsage();
                double keywordMemoryKB = static_cast<double>(keywordMemoryBytes) / 1024.0;
                cout << "Keyword Linked List usage: " << keywordMemoryKB << " KB" << std::endl;

                break;
            }
            case 11:
                cout << "Exiting program." << endl;
                break;
                
            default:
                cout << endl << "Invalid choice! Please enter 1-11." << endl;
                break;
        }
        
    } while(choice != 11);

    return 0;
}