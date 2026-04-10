#include <iostream>
#include <string>
#include <vector>
using namespace std;

// --- Given classes (do not modify) ---

template <typename T> class Iterator {
public:
  virtual ~Iterator() {}
  virtual bool hasNext() = 0;
  virtual T next() = 0;
};

class Playlist {
private:
  vector<string> songs;

public:
  void addSong(const string &song) { songs.push_back(song); }

  vector<string> getSongs() const { return songs; }

  string getAt(int index) const { return songs[index]; }

  int getSize() const { return songs.size(); }
};

// --- Implement the class below ---

class ReversePlaylistIterator : public Iterator<string> {
private:
  // TODO: Store pointer to Playlist and current index
  Playlist *playlist;
  int index;

public:
  ReversePlaylistIterator(Playlist *pl) {
    // TODO: Initialize index to last position
    this->playlist = pl;
    this->index = playlist->getSize() - 1;
  }

  bool hasNext() override { return index >= 0; }

  string next() override {
    // TODO
    return this->playlist->getAt(index--);
  }
};

int main() {
  Playlist playlist;
  playlist.addSong("Shape of You");
  playlist.addSong("Bohemian Rhapsody");
  playlist.addSong("Blinding Lights");

  ReversePlaylistIterator reverse(&playlist);
  cout << "Reverse Playlist:" << endl;
  while (reverse.hasNext()) {
    cout << "  " << reverse.next() << endl;
  }
  return 0;
}
