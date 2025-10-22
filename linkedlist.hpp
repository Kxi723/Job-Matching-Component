#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <iostream>
#include <string>

using namespace std;

struct resumeNode {
    int resumeID;
    string skills;
    int* keywordID;
    int keywordCount;
    resumeNode* nextAddress;

    resumeNode(int id, const string& data, int* skillIDs, int count) : resumeID(id), skills(data), keywordID(skillIDs), keywordCount(count), nextAddress(nullptr) {}
};

class resumeLinkedList {
    private:
        resumeNode* head;
        
    public:
        // Constructor
        resumeLinkedList() : head(nullptr) {}
        
        int getSize() {
            // Skip header
            int count = 0;
            resumeNode* current = head;

            while (current != nullptr) {
                count++;
                current = current->nextAddress;
            }
            return count;
        }

        // Display in order
        void show() {
            resumeNode* current = head;

            while (current != nullptr) {
                cout << "Resume ID: " << current->resumeID << ". " << current->skills << endl;
                current = current->nextAddress;
            }
        }

        void insertAtEnd(int id, const string& data, int* skillIDs, int count) {
            resumeNode* newNode = new resumeNode(id, data, skillIDs, count);
            
            if (head == NULL) {
                head = newNode;
            }
            else {
                resumeNode* current = head;

                while (current->nextAddress != NULL){
                    current = current->nextAddress;
                }
                current->nextAddress = newNode;
            }
        }
        
        resumeNode* findByID(int targetID) {
            resumeNode* current = head;

            while (current != nullptr) {
                if (current->resumeID == targetID) {
                    return current;
                }
                current = current->nextAddress;
            }
            return nullptr;
        }
        
        resumeNode* getHead() {
            return head;
        }

        size_t getTotalMemoryUsage() const {
            size_t totalMemory = 0;
            resumeNode* current = head;

            while (current != nullptr) {
                // Node usage
                totalMemory += sizeof(resumeNode); 
                
                // String usage
                totalMemory += current->skills.capacity();

                // Memory for the dynamic array
                totalMemory += sizeof(int) * current->keywordCount;
                
                current = current->nextAddress;
            }

            // Linkedlist usage
            totalMemory += sizeof(resumeLinkedList); 

            return totalMemory;
        }

        // Clear all nodes
        void clear() {
            resumeNode* current = head;
            while (current != nullptr) {
                resumeNode* temp = current;
                current = current->nextAddress;

                // Free dynamic array
                delete[] temp->keywordID; 
                delete temp;
            }
            head = nullptr;
        }

        ~resumeLinkedList() {
            clear();
        }
};

struct weightNode {
    string keywords;
    int weightScore;
    int index;
    weightNode* nextAddress;

    weightNode(const string& keyword, int keywordScore, int index) : keywords(keyword), weightScore(keywordScore), index(index), nextAddress(nullptr) {}
};

class keywordLinkedList {
    private:
        weightNode* head;
        
    public:
        // Constructor
        keywordLinkedList() : head(nullptr) {}
        
        int getSize() {
            // Skip header
            int count = 0;
            weightNode* current = head;

            while (current != nullptr) {
                count++;
                current = current->nextAddress;
            }
            return count;
        }

        void show() {
            int number = 1;
            string category = "";
            weightNode* current = head;
            while (current != nullptr) {

                if (current->weightScore == 3) {
                    category = "Hard Skills";
                }
                else if (current->weightScore == 2) {
                    category = "Tools";
                }
                else if (current->weightScore == 1) {
                    category = "Soft Skills";
                }
                else {
                    category = "NULL";
                }
                
                cout << number << ". " << current->keywords << " <" << category << ">" << endl;
                current = current->nextAddress;
                
                number++;
            }
        }

