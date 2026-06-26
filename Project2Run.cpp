// Project2Run.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
Project 2
ABCU Advising
Creator: Johnathan Smith
CS: 300
6/18/2026
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//Stores information for a single course object
struct Course {

    string courseNumber;
    string courseTitle;

    //stores prerequisite course numbers
    vector<string> prerequisites;
};

//Create a node used by Binary Search Tree
struct Node {

    Course course;

    Node* left;
    Node* right;

    //Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    //Contructor with course data
    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

//Binary Search Tree Class
class BinarySearchTree {

private:

    //Root node of tree
    Node* root;

    void inOrder(Node* node);

public:

    //Constructor
    BinarySearchTree() {
        root = nullptr;
    }


    void Insert(Course course);

    //Add function to print courseList
    void printCourseList();

    Course Search(string courseNumber);
};

//Load Course Data from CSV
bool loadCourses(string fileName, BinarySearchTree& bst);

//Create a function to insert a course into the Search Tree
void BinarySearchTree::Insert(Course course) {
    
    //Create Node
    Node* newNode = new Node(course);

    //If tree is empty
    if (root == nullptr) {
        root = newNode;
        return;
    }

    //Place pointer at root
    Node* current = root;

    while (true) {
        //start on left
        if (course.courseNumber < current->course.courseNumber) {

            if (current->left == nullptr) {

                current->left = newNode;
                return;
            }

            current = current->left;
        }

        //Now, go right side
        else {
            if (current->right == nullptr) {
                current->right = newNode;
                return;
            }

            current = current->right;
        }
    }
}

//Search the binary search tree for a course number
Course BinarySearchTree::Search(string courseNumber) {

    //Start at root
    Node* current = root;

    while (current != nullptr) {

        //Course was found
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }

        //Search left subtree if not found
        if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }

        //Move to right subtree
        else {
            current = current->right;
        }
    }

    //If nothing is found, return empty course
    Course nullCourse;
    return nullCourse;
}

//Prints coursees in alphanumeric order
void BinarySearchTree::inOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    inOrder(node->left);

    cout << node->course.courseNumber << ", " << node -> course.courseTitle << endl;

    inOrder(node->right);
}

//Print in Sorted order
void BinarySearchTree::printCourseList() {
    cout << endl << "here is a sample schedule: " << endl << endl;

    //Traverse the BST in order
    inOrder(root);
}

//Program Begins Execution Here
int main()
{
    //Binary Search Tree Created to be used by program
    BinarySearchTree bst;

    //Create userInput and set value
    int userChoice = 0;

    //Variable to track if data was loaded
    bool dataLoaded = false;

    //Display Welcome message
    cout << "Welcome to the ABCU Advising Program, below you will find a menu, please select a valid option: " << endl;

    //Utilize a while and switch to display menu options and ask the user what they would like to do
    while (userChoice != 9) {

        cout << "Menu: " << endl << "1. Load Data Structure" << endl << "2. Print Course List" << endl << "3. Print Specific Course" << endl << "9. Exit" << endl;

        //Read users input
        cin >> userChoice;

        switch (userChoice) {

        case 1: {

            //Prevent repreated loading
            if (dataLoaded) {
                cout << "Course data has already been loaded." << endl;
                break;
            }

            //Create string variable for filename
            string fileName;

            cout << "Enter file name" << endl;
            //input file name from user ignoring spaces
            cin.ignore();

            //Call full userinput
            getline(cin, fileName);

            
            //Sets data loaded when program successfully opens
            if (loadCourses(fileName, bst)) {
                dataLoaded = true;
            }
            break;
        }

        case 2:

            //Check if data was loaded
            if (dataLoaded != true) {
                cout << "Please load data before continuing." << endl;
            }

            else {
                
                bst.printCourseList();
            }
            
            break;

        case 3: {

            //Check if data was loaded before
            if (dataLoaded != true) {
                cout << "Please load the course data first." << endl;
                break;
            }

            //Declare placeholder for user input
            string courseNumber;

            //Propmt and request user input
            cout << "Enter course number: " << endl;
            cin >> courseNumber;

            //Convert input to uppercase to prevent exceptions
            transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

            Course course = bst.Search(courseNumber);

            //If course is not found, return not found
            if (course.courseNumber.empty()) {
                cout << "Course not found." << endl;
            }

            else {
                cout << endl;

                cout << course.courseNumber << ", " << course.courseTitle << endl;

                cout << "Prerequisites: ";

                //if prerequisites are found, return, otherwise return none
                if (course.prerequisites.empty()) {
                    cout << "None";
                }

                else {
                    for (string prereq : course.prerequisites) {

                        Course prereqCourse = bst.Search(prereq);

                        cout << prereqCourse.courseNumber << ", " << prereqCourse.courseTitle << endl;
                    }
                }
                cout << endl;
            }
            break;
        }
        case 9:
            cout << "Thanks for using my program, now exiting, Good Bye!" << endl;
            break;

        //Insert Default to catch any exceptions from menu choices
        default: 
            cout << "Thats an invalid option, please enter 1, 2, 3, or 9" << endl;
            break;
        }
    }

    return 0;
}

//Open and read course file
bool loadCourses(string fileName, BinarySearchTree& bst) {

    //open file
    ifstream file(fileName);

    //Checks if file was not opened
    if (!file.is_open()) {

        //Display Error opening file message
        cout << "Error: Unable to open file." << endl;

        return false;
    }

    //If file opened, display message
    cout << "File Opened Successfully, Mission Accomplished." << endl;

    //Store one line from the file
    string line;

    //Read file lines
    while (getline(file, line)) {

        //Create string stream
        stringstream ss(line);

        //Store tokens from CSV
        vector<string> tokens;

        string token;

        //Split line by commas
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        //Ensure validation of token size
        if (tokens.size() < 2) {
            cout << "Invalid file format." << endl;

            continue;
        }

        //Create course object
        Course course;

        //Store the course number
        course.courseNumber = tokens[0];

        //Store course title
        course.courseTitle = tokens[1];


        //Store prerequisite courses
        for (int i = 2; i < tokens.size(); i++) {
            course.prerequisites.push_back(tokens[i]);
        }

        //Insert into BST
        bst.Insert(course);

        /**Print course information (Testing)
        cout << "Course: " << course.courseNumber << " ||| " << course.courseTitle << endl;

        //Print Prerequisites
        cout << "Prerequisites: ";

        for (string preRec : course.prerequisites) {
            cout << preRec << " ";
        }

        cout << endl << endl;

    } */
    }

    //Ensures file is not left open
    file.close();
    return true;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
