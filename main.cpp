#include <iostream>
#include <fstream>
#include <Windows.h>

const char* STR_ERROR_OPEN_FILE = "Failed to open file";
const char* STR_GAME_OVER_1 = "All cells are dead. Game over";
const char* STR_GAME_OVER_2 = "The world has stagnated. Game over";
const char* STR_GEN = "Generation: ";
const char* STR_ALIVE = "Alive cells: ";
const char CHAR_ALIVE = '*';
const char CHAR_DEAD = '-';

bool** createUniverse(int rows, int cols) {
	bool** arr = new bool* [rows];
	for (int i = 0; i < rows; i++) {
		arr[i] = new bool[cols]();
	}
	return arr;
}

void initUniverses(const char* path, bool**& arr1, bool**& arr2, int& rows, int& cols) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cout << STR_ERROR_OPEN_FILE << std::endl;
		exit(1);
	}
	file >> rows >> cols;
	arr1 = createUniverse(rows, cols);
	arr2 = createUniverse(rows, cols);

	int m, n;
	while (file >> m >> n) {
		arr1[m][n] = true;
		arr2[m][n] = true;
	}
	file.close();
}

bool getState(bool** arr, int rows, int cols, int m, int n) {
	int counter = 0;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (m + i >= 0 && m + i < rows && n + j >= 0 && n + j < cols) {
				if (arr[m + i][n + j] && !(m + i == m && n + j == n)) {
					counter++;
				}
			}
		}
	}
	if (arr[m][n] && (counter == 2 || counter == 3)) {
		return true;
	}
	else if (!arr[m][n] && counter == 3) {
		return true;
	}
	else {
		return false;
	}
}

int countAliveCells(bool** arr, int rows, int cols) {
	int counter = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (arr[i][j]) counter++;
		}
	}
	return counter;
}

void copyUniverse(bool** dist, bool** source, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			dist[i][j] = source[i][j];
		}
	}
}

void updateUniverse(bool** curArr, bool** prevArr, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			curArr[i][j] = getState(prevArr, rows, cols, i, j);
		}
	}
}

bool isUniverseAlive(bool** arr, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (arr[i][j]) return true;
		}
	}
	return false;
}

bool isUniversesEqual(bool** arr1, bool** arr2, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (arr1[i][j] != arr2[i][j]) return false;
		}
	}
	return true;
}

void displayUniverse(bool** arr, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (arr[i][j]) {
				std::cout << CHAR_ALIVE;
			}
			else {
				std::cout << CHAR_DEAD;
			}
			std::cout << ' ';
		}
		std::cout << std::endl;
	}
}

void deleteUniverse(bool** arr, int rows) {
	for (int i = 0; i < rows; i++) {
		delete[] arr[i];
	}
	delete[] arr;
}

int main() {
	int rows, cols;
	bool** oldUniverse;
	bool** newUniverse;
	initUniverses("init.txt", oldUniverse, newUniverse, rows, cols);
	int aliveCells = countAliveCells(newUniverse, rows, cols);
	int generation = 1;
	bool isStagnated = false;
	displayUniverse(newUniverse, rows, cols);
	std::cout << STR_GEN << generation << ". " << STR_ALIVE << aliveCells << std::endl;
	if (!isUniverseAlive(newUniverse, rows, cols)) {
		std::cout << STR_GAME_OVER_1 << std::endl;
		return 0;
	}
	Sleep(1000);
	while (true) {
		updateUniverse(newUniverse, oldUniverse, rows, cols);
		isStagnated = isUniversesEqual(newUniverse, oldUniverse, rows, cols);
		copyUniverse(oldUniverse, newUniverse, rows, cols);
		generation++;
		aliveCells = countAliveCells(newUniverse, rows, cols);
		system("CLS");
		displayUniverse(newUniverse, rows, cols);
		std::cout << STR_GEN << generation << ". " << STR_ALIVE << aliveCells << std::endl;
		if (!isUniverseAlive(newUniverse, rows, cols)) {
			std::cout << STR_GAME_OVER_1 << std::endl;
			break;
		}
		else if (isStagnated) {
			std::cout << STR_GAME_OVER_2 << std::endl;
			break;
		}
		else {
			Sleep(200);
		}
	}
	deleteUniverse(oldUniverse, rows);
	deleteUniverse(newUniverse, rows);
	system("pause");
}