        void insertAtEnd(const string& keyword, int keywordScore, int index) {
            weightNode* newNode = new weightNode(keyword, keywordScore, index);
            
            if (head == NULL) {
                head = newNode;
            }
            else {
                weightNode* current = head;

                while (current->nextAddress != NULL){
                    current = current->nextAddress;
                }
                current->nextAddress = newNode;
            }
        }
        
        weightNode* getHead() {
            return head;
        }
        
        // Check exist in list not
        weightNode* find(const string& keywordReaded) {
            weightNode* current = head;

            while (current != nullptr) {
                if (current->keywords == keywordReaded) {
                    return current;
                }
                current = current->nextAddress;
            }
            return nullptr;
        }

        size_t getTotalMemoryUsage() const {
            size_t totalMemory = 0;
            weightNode* current = head;

            while (current != nullptr) {
                // Node usage
                totalMemory += sizeof(weightNode); 
                
                // String usage
                totalMemory += current->keywords.capacity();
                
                current = current->nextAddress;
            }

            // Linkedlist usage
            totalMemory += sizeof(keywordLinkedList); 

            return totalMemory;
        }
        
        ~keywordLinkedList() {
            weightNode* current = head;
            while (current != nullptr) {
                weightNode* next = current->nextAddress;
                delete current;
                current = next;
            }
        }
};

struct jobNode {
    int jobID;
    string position;
    string requirements;
    int* keywordID;
    int keywordCount;
    int totalWeightScore;
    jobNode* nextAddress;

    jobNode(int id, const string& pos, const string& data, int* requirementID, int count, int score) : jobID(id), position(pos), requirements(data), keywordID(requirementID), keywordCount(count), totalWeightScore(score), nextAddress(nullptr) {}
};

class jobLinkedList {
    private:
        jobNode* head;
        
    public:
        // Constructor
        jobLinkedList() : head(nullptr) {}
        
        int getSize() {
            // Skip header
            int count = 0;
            jobNode* current = head;

            while (current != nullptr) {
                count++;
                current = current->nextAddress;
            }
            return count;
        }

        void insertAtEnd(int id, const string& position, const string& data, int* requirementID, int count, int score) {
            jobNode* newNode = new jobNode(id, position, data, requirementID, count, score);
            
            if (head == NULL) {
                head = newNode;
            }
            else {
                jobNode* current = head;

                while (current->nextAddress != NULL){
                    current = current->nextAddress;
                }
                current->nextAddress = newNode;
            }
        }

        jobNode* findByID(int targetID) {
            jobNode* current = head;

            while (current != nullptr) {
                if (current->jobID == targetID) {
                    return current;
                }
                current = current->nextAddress;
            }
            return nullptr;
        }

        jobNode* getHead() {
            return head;
        }

        size_t getTotalMemoryUsage() const {
            size_t totalMemory = 0;
            jobNode* current = head;

            while (current != nullptr) {
                // Node usage
                totalMemory += sizeof(jobNode); 
                
                // String usage
                totalMemory += current->position.capacity();
                totalMemory += current->requirements.capacity();
                // Dynamic array usage
                totalMemory += sizeof(int) * current->keywordCount; 
            
                current = current->nextAddress;
            }

            // Linkedlist usage
            totalMemory += sizeof(jobLinkedList); 

            return totalMemory;
        }
        
        ~jobLinkedList() {
            jobNode* current = head;

            while (current != nullptr) {
                jobNode* next = current->nextAddress;
                // Free dynamic array
                delete[] current->keywordID;
                delete current;
                current = next;
            }
        }
};

struct MatchNode {
    int jobID;
    float weightScoreObtained;
    string data;
    MatchNode* nextAddress;
    
    MatchNode(int id, float matched, string data) : jobID(id), weightScoreObtained(matched), data(data), nextAddress(nullptr) {}
};

class MatchResultLinkedList {
private:
    MatchNode* head;
    int size;
    
public:
    MatchResultLinkedList() : head(nullptr), size(0) {}
    
    // Get size
    int getSize() const {
        return size;
    }

