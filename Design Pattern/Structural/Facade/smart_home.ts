// Subsystem: Controls smart lights in the house
class SmartLightsSystem {
    on(): void {
        // TODO: Print "Lights: Turned on."
        console.log("Lights: Turned on.");
    }

    off(): void {
        // TODO: Print "Lights: Turned off."
        console.log("Lights: Turned off.")
    }
}

// Subsystem: Controls the thermostat temperature and mode
class Thermostat {
    private mode: string = "";

    setTemperature(degrees: number): void {
        // TODO: Print "Thermostat: Mode set to {mode}. Temperature set to {degrees}C."
        console.log(`Thermostat: Mode set to ${this.mode}. Temperature set to ${degrees}C.`);
    }

    setMode(mode: string): void {
        // TODO: Store the mode
        this.mode = mode;
    }
}

// Subsystem: Controls the home security system
class SecuritySystem {
    arm(): void {
        // TODO: Print "Security: System armed."
        console.log("Security: System armed.")
    }

    disarm(): void {
        // TODO: Print "Security: System disarmed."
        console.log("Security: System disarmed.")
    }
}

// Facade: Provides simplified methods to control all smart home subsystems
class SmartHomeFacade {
    private lights: SmartLightsSystem;
    private thermostat: Thermostat;
    private security: SecuritySystem;

    constructor(lights: SmartLightsSystem, thermostat: Thermostat, security: SecuritySystem) {
        // TODO: Store references to all subsystems
        this.lights = lights;
        this.thermostat = thermostat;
        this.security = security;
    }

    leaveHome(): void {
        // TODO: Print "--- Leaving Home ---"
        // TODO: Turn off lights, set thermostat to eco mode (18C), arm security
        // TODO: Print "--- Home secured ---"
        console.log("--- Leaving Home ---")
        this.lights.off();
        this.thermostat.setMode("eco");
        this.thermostat.setTemperature(18);
                
        this.security.arm();
        console.log("--- Home secured ---")
    }

    arriveHome(): void {
        // TODO: Print "--- Arriving Home ---"
        // TODO: Turn on lights, set thermostat to comfort mode (22C), disarm security
        // TODO: Print "--- Welcome home! ---"
        console.log("--- Arriving Home ---")
        this.lights.on();
        this.thermostat.setMode("comfort");
        this.thermostat.setTemperature(22);
        this.security.disarm();
        console.log("--- Welcome home! ---")
    }
}

const lights = new SmartLightsSystem();
const thermostat = new Thermostat();
const security = new SecuritySystem();

const home = new SmartHomeFacade(lights, thermostat, security);
home.leaveHome();
console.log();
home.arriveHome();
