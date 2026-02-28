#include <iostream>
#include <string>
using namespace std;

class CPU {
private:
    string model;
    int cores;
public:
    CPU(const string& model, int cores) : model(model), cores(cores) {}

    void describe() const {
        // TODO: Print CPU model and core count
        cout<<"  CPU: "<<model<<" ("<<cores<<" cores)"<<endl;
    }
};

class RAM {
private:
    int sizeGB;
public:
    RAM(int sizeGB) : sizeGB(sizeGB) {}

    void describe() const {
        // TODO: Print RAM size
        cout<<"  RAM: "<<sizeGB<<" GB"<<endl;
    }

    int getSizeGB() const { return sizeGB; }
};

class HardDrive {
private:
    int capacityGB;
public:
    HardDrive(int capacityGB) : capacityGB(capacityGB) {}

    void describe() const {
        // TODO: Print hard drive capacity
        cout<<"  Storage: "<<capacityGB<<" GB"<<endl;
    }
};

class Computer {
private:
    string name;
    CPU cpu;
    RAM ram;
    HardDrive hardDrive;
public:
    Computer(const string& name, const string& cpuModel, int cpuCores,
             int ramGB, int storageGB)
        : name(name),
          cpu(cpuModel, cpuCores),       // TODO: Initialize with actual values
          ram(ramGB),            // TODO: Initialize with actual values
          hardDrive(storageGB) {}    // TODO: Initialize with actual values

    void describeSpecs() const {
        // TODO: Print computer name and describe all components
        cout<<"Computer: "<<name<<endl;
        cpu.describe();
        ram.describe();
        hardDrive.describe();
    }

    void upgradeRAM(int newSizeGB) {
        // TODO (Challenge): Replace RAM with a higher-capacity one
        ram = RAM(newSizeGB);
    }
};

int main() {
    Computer pc("Dev Workstation", "Intel i7-13700K", 16, 32, 1000);

    pc.describeSpecs();

    // Challenge: upgrade RAM and verify
    pc.upgradeRAM(64);
    cout << "\nAfter RAM upgrade:" << endl;
    pc.describeSpecs();

    // When pc goes out of scope, all components are destroyed via RAII.
    return 0;
}