    ~MatchResultLinkedList() {
        MatchNode* current = head;
        while (current != nullptr) {
            MatchNode* temp = current;
            current = current->nextAddress;
            delete temp;
        }
    }
    
    void insertAtEnd(int id, float totalScore, string data) {
        MatchNode* newNode = new MatchNode(id, totalScore, data);
        
        if (head == nullptr) {
            head = newNode;
        } 
        else {
            MatchNode* current = head;

            while (current->nextAddress != nullptr) {
                current = current->nextAddress;
            }
            current->nextAddress = newNode;
        }
        size++;
    }
    
    // Get middle node using slow and fast pointer
    MatchNode* getMiddle(MatchNode* head) {
        if (head == nullptr)
            return head;
        
        MatchNode* slow = head;
        MatchNode* fast = head->nextAddress;
        
        while (fast != nullptr && fast->nextAddress != nullptr) {
            // if slow = x, fast = 2x. So if fast reach the end, let say 100, 2x=100, x=50=middle
            slow = slow->nextAddress; 
            fast = fast->nextAddress->nextAddress;
        }
        
        return slow; //get middle point
    }
    
    // Merge two sorted LinkedLists
     MatchNode* merge(MatchNode* l1, MatchNode* l2, bool descending) {
        if (l1 == nullptr)
            return l2;
        if (l2 == nullptr)
            return l1;
        
        MatchNode* result = nullptr;
        
        if (descending) {
            // Higher score first
            if (l1->weightScoreObtained >= l2->weightScoreObtained) {
                result = l1;
                result->nextAddress = merge(l1->nextAddress, l2, descending);
            } 
            else {
                result = l2;
                result->nextAddress = merge(l1, l2->nextAddress, descending);
            }
        } 
        else {
            // Lower score first
            if (l2->weightScoreObtained >= l1->weightScoreObtained) {
                result = l1;
                result->nextAddress = merge(l1->nextAddress, l2, descending);
            } 
            else {
                result = l2;
                result->nextAddress = merge(l1, l2->nextAddress, descending);
            }
        }
        
        return result;
    }
    
    // Keep split and merge
    MatchNode* mergeSort(MatchNode* head, bool descending) {
        // If split til only 1 or end
        if (head == nullptr || head->nextAddress == nullptr) {
            return head;
        }
        
        // Split the list into two halves
        MatchNode* mid = getMiddle(head);
        MatchNode* midNext = mid->nextAddress;
        
        mid->nextAddress = nullptr;
        
        // Recursively sort both halves
        MatchNode* left = mergeSort(head, descending);
        MatchNode* right = mergeSort(midNext, descending);
        
        // Merge the sorted halves
        return merge(left, right, descending);
    }
    
    // Sort the LinkedList
    void sort(bool descending) {
        head = mergeSort(head, descending);
    
    }
    
    MatchNode* getHead() const {
        return head;
    }
    
    void showJobTop10(int n) {
        MatchNode* current = head;
        int count = 1;
        
        while (current != nullptr && count < n && count < 11) {
            // Only perfect match are stored, so just weightScore/weightScore means full marks
            cout << "[" << count << "] Job ID: " << current->jobID << " | Score: " << current->weightScoreObtained << " / " << current->weightScoreObtained << endl;
            cout << "    Requirements: " << current->data << endl << endl;

            current = current->nextAddress;
            count++;
        }
    }

    void showResumeTop10(int n) {
        MatchNode* current = head;
        int count = 1;
        
        while (current != nullptr && count < n && count < 11) {
            // Only perfect match are stored, so just weightScore/weightScore means full marks
            cout << "[" << count << "] Resume ID: " << current->jobID << " | Score: " << current->weightScoreObtained << " / " << current->weightScoreObtained << endl;
            cout << "    Skills: " << current->data << endl << endl;

            current = current->nextAddress;
            count++;
        }
    }
};

#endif