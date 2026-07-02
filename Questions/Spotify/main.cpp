#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace std;

class Artist {
private:
  string id;
  string name;
  string country;

public:
  Artist(const string &id, const string &name, const string &country)
      : id(id), name(name), country() {}

  string getId() const { return id; }

  string getName() const { return name; }
  string getCountry() const { return country; }
};

class Song {
private:
  string id;
  string name;
  string audioUrl;
  vector<shared_ptr<Artist>> artists;

public:
  Song(const string &id, const string &name, const string &audioUrl,
       vector<shared_ptr<Artist>> artists)
      : id(id), name(name), audioUrl(audioUrl), artists(artists) {}

  string getId() const { return id; }
  string getName() const { return name; }
  string getAudioUrl() const { return audioUrl; }
  vector<shared_ptr<Artist>> getArtists() const { return artists; }
};

class Album {
private:
  string id;
  string name;
  vector<shared_ptr<Song>> songs;

public:
  Album(const string &id, const string &name, vector<shared_ptr<Song>> songs)
      : id(id), name(name), songs(songs) {}

  string getId() const { return id; }
  string getName() const { return name; }
  vector<shared_ptr<Song>> getSongs() const { return songs; }

  void display() {
    cout << "============================================" << endl;
    cout << "Album: " << name << endl;
    for (auto song : songs) {
      cout << "  - " << song->getName() << endl;
    }
    cout << "============================================" << endl;
  }
};

class Playlist {
private:
  string id;
  string name;
  vector<shared_ptr<Song>> songs;
  string owner_id;

public:
  Playlist(const string &id, const string &name, const string &owner_id)
      : id(id), name(name), owner_id(owner_id) {}

  string getId() const { return id; }
  string getName() const { return name; }
  string getOwnerId() const { return owner_id; }

  vector<shared_ptr<Song>> getSongs() { return songs; }

  void addSong(shared_ptr<Song> song) { songs.push_back(song); }
  void display() {
    cout << "============================================" << endl;
    cout << "Playlist: " << name << endl;
    for (auto song : songs) {
      cout << "  - " << song->getName() << endl;
    }
    cout << "============================================" << endl;
  }
};

class MusicPlayer {
private:
  shared_ptr<Song> currentSongs;
  queue<shared_ptr<Song>> songQueue;

public:
  void play() {
    if (!currentSongs) {
      currentSongs = songQueue.front();
      songQueue.pop();
    }

    cout << "Now Playing: " << currentSongs->getName() << endl;
  }

  void pause() {
    if (!currentSongs) {
      cout << "Cannot perform this action" << endl;
      return;
    }
    cout << "Now Pausing: " << currentSongs->getName() << endl;
  }

  void next() {
    pause();
    if (!currentSongs) {
      if (songQueue.empty()) {
        cout << "Queue finished play other songs" << endl;
        return;
      }
    }
    currentSongs = songQueue.front();
    songQueue.pop();
    play();
  }

  void loadSongs(vector<shared_ptr<Song>> songs) {
    currentSongs.reset();
    while (!songQueue.empty()) {
      songQueue.pop();
    }

    for (auto song : songs) {
      songQueue.push(song);
    }
    play();
  }
};

class User {
private:
  string id;
  string name;
  vector<string> playlists;

public:
  User(const string &id, const string &name) : id(id), name(name) {}

  string getId() const { return id; }
  string getName() const { return name; }

  vector<string> getPlaylist() { return playlists; }

  void addPlaylist(const string &playlistId) {
    playlists.push_back(playlistId);
  }
};

class UserService {
private:
  unordered_map<string, shared_ptr<User>> users;

public:
  string createUser(const string &id, const string &name) {
    auto user = make_shared<User>(id, name);
    users[user->getId()] = user;
    return user->getId();
  }

  shared_ptr<User> getUser(const string &userId) {
    if (!users.count(userId)) {
      throw runtime_error("User not found!");
    }
    return users[userId];
  }

  void addPlaylist(const string &userId, const string &playlistId) {
    if (!users.count(userId)) {
      throw runtime_error("User not found!");
    }
    users[userId]->addPlaylist(playlistId);
  }
};

class CatalogService {
private:
  unordered_map<string, shared_ptr<Song>> songs;
  unordered_map<string, shared_ptr<Album>> albums;
  unordered_map<string, shared_ptr<Playlist>> playlists;
  CatalogService() = default;

public:
  CatalogService(CatalogService &) = delete;
  CatalogService &operator=(const CatalogService) = delete;

  static CatalogService &getInstance() {
    static CatalogService instance;
    return instance;
  }

  vector<shared_ptr<Song>> getSongs() { return {}; }

  shared_ptr<Song> getSong(const string &songId) {
    if (!songs.count(songId)) {
      throw runtime_error("No song found!");
    }

    return songs[songId];
  }

  shared_ptr<Album> getAlbum(const string &albumId) {
    if (!albums.count(albumId)) {
      throw runtime_error("No album found!");
    }

    return albums[albumId];
  }

  shared_ptr<Playlist> getPlaylist(const string &playlistId) {
    if (!playlists.count(playlistId)) {
      throw runtime_error("No playlist found!");
    }

    return playlists[playlistId];
  }

  void addSong(shared_ptr<Song> song) { songs[song->getId()] = song; }

  void addAlbum(shared_ptr<Album> album) { albums[album->getId()] = album; }

