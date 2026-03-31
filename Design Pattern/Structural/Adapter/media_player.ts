interface MediaPlayer {
  play(filename: string): void;
}

class Mp3Player implements MediaPlayer {
  play(filename: string): void {
    console.log(`MP3 Player: Playing ${filename}`);
  }
}

// External codec libraries (Adaptees)
class VlcCodec {
  playVlc(filename: string): void {
    console.log(`VLC Codec: Decoding and playing ${filename}`);
  }
}

class Mp4Codec {
  playMp4(filename: string): void {
    console.log(`MP4 Codec: Decoding and playing ${filename}`);
  }
}

class VlcPlayerAdapter implements MediaPlayer {
  private codec: VlcCodec;

  constructor(codec: VlcCodec) {
    this.codec = codec;
  }

  play(filename: string): void {
    this.codec.playVlc(filename);
  }
}

class Mp4PlayerAdapter implements MediaPlayer {
  private codec: Mp4Codec;

  constructor(codec: Mp4Codec) {
    this.codec = codec;
  }

  play(filename: string): void {
    this.codec.playMp4(filename);
  }
}

class AudioPlayer {
  playFile(filename: string): void {
    const ext = filename.split(".").pop()?.toLowerCase() ?? "";
    const players: Record<string, () => MediaPlayer> = {
      mp3: () => new Mp3Player(),
      vlc: () => new VlcPlayerAdapter(new VlcCodec()),
      mp4: () => new Mp4PlayerAdapter(new Mp4Codec()),
    };

    const creator = players[ext];
    if (!creator) {
      console.log(`Unsupported format: ${ext}`);
      return;
    }

    creator().play(filename);
  }
}

// Main
const player = new AudioPlayer();
player.playFile("song.mp3");
player.playFile("movie.mp4");
player.playFile("documentary.vlc");
player.playFile("image.png");
