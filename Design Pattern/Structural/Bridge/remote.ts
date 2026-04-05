// Implementor
interface Device {
    isEnabled(): boolean;
    enable(): void;
    disable(): void;
    getVolume(): number;
    setVolume(volume: number): void;
}

// ConcreteImplementor: TV
class TV implements Device {
    private enabled = false;
    private volume = 30;

    isEnabled(): boolean {
        return this.enabled;
    }

    enable(): void {
        this.enabled = true;
        console.log("TV: Turned ON");
    }

    disable(): void {
        this.enabled = false;
        console.log("TV: Turned OFF");
    }

    getVolume(): number {
        return this.volume;
    }

    setVolume(volume: number): void {
        this.volume = Math.max(0, Math.min(100, volume));
        console.log(`TV: Volume set to ${this.volume}`);
    }
}

// ConcreteImplementor: Radio
class Radio implements Device {
    private enabled = false;
    private volume = 20;

    isEnabled(): boolean {
        return this.enabled;
    }

    enable(): void {
        this.enabled = true;
        console.log("Radio: Turned ON");
    }

    disable(): void {
        this.enabled = false;
        console.log("Radio: Turned OFF");
    }

    getVolume(): number {
        return this.volume;
    }

    setVolume(volume: number): void {
        this.volume = Math.max(0, Math.min(100, volume));
        console.log(`Radio: Volume set to ${this.volume}`);
    }
}

// Abstraction
abstract class Remote {
    protected device: Device;

    constructor(device: Device) {
        this.device = device;
    }

    togglePower(): void {
        if (this.device.isEnabled()) {
            this.device.disable();
        } else {
            this.device.enable();
        }
    }

    volumeUp(): void {
        this.device.setVolume(this.device.getVolume() + 10);
    }

    volumeDown(): void {
        this.device.setVolume(this.device.getVolume() - 10);
    }
}

// RefinedAbstraction: BasicRemote
class BasicRemote extends Remote {
    constructor(device: Device) {
        super(device);
    }
}

// RefinedAbstraction: AdvancedRemote
class AdvancedRemote extends Remote {
    constructor(device: Device) {
        super(device);
    }

    mute(): void {
        this.device.setVolume(0);
        console.log("AdvancedRemote: Muted");
    }
}

console.log("--- Basic Remote with TV ---");
const tv: Device = new TV();
const basicRemote: Remote = new BasicRemote(tv);
basicRemote.togglePower();
basicRemote.volumeUp();
basicRemote.volumeUp();
basicRemote.volumeDown();

console.log("\n--- Advanced Remote with Radio ---");
const radio: Device = new Radio();
const advancedRemote = new AdvancedRemote(radio);
advancedRemote.togglePower();
advancedRemote.volumeUp();
advancedRemote.mute();

console.log("\n--- Advanced Remote with TV ---");
const tvAdvanced = new AdvancedRemote(tv);
tvAdvanced.volumeUp();
tvAdvanced.mute();
