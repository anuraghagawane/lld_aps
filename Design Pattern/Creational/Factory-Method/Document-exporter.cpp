#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Document {
public:
  virtual string getHeader() = 0;
  virtual string formatRow(const vector<string> &data) = 0;
  virtual string getFooter() = 0;
  virtual string getFileExtension() = 0;
  virtual ~Document() = default;
};

class PdfDocument : public Document {
public:
  string getHeader() override { return "--- PDF DOCUMENT START ---"; }

  string formatRow(const vector<string> &data) override {
    string result = "| ";
    for (size_t i = 0; i < data.size(); i++) {
      result += data[i];
      if (i < data.size() - 1)
        result += " | ";
    }
    result += " |";
    return result;
  }

  string getFooter() override { return "--- PDF DOCUMENT END ---"; }
  string getFileExtension() override { return ".pdf"; }
};

class HtmlDocument : public Document {
public:
  string getHeader() override { return "<html><body><table>"; }

  string formatRow(const vector<string> &data) override {
    string result = "<tr>";
    for (const auto &cell : data) {
      result += "<td>" + cell + "</td>";
    }
    result += "</tr>";
    return result;
  }

  string getFooter() override { return "</table></body></html>"; }
  string getFileExtension() override { return ".html"; }
};

class CsvDocument : public Document {
public:
  string getHeader() override { return ""; }

  string formatRow(const vector<string> &data) override {
    string result;
    for (size_t i = 0; i < data.size(); i++) {
      result += data[i];
      if (i < data.size() - 1)
        result += ",";
    }
    return result;
  }

  string getFooter() override { return ""; }
  string getFileExtension() override { return ".csv"; }
};

class ExportCreator {
public:
  virtual unique_ptr<Document> createDocument() = 0;

  void exportData(const vector<vector<string>> &data) {
    auto doc = createDocument();

    cout << "Exporting to " << doc->getFileExtension() << " format..." << endl;

    string header = doc->getHeader();
    if (!header.empty())
      cout << header << endl;

    for (const auto &row : data) {
      cout << doc->formatRow(row) << endl;
    }

    string footer = doc->getFooter();
    if (!footer.empty())
      cout << footer << endl;

    cout << "Export complete." << endl << endl;
  }

  virtual ~ExportCreator() = default;
};

class PdfExportCreator : public ExportCreator {
public:
  unique_ptr<Document> createDocument() override {
    return make_unique<PdfDocument>();
  }
};

class HtmlExportCreator : public ExportCreator {
public:
  unique_ptr<Document> createDocument() override {
    return make_unique<HtmlDocument>();
  }
};

class CsvExportCreator : public ExportCreator {
public:
  unique_ptr<Document> createDocument() override {
    return make_unique<CsvDocument>();
  }
};

int main() {
  vector<vector<string>> reportData = {{"Name", "Department", "Salary"},
                                       {"Alice", "Engineering", "120000"},
                                       {"Bob", "Marketing", "95000"},
                                       {"Charlie", "Design", "105000"}};

  PdfExportCreator pdfExporter;
  pdfExporter.exportData(reportData);

  HtmlExportCreator htmlExporter;
  htmlExporter.exportData(reportData);

  CsvExportCreator csvExporter;
  csvExporter.exportData(reportData);

  return 0;
}
