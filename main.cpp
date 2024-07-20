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
    static double coefficientOfVariation(const std::vector<double>& data);
    static std::vector<double> findOutliers(const std::vector<double>& data);
    static double skewness(const std::vector<double>& data);
    static double kurtosis(const std::vector<double>& data);
    static double quartile(const std::vector<double>& data, double percentile);

    // Qualitative methods
    static std::string mode(const std::vector<std::string>& data);
    static std::map<std::string, int> frequency(const std::vector<std::string>& data);
    static std::map<std::string, double> proportion(const std::vector<std::string>& data);

    // Helper methods
    static DataType detectDataType(const std::string& filename);
    static void processQuantitativeData(const std::vector<double>& data, std::ofstream& report);
    static void processQualitativeData(const std::vector<std::string>& data, std::ofstream& report);
    static void readFile(const std::string& filename, std::vector<double>& qData, std::vector<std::string>& qlData);
};

// Quantitative methods
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

double Statistics::coefficientOfVariation(const std::vector<double>& data) {
    return (standardDeviation(data) / mean(data)) * 100;
}

std::vector<double> Statistics::findOutliers(const std::vector<double>& data) {
    std::vector<double> outliers;
    double q1, q3, iqr;
    std::vector<double> sortedData = data;

    std::sort(sortedData.begin(), sortedData.end());
    size_t size = sortedData.size();
    q1 = quartile(sortedData, 25);
    q3 = quartile(sortedData, 75);
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

double Statistics::quartile(const std::vector<double>& data, double percentile) {
    size_t n = data.size();
    double k = (percentile / 100.0) * (n + 1);
    size_t index = static_cast<size_t>(std::floor(k));
    double fraction = k - index;
    if (index < 1) index = 1;
    if (index >= n) index = n - 1;
    return data[index - 1] + fraction * (data[index] - data[index - 1]);
}

double Statistics::skewness(const std::vector<double>& data) {
    double m = mean(data);
    double s = standardDeviation(data);
    double sum = 0.0;
    for (double value : data) {
        sum += std::pow((value - m) / s, 3);
    }
    return sum / data.size();
}

double Statistics::kurtosis(const std::vector<double>& data) {
    double m = mean(data);
    double s = standardDeviation(data);
    double sum = 0.0;
    for (double value : data) {
        sum += std::pow((value - m) / s, 4);
    }
    return (sum / data.size()) - 3;
}

// Qualitative methods
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

std::map<std::string, double> Statistics::proportion(const std::vector<std::string>& data) {
    std::map<std::string, int> freq = frequency(data);
    std::map<std::string, double> prop;
    int total = data.size();
    for (const auto& pair : freq) {
        prop[pair.first] = static_cast<double>(pair.second) / total;
    }
    return prop;
}

// Helper methods
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

void Statistics::processQuantitativeData(const std::vector<double>& data, std::ofstream& report) {
    report << "Quantitative Data Analysis\n";
    report << "N: " << data.size() << std::endl;
    report << "Mean: " << mean(data) << std::endl;
    report << "Median: " << median(const_cast<std::vector<double>&>(data)) << std::endl;
    report << "Mode: " << mode(data) << std::endl;
    report << "Variance: " << variance(data) << std::endl;
    report << "Standard Deviation: " << standardDeviation(data) << std::endl;
    report << "Coefficient of Variation: " << coefficientOfVariation(data) << "%" << std::endl;
    report << "Skewness: " << skewness(data) << std::endl;
    report << "Kurtosis: " << kurtosis(data) << std::endl;
    report << "Quartiles (Q1, Q3): " << quartile(data, 25) << ", " << quartile(data, 75) << std::endl;
    std::vector<double> outliers = findOutliers(data);
    report << "Outliers: ";
    for (double outlier : outliers) {
        report << outlier << " ";
    }
    report << std::endl;
}

void Statistics::processQualitativeData(const std::vector<std::string>& data, std::ofstream& report) {
    report << "Qualitative Data Analysis\n";
    report << "N: " << data.size() << std::endl;
    report << "Mode: " << mode(data) << std::endl;
    std::map<std::string, int> freq = frequency(data);
    std::map<std::string, double> prop = proportion(data);

    report << "Frequencies: " << std::endl;
    for (const auto& pair : freq) {
        report << pair.first << ": " << pair.second << std::endl;
    }

    report << "Proportions: " << std::endl;
    for (const auto& pair : prop) {
        report << pair.first << ": " << pair.second << std::endl;
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
    std::string outputFilename = "report.txt";
    std::ofstream report(outputFilename);

    if (!report.is_open()) {
        std::cerr << "Could not open the report file!" << std::endl;
        return 1;
    }

    DataType dataType = Statistics::detectDataType(filename);

    std::vector<double> quantitativeData;
    std::vector<std::string> qualitativeData;

    Statistics::readFile(filename, quantitativeData, qualitativeData);

    if (dataType == Quantitative) {
        Statistics::processQuantitativeData(quantitativeData, report);
    } else if (dataType == Qualitative) {
        Statistics::processQualitativeData(qualitativeData, report);
    } else {
        std::cerr << "Unable to determine data type." << std::endl;
        return 1;
    }

    report.close();
    std::cout << "Report generated: " << outputFilename << std::endl;

    return 0;
}
