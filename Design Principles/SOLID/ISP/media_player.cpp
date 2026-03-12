#include <iostream>
#include <string>

using namespace std;

// Audio-only capabilities
class AudioPlayerControls {
public:
  virtual void playAudio(const string &audioFile) = 0;
  virtual void stopAudio() = 0;
  virtual void adjustAudioVolume(int volume) = 0;
  virtual ~AudioPlayerControls() = default;
};

// Video-only capabilities
class VideoPlayerControls {
public:
  virtual void playVideo(const string &videoFile) = 0;
  virtual void stopVideo() = 0;
  virtual void adjustVideoBrightness(int brightness) = 0;
  virtual void displaySubtitles(const string &subtitleFile) = 0;
  virtual ~VideoPlayerControls() = default;
};

class ModernAudioPlayer : public AudioPlayerControls {
public:
  void playAudio(const string &audioFile) override {
    cout << "ModernAudioPlayer: Playing audio - " << audioFile << endl;
  }

  void stopAudio() override {
    cout << "ModernAudioPlayer: Audio stopped." << endl;
  }

  void adjustAudioVolume(int volume) override {
    cout << "ModernAudioPlayer: Volume set to " << volume << endl;
  }
};

class SilentVideoPlayer : public VideoPlayerControls {
public:
  void playVideo(const string &videoFile) override {
    cout << "SilentVideoPlayer: Playing video - " << videoFile << endl;
  }

  void stopVideo() override {
    cout << "SilentVideoPlayer: Video stopped." << endl;
  }

  void adjustVideoBrightness(int brightness) override {
    cout << "SilentVideoPlayer: Brightness set to " << brightness << endl;
  }

  void displaySubtitles(const string &subtitleFile) override {
    cout << "SilentVideoPlayer: Subtitles from " << subtitleFile << endl;
  }
};

class ComprehensiveMediaPlayer : public AudioPlayerControls,
                                 public VideoPlayerControls {
public:
  void playAudio(const string &audioFile) override {
    cout << "ComprehensiveMediaPlayer: Playing audio - " << audioFile << endl;
  }

  void stopAudio() override {
    cout << "ComprehensiveMediaPlayer: Audio stopped." << endl;
  }

  void adjustAudioVolume(int volume) override {
    cout << "ComprehensiveMediaPlayer: Audio volume set to " << volume << endl;
  }

  void playVideo(const string &videoFile) override {
    cout << "ComprehensiveMediaPlayer: Playing video - " << videoFile << endl;
  }

  void stopVideo() override {
    cout << "ComprehensiveMediaPlayer: Video stopped." << endl;
  }

  void adjustVideoBrightness(int brightness) override {
    cout << "ComprehensiveMediaPlayer: Brightness set to " << brightness
         << endl;
  }

  void displaySubtitles(const string &subtitleFile) override {
    cout << "ComprehensiveMediaPlayer: Subtitles from " << subtitleFile << endl;
  }
};

int main() {
  ModernAudioPlayer mdp;
  SilentVideoPlayer svp;
  ComprehensiveMediaPlayer cmp;

  mdp.playAudio("Song.mp3");
  mdp.adjustAudioVolume(10);
  mdp.stopAudio();

  svp.playVideo("video.mp4");
  svp.adjustVideoBrightness(90);
  svp.displaySubtitles("Subtitles");
  svp.stopVideo();

  cmp.playAudio("Song.mp3");
  cmp.playVideo("video.mp4");
  cmp.adjustAudioVolume(10);
  cmp.adjustVideoBrightness(90);
  cmp.displaySubtitles("Subtitles");
  cmp.stopAudio();
  cmp.stopVideo();
  return 0;
}
