interface Thermometer {
    getTemperature(): number;  // Returns Celsius
}

class CelsiusSensor implements Thermometer {
    getTemperature(): number { return 25.0; }
}

class FahrenheitSensor {
    readFahrenheit(): number { return 98.6; }
}

class FahrenheitSensorAdapter implements Thermometer {
    // TODO: Declare a private FahrenheitSensor field
    fs: FahrenheitSensor;

    constructor(sensor: FahrenheitSensor) {
        this.fs = sensor;
    }

    getTemperature(): number {
        // TODO: Read the Fahrenheit value from the sensor
        // TODO: Convert Fahrenheit to Celsius using (F - 32) * 5.0 / 9.0
        return (this.fs.readFahrenheit() - 32) * 5.0 / 9.0; // TODO: Return the converted Celsius value
    }
}

const celsius: Thermometer = new CelsiusSensor();
console.log(`Celsius sensor: ${celsius.getTemperature().toFixed(1)} C`);

// TODO: Uncomment the lines below after implementing the adapter
const sensor = new FahrenheitSensor();
const adapted: Thermometer = new FahrenheitSensorAdapter(sensor);
console.log(`Fahrenheit sensor (adapted): ${adapted.getTemperature().toFixed(1)} C`);
