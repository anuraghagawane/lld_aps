#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class TreeType {
public:
  virtual ~TreeType() {}
  virtual void render(int x, int y) = 0;
};

class ConcreteTreeType : public TreeType {
private:
  string name;
  string color;
  string texture;

public:
  ConcreteTreeType(string name, string color, string texture)
      : name(name), color(color), texture(texture) {}

  void render(int x, int y) override {
    cout << "Rendering " << name << " tree [color=" << color
         << ", texture=" << texture << "] at (" << x << "," << y << ")" << endl;
  }
};

class TreeTypeFactory {
private:
  map<string, TreeType *> treeTypes;

public:
  TreeType *getTreeType(const string &name, const string &color,
                        const string &texture) {
    string key = name + "_" + color + "_" + texture;
    if (treeTypes.find(key) == treeTypes.end()) {
      treeTypes[key] = new ConcreteTreeType(name, color, texture);
    }

    return treeTypes[key];
  }

  int getTypeCount() { return treeTypes.size(); }

  ~TreeTypeFactory() {
    for (auto &pair : treeTypes) {
      delete pair.second;
    }
  }
};

class Tree {
private:
  TreeType *type;
  int x, y;

public:
  Tree(TreeType *type, int x, int y) : type(type), x(x), y(y) {}

  void draw() { type->render(x, y); }
};

class Forest {
private:
  TreeTypeFactory factory;
  vector<Tree> trees;

public:
  void plantTree(int x, int y, const string &name, const string &color,
                 const string &texture) {
    TreeType *type = factory.getTreeType(name, color, texture);
    trees.emplace_back(type, x, y);
  }

  void render() {
    for (Tree &tree : trees) {
      tree.draw();
    }
    cout << "\nTotal trees planted: " << trees.size() << endl;
    cout << "Unique tree types created: " << factory.getTypeCount() << endl;
  }
};

int main() {
  Forest forest;

  forest.plantTree(10, 20, "Oak", "dark green", "rough bark");
  forest.plantTree(50, 80, "Pine", "green", "needle texture");
  forest.plantTree(30, 60, "Oak", "dark green", "rough bark");
  forest.plantTree(70, 40, "Birch", "light green", "white bark");
  forest.plantTree(90, 10, "Pine", "green", "needle texture");

  forest.render();

  return 0;
}
