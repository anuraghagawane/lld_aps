class Amplifier {
  on(): void {
    console.log("Amplifier: Powering on.");
  }
  off(): void {
    console.log("Amplifier: Shutting down.");
  }
  setVolume(level: number): void {
    console.log("Amplifier: Volume set to " + level + ".");
  }
}

class DvdPlayer {
  on(): void {
    console.log("DVD Player: Powering on.");
  }
  off(): void {
    console.log("DVD Player: Shutting down.");
  }
  play(movie: string): void {
    console.log("DVD Player: Playing '" + movie + "'.");
  }
  stop(): void {
    console.log("DVD Player: Stopped.");
  }
}

class Projector {
  on(): void {
    console.log("Projector: Warming up.");
  }
  off(): void {
    console.log("Projector: Cooling down.");
  }
  wideScreenMode(): void {
    console.log("Projector: Widescreen mode enabled.");
  }
}

class SmartLights {
  dim(level: number): void {
    console.log("Lights: Dimmed to " + level + "%.");
  }
  on(): void {
    console.log("Lights: Full brightness.");
  }
}

class StreamingService {
  connect(): void {
    console.log("Streaming: Connected to service.");
  }
  disconnect(): void {
    console.log("Streaming: Disconnected.");
  }
  stream(movie: string): void {
    console.log("Streaming: Now streaming '" + movie + "'.");
  }
}

class HomeTheaterFacade {
  private amp: Amplifier;
  private dvd: DvdPlayer;
  private projector: Projector;
  private lights: SmartLights;
  private streaming: StreamingService;

  constructor(
    amp: Amplifier,
    dvd: DvdPlayer,
    projector: Projector,
    lights: SmartLights,
    streaming: StreamingService
  ) {
    this.amp = amp;
    this.dvd = dvd;
    this.projector = projector;
    this.lights = lights;
    this.streaming = streaming;
  }

  watchMovie(movie: string): void {
    console.log("\n--- Preparing to watch: " + movie + " ---");
    this.lights.dim(15);
    this.projector.on();
    this.projector.wideScreenMode();
    this.amp.on();
    this.amp.setVolume(20);

    // Optional: actually use DVD too (like the C# version)
    // this.dvd.on();
    // this.dvd.play(movie);

    this.streaming.connect();
    this.streaming.stream(movie);

    console.log("--- Enjoy the movie! ---\n");
  }

  endMovie(): void {
    console.log("\n--- Shutting down home theater ---");

    this.streaming.disconnect();

    // Optional:
    // this.dvd.stop();
    // this.dvd.off();

    this.amp.off();
    this.projector.off();
    this.lights.on();

    console.log("--- Home theater off ---\n");
  }

}

const amp = new Amplifier();
const dvd = new DvdPlayer();
const projector = new Projector();
const lights = new SmartLights();
const streaming = new StreamingService();

const theater = new HomeTheaterFacade(amp, dvd, projector, lights, streaming);

theater.watchMovie("Interstellar");
theater.endMovie();
