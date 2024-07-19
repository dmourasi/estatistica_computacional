#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <numeric>
#include <string>

enum DataType {
    Quantitative,
    Qualitative,
    Unknown
};

class Statistics {
public:
    // Quantitative methods
    static double mean(const std::vector<double>& data);
    static double median(std::vector<double>& data);
    static double mode(const std::vector<double>& data);
    static double variance(const std::vector<double>& data);
    static double standardDeviation(const std::vector<double>& data);
    static std::vector<double> findOutliers(const std::vector<double>& data);

    // Qualitative methods
    static std::string mode(const std::vector<std::string>& data);
    static std::map<std::string, int> frequency(const std::vector<std::string>& data);

    // Helper methods
    static DataType detectDataType(const std::string& filename);
    static void processQuantitativeData(const std::vector<double>& data);
    static void processQualitativeData(const std::vector<std::string>& data);
    static void readFile(const std::string& filename, std::vector<double>& qData, std::vector<std::string>& qlData);
};

double Statistics::mean(const std::vector<double>& data) {
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

double Statistics::median(std::vector<double>& data) {
    std::sort(data.begin(), data.end());
    size_t size = data.size();
    if (size % 2 == 0) {
        return (data[size / 2 - 1] + data[size / 2]) / 2;
    } else {
        return data[size / 2];
    }
}

double Statistics::mode(const std::vector<double>& data) {
    std::map<double, int> frequency;
    for (double value : data) {
        frequency[value]++;
    }
    double mode = data[0];
    int maxCount = 0;
    for (const auto& pair : frequency) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            mode = pair.first;
        }
    }
    return mode;
}

double Statistics::variance(const std::vector<double>& data) {
    double m = mean(data);
    double sum = 0.0;
    for (double value : data) {
        sum += (value - m) * (value - m);
    }
    return sum / data.size();
}

double Statistics::standardDeviation(const std::vector<double>& data) {
    return std::sqrt(variance(data));
}

std::vector<double> Statistics::findOutliers(const std::vector<double>& data) {
    std::vector<double> outliers;
    double q1, q3, iqr;
    std::vector<double> sortedData = data;

    std::sort(sortedData.begin(), sortedData.end());
    size_t size = sortedData.size();
    q1 = sortedData[size / 4];
    q3 = sortedData[3 * size / 4];
    iqr = q3 - q1;

    double lowerBound = q1 - 1.5 * iqr;
    double upperBound = q3 + 1.5 * iqr;

    for (double value : sortedData) {
        if (value < lowerBound || value > upperBound) {
            outliers.push_back(value);
        }
    }
    return outliers;
}

std::string Statistics::mode(const std::vector<std::string>& data) {
    std::map<std::string, int> frequency;
    for (const std::string& value : data) {
        frequency[value]++;
    }
    std::string mode = data[0];
    int maxCount = 0;
    for (const auto& pair : frequency) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            mode = pair.first;
        }
    }
    return mode;
}

std::map<std::string, int> Statistics::frequency(const std::vector<std::string>& data) {
    std::map<std::string, int> freq;
    for (const std::string& value : data) {
        freq[value]++;
    }
    return freq;
}

DataType Statistics::detectDataType(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return Unknown;
    }

    std::string line, cell;
    std::getline(file, line);
    std::istringstream ss(line);
    while (std::getline(ss, cell, ',')) {
        try {
            std::stod(cell);
        } catch (...) {
            return Qualitative;
        }
    }
    return Quantitative;
}

void Statistics::processQuantitativeData(const std::vector<double>& data) {
    std::cout << "Mean: " << mean(data) << std::endl;
    std::cout << "Median: " << median(const_cast<std::vector<double>&>(data)) << std::endl;
    std::cout << "Mode: " << mode(data) << std::endl;
    std::cout << "Variance: " << variance(data) << std::endl;
    std::cout << "Standard Deviation: " << standardDeviation(data) << std::endl;
    std::vector<double> outliers = findOutliers(data);
    std::cout << "Outliers: ";
    for (double outlier : outliers) {
        std::cout << outlier << " ";
    }
    std::cout << std::endl;
}

void Statistics::processQualitativeData(const std::vector<std::string>& data) {
    std::cout << "Mode: " << mode(data) << std::endl;
    std::map<std::string, int> freq = frequency(data);
    std::cout << "Frequencies: " << std::endl;
    for (const auto& pair : freq) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

void Statistics::readFile(const std::string& filename, std::vector<double>& qData, std::vector<std::string>& qlData) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    std::string line, cell;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        while (std::getline(ss, cell, ',')) {
            try {
                qData.push_back(std::stod(cell));
            } catch (...) {
                qlData.push_back(cell);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    DataType dataType = Statistics::detectDataType(filename);

    std::vector<double> quantitativeData;
    std::vector<std::string> qualitativeData;

    Statistics::readFile(filename, quantitativeData, qualitativeData);

    if (dataType == Quantitative) {
        Statistics::processQuantitativeData(quantitativeData);
    } else if (dataType == Qualitative) {
        Statistics::processQualitativeData(qualitativeData);
    } else {
        std::cerr << "Unable to determine data type." << std::endl;
        return 1;
    }

    return 0;
}
