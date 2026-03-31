#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

class MediaPlayer {
public:
  virtual void play(string filename) = 0;
  virtual ~MediaPlayer() {}
};

class Mp3Player : public MediaPlayer {
public:
  void play(string filename) override {
    cout << "MP3 Player: Playing" << filename << endl;
  }
};

class VlcCodec {
public:
  void playVlc(string filename) {
    cout << "VLC Codec: Decoding and playing " << filename << endl;
  }
};

class Mp4Codec {
public:
  void playMp4(string filename) {
    cout << "MP4 Codec: Decoding and playing " << filename << endl;
  }
};

class VlcPlayerAdapter : public MediaPlayer {
private:
  VlcCodec *codec;

public:
  VlcPlayerAdapter(VlcCodec *codec) : codec(codec) {}

  void play(string filename) override { codec->playVlc(filename); }
};

class Mp4PlayerAdapter : public MediaPlayer {
private:
  Mp4Codec *codec;

public:
  Mp4PlayerAdapter(Mp4Codec *codec) : codec(codec) {}
  void play(string filename) override { codec->playMp4(filename); }
};

class AudioPlayer {
public:
  void playFile(string filename) {
    string ext = filename.substr(filename.rfind('.') + 1);
    transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == "mp3") {
      Mp3Player player;
      player.play(filename);
    } else if (ext == "vlc") {
      VlcCodec codec;
      VlcPlayerAdapter adapter(&codec);
      adapter.play(filename);
    } else if (ext == "mp4") {
      Mp4Codec codec;
      Mp4PlayerAdapter adapter(&codec);
      adapter.play(filename);
    } else {
      cout << "Unsupported format: " << ext << endl;
    }
  }
};

int main() {
  AudioPlayer player;
  player.playFile("song.mp3");
  player.playFile("movie.mp4");
  player.playFile("documentary.vlc");
  player.playFile("image.png");
  return 0;
}
