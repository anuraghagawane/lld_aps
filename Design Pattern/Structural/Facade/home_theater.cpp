#include <iostream>
#include <string>

using namespace std;

class Amplifier {
public:
  void on() { cout << "Amplifier: Powering on." << endl; }

  void off() { cout << "Amplifier: Shutting down." << endl; }

  void setVolume(int level) {
    cout << "Amplifier: Volume set to" << level << "." << endl;
  }
};

class DvdPlayer {
public:
  void on() { cout << "DVD Player: Powering on." << endl; }

  void off() { cout << "DVD Player: Shutting down." << endl; }

  void play(string movie) {
    cout << "DVD Player: Playing '" << movie << "'." << endl;
  }

  void stop() { cout << "DVD Player: Stopped." << endl; }
};

class Projector {
public:
  void on() { cout << "Projector: Warming up." << endl; }

  void off() { cout << "Projector: Cooling down." << endl; }

  void wideScreenMode() {
    cout << "Projector: Widescreen mode enabled." << endl;
  }
};

class SmartLights {
public:
  void dim(int level) { cout << "Lights: Dimmed to " << level << "%." << endl; }

  void on() { cout << "Lights: Full brightness." << endl; }
};

class StreamingService {
public:
  void connect() { cout << "Streaming: Connected to service." << endl; }

  void disconnect() { cout << "Streaming: Disconnected." << endl; }

  void stream(string movie) {
    cout << "Streaming: Now streaming '" << movie << "'." << endl;
  }
};

class HomeTheaterFacade {
private:
  Amplifier &amp;
  DvdPlayer &dvd;
  Projector &projector;
  SmartLights &lights;
  StreamingService &streaming;

public:
  HomeTheaterFacade(Amplifier &amp, DvdPlayer &dvd, Projector &projector,
                    SmartLights &lights, StreamingService &streaming)
      : amp(amp), dvd(dvd), projector(projector), lights(lights),
        streaming(streaming) {}

  void watchMovie(string movie) {
    cout << "\n--- Preparing to watch: " << movie << " ---" << endl;
    lights.dim(15);
    projector.on();
    projector.wideScreenMode();
    amp.on();
    amp.setVolume(20);
    streaming.connect();
    streaming.stream(movie);
    cout << "--- Enjoy the movie! ---\n" << endl;
  }

  void endMovie() {
    cout << "\n--- Shutting down home theater ---" << endl;
    streaming.disconnect();
    amp.off();
    projector.off();
    lights.on();
    cout << "--- Home theater off ---\n" << endl;
  }
};

int main() {
  Amplifier amp;
  DvdPlayer dvd;
  Projector projector;
  SmartLights lights;
  StreamingService streaming;

  HomeTheaterFacade theater(amp, dvd, projector, lights, streaming);

  theater.watchMovie("Interstellar");
  theater.endMovie();

  return 0;
}
