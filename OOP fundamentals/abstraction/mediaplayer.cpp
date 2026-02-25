#include <iostream>
#include <string>

using namespace std;

class MediaPlayer {
protected:
  string playerName;

  void logAction(const string& action) {
    cout<<"["<<playerName<<"] Action: "<<action<<endl;
  }

public:
  MediaPlayer(const string& playerName) : playerName(playerName) {}

  virtual ~MediaPlayer() {}

  virtual void play() = 0;
  virtual void pause() = 0;
  virtual void stop() = 0;

  void displayStatus() {
    cout<<"["<<playerName<<"] Status: Ready"<<endl;
  }
};

class AudioPlayer: public MediaPlayer {
private:
  string audioFile;

public:
  AudioPlayer(const string& audioFile) : MediaPlayer("AudioPlayer"), audioFile(audioFile) {}

  void play() override {
    logAction("Playing audio: " + audioFile);
  }

  void pause() override {
    logAction("Paused audio: " + audioFile);
  }

  void stop() override {
    logAction("Stopped audio: " + audioFile);
  }
};

class VideoPlayer:  public MediaPlayer {
private:
  string videoFile;
  string resolution;

public:
  VideoPlayer(const string& videoFile, const string& resolution) :
    MediaPlayer("VideoPlayer"), videoFile(videoFile), resolution(resolution) {}

  void play() override {
    logAction("Playing video: " + videoFile + " at " + resolution);
  }

  void pause() override {
    logAction("Paused video: " + videoFile);
  }

  void stop() override {
    logAction("Stopped video: " + videoFile);
  }
};

class StreamingPlayer: public MediaPlayer {
private:
  string streamUrl;
  int bufferSize;

public:
  StreamingPlayer(const string& streamUrl, int bufferSize) :
    MediaPlayer("StreamingPlayer"), streamUrl(streamUrl), bufferSize(bufferSize) {}

  void play() override {
    logAction("Streaming from: " + streamUrl + " (buffer: " + to_string(bufferSize) + "KB");
  }

  void pause() override {
    logAction("Paused stream: " + streamUrl);
  }

  void stop() override {
    logAction("Stopped stream: " + streamUrl);
  }
};

class PlayerController {
private:
  MediaPlayer* player;

public:
  PlayerController(MediaPlayer* player) : player(player) {}

  void startPlayback() {
    player->displayStatus();
    player->play();
  }

  void pausePlayback() {
    player->pause();
  }

  void stopPlayback() {
    player->stop();
  }
};

int main() {
  AudioPlayer audio("song.mp3");
  PlayerController audioCtrl(&audio);
  audioCtrl.startPlayback();
  audioCtrl.pausePlayback();

  cout<<endl;

  VideoPlayer video("movie.mp4", "1080p");
  PlayerController videoCtrl(&video);
  videoCtrl.startPlayback();
  videoCtrl.stopPlayback();

  cout<<endl;

  StreamingPlayer stream("https://stream.example.com/live", 2048);
  PlayerController streamCtrl(&stream);
  streamCtrl.startPlayback();
  streamCtrl.stopPlayback();

  return 0;
}
