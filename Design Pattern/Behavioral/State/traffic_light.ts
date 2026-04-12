interface TrafficLightState {
    change(context: TrafficLight): void;
}

class RedState implements TrafficLightState {
    change(context: TrafficLight): void {
        // TODO: Print "RED light - Stop" and transition to GreenState
        console.log("RED light - Stop");
        context.setState(new GreenState());
    }
}

class GreenState implements TrafficLightState {
    change(context: TrafficLight): void {
        // TODO: Print "GREEN light - Go" and transition to YellowState
        console.log("GREEN light - Go");
        context.setState(new YellowState());
    }
}

class YellowState implements TrafficLightState {
    change(context: TrafficLight): void {
        // TODO: Print "YELLOW light - Slow down" and transition to RedState
        console.log("YELLOW light - Slow down");
        context.setState(new RedState());
    }
}

class TrafficLight {
    private state: TrafficLightState;

    constructor() {
        this.state = new RedState(); // Start with red
    }

    setState(state: TrafficLightState): void {
        this.state = state;
    }
    change(): void {
        this.state.change(this);
    }
}

const light = new TrafficLight();
light.change(); // RED light - Stop
light.change(); // GREEN light - Go
light.change(); // YELLOW light - Slow down
light.change(); // RED light - Stop
light.change(); // GREEN light - Go
