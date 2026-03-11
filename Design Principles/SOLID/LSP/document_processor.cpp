#include <cmath>
#include <iostream>
#include <string>

using namespace std;

class Document {
public:
  virtual ~Document() = default;
  virtual void open() const = 0;
  virtual string getData() const = 0;
};

class Editable : public Document {
public:
  virtual void save(const string &newData) = 0;
};

class EditableDocument : public Editable {
  string data;

public:
  EditableDocument(const string &data) : data(data) {}
  void open() const override {
    cout << "Editable Document Open. Data: " << preview() << endl;
  }

  void save(const string &newData) override {
    data = newData;
    cout << "Document saved." << endl;
  }

  string getData() const override { return data; }

private:
  string preview() const {
    return data.substr(0, min((size_t)20, data.length())) + "...";
  }
};

class ReadOnlyDocument : public Document {
  string data;

public:
  ReadOnlyDocument(const string &data) : data(data) {}
  void open() const override {
    cout << "Read-Only Document Open. Data: " << preview() << endl;
  }

  string getData() const override { return data; }

private:
  string preview() const {
    return data.substr(0, min((size_t)20, data.length())) + "...";
  }
};

class DocumentProcessor {
public:
  void process(const Document &doc) const {
    doc.open();
    cout << "Document processed." << endl;
  }

  void processAndSave(Editable &doc, const string &additionalInfo) {
    doc.open();
    string currentData = doc.getData();
    string newData = currentData + " | Processed: " + additionalInfo;
    doc.save(newData);
    cout << "Editable document processed and saved." << endl;
  }
};

int main() {
  EditableDocument editable("Draft Proposal for Q3.");
  ReadOnlyDocument readOnly("Top secret strategy.");
  DocumentProcessor processor;

  cout << "--- Processing Editable Document ---" << endl;
  processor.processAndSave(editable, "Reviewed by Alice");

  cout << "\n--- Processing Read-Only Document ---" << endl;
  processor.process(readOnly);
  // processor.processAndSave(readOnly, "By me");
  return 0;
}
