interface SmartHomeMediator {
    notify(device: SmartDevice, event: string): void;
    registerDevice(device: SmartDevice): void;
}

abstract class SmartDevice {
    protected mediator: SmartHomeMediator;
    protected name: string;

    constructor(mediator: SmartHomeMediator, name: string) {
        this.mediator = mediator;
        this.name = name;
    }

    getName(): string {
        return this.name;
    }
    abstract receiveCommand(command: string): void;
}

// Colleagues
class Thermostat extends SmartDevice {
    private temperature: number = 72;

    constructor(mediator: SmartHomeMediator, name: string) {
        super(mediator, name);
    }

    setTemperature(temp: number): void {
        // TODO: Set temperature field to temp
        // TODO: Print "Name: Temperature set to X"
        // TODO: If temp > 75, notify mediator with "TEMP_HIGH"
        // TODO: If temp <= 75, notify mediator with "TEMP_NORMAL"
        this.temperature = temp;
        console.log(`${this.name}: Temperature set to ${this.temperature}`)
        if(this.temperature > 75) {
            this.mediator.notify(this, "TEMP_HIGH");
        }
        else {
            this.mediator.notify(this, "TEMP_NORMAL");
        }

    }

    receiveCommand(command: string): void {
        // TODO: Handle commands if needed
    }
}

class Lights extends SmartDevice {
    constructor(mediator: SmartHomeMediator, name: string) {
        super(mediator, name);
    }

    receiveCommand(command: string): void {
        // TODO: Print "Name: Turned ON" or "Name: Turned OFF" based on command
        console.log(`${this.name}: Turned ${command}`);
    }
}

class AirConditioner extends SmartDevice {
    constructor(mediator: SmartHomeMediator, name: string) {
        super(mediator, name);
    }

    receiveCommand(command: string): void {
        // TODO: Print "Name: Turned ON" or "Name: Turned OFF" based on command
        console.log(`${this.name}: Turned ${command}`);
    }
}

// Concrete Mediator
class SmartHomeHub implements SmartHomeMediator {
    private devices: SmartDevice[] = [];

    registerDevice(device: SmartDevice): void {
        // TODO: Add device to the list
        this.devices.push(device);
    }

    notify(device: SmartDevice, event: string): void {
        // TODO: Print "[SmartHomeHub] EVENT detected. Action message."
        // TODO: "TEMP_HIGH" -> find AirConditioner, send "ON" command
        // TODO: "TEMP_NORMAL" -> find AirConditioner, send "OFF" command
        // TODO: "NIGHT_MODE" -> find Lights, send "OFF" command
        if (event === "TEMP_HIGH") {
            console.log("[SmartHomeHub] TEMP_HIGH detected. Turning on AC.");
            for (const d of this.devices) {
                if(d instanceof AirConditioner) d.receiveCommand("ON");
            }
        }
        else if (event === "TEMP_NORMAL") {
            console.log("[SmartHomeHub] TEMP_NORMAL detected. Turning off AC.");
            for (const d of this.devices) {
                if(d instanceof AirConditioner) d.receiveCommand("OFF");
            }
        }
        else if (event === "NIGHT_MODE") {
            console.log("[SmartHomeHub] NIGHT_MODE activated. Turning off lights.");
            for (const d of this.devices) {
                if(d instanceof Lights) d.receiveCommand("OFF");
            }
        }
    }
}

const hub = new SmartHomeHub();
const thermostat = new Thermostat(hub, "Living Room Thermostat");
const lights = new Lights(hub, "Living Room Lights");
const ac = new AirConditioner(hub, "Central AC");
hub.registerDevice(thermostat);
hub.registerDevice(lights);
hub.registerDevice(ac);
thermostat.setTemperature(80);
thermostat.setTemperature(70);
hub.notify(lights, "NIGHT_MODE");
