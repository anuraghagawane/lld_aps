#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

using namespace std;

class TemperatureSensor {
private:
    vector<double> readings;

public:
    void addReading(double value) {
        if (value >= -50 && value <= 150) {
            readings.push_back(value);
        }
    }

    double getAverage() const {
        if (readings.empty()) {
            return 0.0;
        }
        double sum = accumulate(readings.begin(), readings.end(), 0.0);
        return round(sum / readings.size() * 100.0) / 100.0;
    }

    int getReadingCount() const {
        return readings.size();
    }

    vector<double> getReadings() const {
        return readings;
    }
};

int main() {
    TemperatureSensor sensor;
    sensor.addReading(22.5);
    sensor.addReading(23.1);
    sensor.addReading(200.0);  // Should be rejected
    sensor.addReading(-10.0);

    cout << "Count: " << sensor.getReadingCount() << endl;  // 3
    cout << "Average: " << sensor.getAverage() << endl;     // 11.87
    return 0;
}
