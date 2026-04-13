#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class DocumentVisitor;

class DocumentElement {
public:
  virtual void accept(DocumentVisitor *visitor) = 0;
  virtual ~DocumentElement() {}
};

class Paragraph;
class Heading;
class Image;

class DocumentVisitor {
public:
  virtual void visitParagraph(Paragraph *p) = 0;
  virtual void visitHeading(Heading *h) = 0;
  virtual void visitImage(Image *img) = 0;
  virtual ~DocumentVisitor() {}
};

// Elements
class Paragraph : public DocumentElement {
  string text;

public:
  Paragraph(const string &text) : text(text) {}

  const string &getText() const { return text; }

  void accept(DocumentVisitor *visitor) override {
    // TODO: Call visitor->visitParagraph(this)
    visitor->visitParagraph(this);
  }
};

class Heading : public DocumentElement {
  string text;
  int level;

public:
  Heading(const string &text, int level) : text(text), level(level) {}

  const string &getText() const { return text; }

  int getLevel() const { return level; }

  void accept(DocumentVisitor *visitor) override {
    // TODO: Call visitor->visitHeading(this)
    visitor->visitHeading(this);
  }
};

class Image : public DocumentElement {
  string url;
  string altText;

public:
  Image(const string &url, const string &altText)
      : url(url), altText(altText) {}

  const string &getUrl() const { return url; }

  const string &getAltText() const { return altText; }

  void accept(DocumentVisitor *visitor) override {
    // TODO: Call visitor->visitImage(this)
    visitor->visitImage(this);
  }
};

// Visitors
class WordCountVisitor : public DocumentVisitor {
  int wordCount = 0;

public:
  int getWordCount() const { return wordCount; }

  void visitParagraph(Paragraph *p) override {
    // TODO: Split paragraph text by spaces, add count to wordCount
    stringstream ss;
    ss << p->getText();
    string word;
    while (ss >> word) {
      wordCount++;
    }
  }

  void visitHeading(Heading *h) override {
    // TODO: Split heading text by spaces, add count to wordCount
    stringstream ss;
    ss << h->getText();
    string word;
    while (ss >> word) {
      wordCount++;
    }
  }

  void visitImage(Image *img) override {
    // TODO: Images contribute 0 words, nothing to do
  }
};

class HtmlExportVisitor : public DocumentVisitor {
public:
  void visitParagraph(Paragraph *p) override {
    // TODO: Print "<p>text</p>"
    cout << "<p>" << p->getText() << "</p>" << endl;
  }

  void visitHeading(Heading *h) override {
    // TODO: Print "<h{level}>text</h{level}>"
    cout << "<h" << h->getLevel() << ">" << h->getText() << "</h"
         << h->getLevel() << ">" << endl;
  }

  void visitImage(Image *img) override {
    // TODO: Print "<img src=\"url\" alt=\"altText\" />"
    cout << "<img src=\"" << img->getUrl() << "\" alt=\"" << img->getAltText()
         << "\" />" << endl;
  }
};

int main() {
  vector<DocumentElement *> doc = {
      new Heading("My Document", 1),
      new Paragraph("This is the first paragraph with some text."),
      new Image("photo.jpg", "A photo"),
      new Paragraph("Another paragraph here.")};

  WordCountVisitor counter;
  for (auto el : doc)
    el->accept(&counter);
  printf("Word count: %d\n", counter.getWordCount());

  HtmlExportVisitor exporter;
  for (auto el : doc)
    el->accept(&exporter);

  for (auto el : doc)
    delete el;
  return 0;
}
