/** @file bankerAlgo.cpp
    @brief Implements the Banker's Algorithm for deadlock avoidance.
    @details This program reads resource allocation and maximum demand data from an input file and determines all safe sequences.
    @author Your Name
    @date January 29, 2025
*/

#include <iostream>
#include <fstream>


/** Number of resources and processes. */
int resource_count, process_count;

/**
 * @brief Loads input data from a file.
 * @param total_resources Vector to store total available resources.
 * @param max_needs Matrix to store maximum resource demand for each process.
 * @param allocated Matrix to store currently allocated resources per process.
 * @pre Input file "input.txt" must be properly formatted.
 * @post Populates total_resources, max_needs, and allocated matrices.
 */
void loadInputData(std::vector<int> &total_resources, std::vector<std::vector<int>> &max_needs, std::vector<std::vector<int>> &allocated) {
    std::ifstream file("input.txt");
    if (!file) {
        std::cerr << "Error opening file.\n";
        exit(1);
    }

    file >> resource_count;
    file >> process_count;

    total_resources.resize(resource_count);
    for (int i = 0; i < resource_count; i++) {
        file >> total_resources[i];
    }

    max_needs.resize(process_count, std::vector<int>(resource_count));
    for (int i = 0; i < process_count; i++) {
        for (int j = 0; j < resource_count; j++) {
            file >> max_needs[i][j];
        }
    }

    allocated.resize(process_count, std::vector<int>(resource_count));
    for (int i = 0; i < process_count; i++) {
        for (int j = 0; j < resource_count; j++) {
            file >> allocated[i][j];
        }
    }
    file.close();
}

/**
 * @brief Computes and prints all safe sequences using backtracking.
 * @param available Vector representing available resources.
 * @param max_needs Matrix of maximum resource demand per process.
 * @param allocated Matrix of allocated resources per process.
 * @param completed Boolean vector tracking completed processes.
 * @param current_sequence Vector storing the current safe sequence.
 * @param level Current recursion depth.
 * @param sequence_count Reference to the count of safe sequences found.
 * @pre The available resources, max_needs, and allocated matrices must be initialized.
 * @post Prints all safe sequences and updates sequence_count.
 */
void computeSafeSequences(std::vector<int> &available, std::vector<std::vector<int>> &max_needs, std::vector<std::vector<int>> &allocated, std::vector<bool> &completed, std::vector<int> &current_sequence, int level, int &sequence_count) {
    if (level == process_count) {
        sequence_count++;
        for (int i = 0; i < process_count; i++) {
            std::cout << current_sequence[i] << " ";
        }
        std::cout << std::endl;
        return;
    }

    for (int i = 0; i < process_count; i++) {
        if (!completed[i]) {
            bool canAllocate = true;
            for (int j = 0; j < resource_count; j++) {
                if (max_needs[i][j] - allocated[i][j] > available[j]) {
                    canAllocate = false;
                    break;
                }
            }

            if (canAllocate) {
                for (int j = 0; j < resource_count; j++) {
                    available[j] += allocated[i][j];
                }
                completed[i] = true;
                current_sequence[level] = i;

                computeSafeSequences(available, max_needs, allocated, completed, current_sequence, level + 1, sequence_count);

                for (int j = 0; j < resource_count; j++) {
                    available[j] -= allocated[i][j];
                }
                completed[i] = false;
            }
        }
    }
}

/**
 * @brief Main function executing Banker's Algorithm.
 * @return int Returns 0 on successful execution.
 */
int main() {
    
    std::vector<int> total_resources, available;
    std::vector<std::vector<int>> max_needs, allocated;
    std::vector<bool> completed(10, false);
    std::vector<int> current_sequence(10);
    int sequence_count = 0;

    loadInputData(total_resources, max_needs, allocated);

    available.resize(resource_count);
    for (int i = 0; i < resource_count; i++) {
        available[i] = total_resources[i];
        for (int j = 0; j < process_count; j++) {
            available[i] -= allocated[j][i];

            
        }
    }

    std::cout << "Safe sequences are:\n";
    computeSafeSequences(available, max_needs, allocated, completed, current_sequence, 0, sequence_count);

    std::cout << "Total number of safe sequences: " << sequence_count << std::endl;
    return 0;
}
