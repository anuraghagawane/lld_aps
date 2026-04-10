// --- Given classes (do not modify) ---

interface MyIterator<T> {
    hasNext(): boolean;
    next(): T;
}

class Playlist {
    private songs: string[] = [];

    addSong(song: string): void {
        this.songs.push(song);
    }

    getSongs(): string[] {
        return this.songs;
    }

    getSize(): number {
        return this.songs.length;
    }

    getAt(index: number): string {
        return this.songs[index];
    }
}

// --- Implement the class below ---

class ReversePlaylistIterator implements MyIterator<string> {
    // TODO: Store reference to Playlist and current index
    private playlist: Playlist;
    private index: number;

    constructor(playlist: Playlist) {
        // TODO: Initialize index to last position
        this.playlist = playlist;
        this.index = playlist.getSize() - 1;
    }

    hasNext(): boolean {
        // TODO
        return this.index >= 0;;
    }

    next(): string {
        // TODO
        return this.playlist.getAt(this.index--) as string;
    }
}

const playlist = new Playlist();
playlist.addSong("Shape of You");
playlist.addSong("Bohemian Rhapsody");
playlist.addSong("Blinding Lights");

const reverse = new ReversePlaylistIterator(playlist);
console.log("Reverse Playlist:");
while (reverse.hasNext()) {
    console.log("  " + reverse.next());
}
