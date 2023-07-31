/*In this example, we have a struct Person representing a person with fields name and age. We also define a comparison function compareByName that compares a person's name with the target name.
The binarySearch function is templated, so it can work with any struct type for which you provide a suitable comparison function. The comparison function is passed as a function pointer to binarySearch, allowing it to be used for different types of comparisons.
The binary search algorithm remains the same as before, but this time we call the comparison function with the current element from the vector and the target value to decide which half of the search space to continue with.
Remember that for binary search to work correctly, the vector of structs (sortedPeople in this example) needs to be sorted according to the comparison function. If it's not sorted, you should sort it before applying binary search using std::sort().*/

#include <vector>
#include <string>
#include <iostream>

using namespace std;

// Define the struct representing a person
struct Person {
    string name;
    int age;

    // Comparison function for binary search
    // Returns true if the person's name is less than the target name
    static bool compareByName(const Person& p, const string& targetName) {
        return p.name < targetName;
    }
};

// Binary search function for structs
// Returns the index of the target struct if found, otherwise returns -1
template<typename T>
int binarySearch(const vector<T>& arr, const string& targetName, bool (*compareFunc)(const T&, const string&)) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (compareFunc(arr[mid], targetName)) {
            left = mid + 1; // Target may be in the right half
        } else if (compareFunc(arr[mid], targetName)) {
            right = mid - 1; // Target may be in the left half
        } else {
            return mid; // Target struct found
        }
    }

    return -1; // Target struct not found
}

int main() {
    vector<Person> sortedPeople = {
        {"Alice", 25},
        {"Bob", 30},
        {"John", 22},
        {"Kate", 27},
        {"Michael", 35}
    };

    string targetName = "John";

    int result = binarySearch(sortedPeople, targetName, &Person::compareByName);

    if (result != -1) {
        cout << "Person with name \"" << targetName << "\" found at index " << result << endl;
    } else {
        cout << "Person not found." << endl;
    }

    return 0;
}
