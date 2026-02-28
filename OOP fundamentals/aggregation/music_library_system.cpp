#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Artist {
private:
  string name;

public:
  Artist(const string& name) : name(name) {}
  string getName() const { return name; }
};

class Song {
private:
  string title;
  Artist* artist;
  int duration;
public:
  Song(const string& title, Artist* artist, int duration) :
    title(title), artist(artist), duration(duration) {}

  string getTitle() const { return title; }
  Artist* getArtist() const { return artist; }
  int getDuration() const { return duration; }

  string toString() const {
    return title + " by " + artist->getName() + "("  + to_string(duration) + "s)";
  }
};

class Playlist {
private:
  string name;
  vector<Song*> songs;
public:
  Playlist(const string& name) : name(name) {}

  void addSong(Song* song) {
    songs.push_back(song);
  }

  void removeSong(Song* song) {
    songs.erase(remove(songs.begin(), songs.end(), song), songs.end());
  }

  int getSongCount() const {
    return songs.size();
  }

  int getTotalDuration() const {
    int total = 0;
    for(auto* song : songs) total += song->getDuration();
    return total;
  }

  string getName() const { return name; }
  vector<Song*> getSongs() const { return songs; }
};

class User {
private:
  string name;
  vector<Playlist*> playlists;
public:
  User(const string& name) : name(name) {}

  Playlist* createPlaylist(const string& playlistName) {
    auto* playlist = new Playlist(playlistName);
    playlists.push_back(playlist);
    return playlist;
  }

  void deletePlaylist(Playlist* playlist) {
    playlists.erase(remove(playlists.begin(), playlists.end(), playlist), playlists.end());
    delete playlist;
  }

  string getName() const { return name; }
  vector<Playlist*> getPlaylists() const { return playlists; }
};

class Library {
private:
  vector<Song*> songs;
public:
  void addSong(Song* song) { songs.push_back(song); }
  int getSongCount() const { return songs.size(); }
  vector<Song*> getSongs() const { return songs; }
};

int main() {
    Artist coldplay("Coldplay");
    Artist adele("Adele");

    Song yellow("Yellow", &coldplay, 269);
    Song clocks("Clocks", &coldplay, 307);
    Song hello("Hello", &adele, 295);
    Song someone("Someone Like You", &adele, 285);

    Library library;
    library.addSong(&yellow);
    library.addSong(&clocks);
    library.addSong(&hello);
    library.addSong(&someone);

    User alice("Alice");
    Playlist* workout = alice.createPlaylist("Workout Mix");
    Playlist* chill = alice.createPlaylist("Chill Vibes");

    workout->addSong(&yellow);
    workout->addSong(&clocks);
    workout->addSong(&hello);

    chill->addSong(&hello);
    chill->addSong(&someone);

    cout << "Library has " << library.getSongCount() << " songs" << endl;
    cout << endl;

    cout << workout->getName() << " (" << workout->getSongCount() << " songs, "
         << workout->getTotalDuration() << "s):" << endl;
    for (auto* s : workout->getSongs())
        cout << "  - " << s->toString() << endl;
    cout << endl;

    cout << chill->getName() << " (" << chill->getSongCount() << " songs, "
         << chill->getTotalDuration() << "s):" << endl;
    for (auto* s : chill->getSongs())
        cout << "  - " << s->toString() << endl;
    cout << endl;

    string workoutName = workout->getName();
    alice.deletePlaylist(workout);
    cout << "After deleting '" << workoutName << "':" << endl;
    cout << "  Library still has " << library.getSongCount() << " songs" << endl;
    cout << "  '" << chill->getName() << "' still has " << chill->getSongCount() << " songs" << endl;
    cout << "  'Yellow' still exists: " << yellow.getTitle() << endl;

    return 0;
}