  void addPlaylist(shared_ptr<Playlist> playlist) {
    playlists[playlist->getId()] = playlist;
  }

  void displayAlbum(const string &albumId) {
    if (!albums.count(albumId)) {
      throw runtime_error("No album found!");
    }

    albums[albumId]->display();
  }

  void displayPlaylist(const string &playlistId) {
    if (!playlists.count(playlistId)) {
      throw runtime_error("No playlist found!");
    }

    playlists[playlistId]->display();
  }
};

class RecommendationService {
  CatalogService &catalogService;

public:
  RecommendationService() : catalogService(CatalogService::getInstance()) {}
  vector<shared_ptr<Song>> getRecommendedSongs(const string &userId) {
    return catalogService.getSongs();
  }
};

class AppService {
private:
  UserService *userService;
  RecommendationService *recommendationService;
  CatalogService &catalogService;
  unordered_map<string, unique_ptr<MusicPlayer>> sessions;

public:
  AppService()
      : userService(new UserService()),
        recommendationService(new RecommendationService),
        catalogService(CatalogService::getInstance()) {}

  string createUser(const string &id, const string &name) {
    return userService->createUser(id, name);
  }

  string login(const string &userId, const string &sessionId) {
    const string sessionKey = userId + "_" + sessionId;
    sessions[sessionKey] = make_unique<MusicPlayer>();
    cout << "Login Successful, sessionId: " << sessionKey << endl;
    return sessionKey;
  }

  void logout(const string &sessionKey) { sessions.erase(sessionKey); }

  string addSong(const string &id, const string &name, const string &audioUrl,
                 vector<shared_ptr<Artist>> artists) {
    catalogService.addSong(make_shared<Song>(id, name, audioUrl, artists));

    return id;
  }

  string addAlbum(const string &id, const string &name,
                  vector<string> songIds) {
    vector<shared_ptr<Song>> songs;
    for (auto songId : songIds) {
      songs.push_back(catalogService.getSong(songId));
    }
    catalogService.addAlbum(make_shared<Album>(id, name, songs));
    return id;
  }

  void displayAlbum(const string &albumId) {}

  void playSong(const string &sessionKey, const string &userId,
                const string &songId) {
    if (!sessions.count(sessionKey)) {
      throw runtime_error("Session does not exist");
    }
    auto &sessionPlayer = sessions[sessionKey];
    vector<shared_ptr<Song>> songs = {catalogService.getSong(songId)};
    auto recSongs = recommendationService->getRecommendedSongs(userId);
    songs.insert(songs.end(), recSongs.begin(), recSongs.end());
    sessionPlayer->loadSongs(songs);
  }

  void pauseSong(const string &sessionKey) {
    if (!sessions.count(sessionKey)) {
      throw runtime_error("Session does not exist");
    }

    auto &sessionPlayer = sessions[sessionKey];
    sessionPlayer->pause();
  }

  void playNext(const string &sessionKey) {
    if (!sessions.count(sessionKey)) {
      throw runtime_error("Session does not exist");
    }

    auto &sessionPlayer = sessions[sessionKey];
    sessionPlayer->next();
  }

  string createPlaylist(const string &id, const string &name,
                        const string &owner_id) {
    auto playList = make_shared<Playlist>(id, name, owner_id);
    userService->addPlaylist(owner_id, id);
    catalogService.addPlaylist(playList);
    return id;
  }

  void addSongToPlaylist(const string &playlistId, const string &songId) {
    auto playList = catalogService.getPlaylist(playlistId);
    playList->addSong(catalogService.getSong(songId));
  }

  void loadPlaylist(const string &sessionKey, const string &userId,
                    const string &playlistId) {
    if (!sessions.count(sessionKey)) {
      throw runtime_error("Session does not exist");
    }

    auto &sessionPlayer = sessions[sessionKey];
    auto songs = catalogService.getPlaylist(playlistId)->getSongs();
    sessionPlayer->loadSongs(songs);
  }

  void displayAlbum(const string &albumId) const {
    catalogService.displayAlbum(albumId);
  }

  void displayPlaylist(const string &playlistId) const {
    catalogService.displayPlaylist(playlistId);
  }
};

int main() {
  auto appService = new AppService();
  shared_ptr<Artist> a1 = make_shared<Artist>("a1", "ach", "india");
  auto s1 = appService->addSong("S1", "ABCD", "/abcd.mp3", {a1});
  auto s2 = appService->addSong("S2", "EFGH", "/efgh.mp3", {a1});

  shared_ptr<Artist> a2 = make_shared<Artist>("a2", "kdkd", "india");
  auto s3 = appService->addSong("S3", "XY", "/XY.mp3", {a2});
  auto s4 = appService->addSong("S4", "ZA", "/ZA.mp3", {a2});

  appService->addAlbum("A1", "Ding Dong", {s1, s2});
  auto u1 = appService->createUser("U1", "Anurag");

  auto p1 = appService->createPlaylist("P1", "My fav", u1);
  appService->addSongToPlaylist(p1, s1);
  appService->addSongToPlaylist(p1, s2);
  appService->displayPlaylist(p1);

  auto ss1 = appService->login(u1, "ss1");

  appService->loadPlaylist(ss1, u1, p1);
  appService->playNext(ss1);
  appService->displayAlbum("A1");
  appService->playSong(ss1, u1, s3);

  return 0;
}
