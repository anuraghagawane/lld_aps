interface WeatherObserver {
    update(station: WeatherStation): void;
}

class WeatherStation {
    private temperature: number = 0;
    private humidity: number = 0;
    private pressure: number = 0;
    private observers: WeatherObserver[] = [];

    registerObserver(observer: WeatherObserver): void {
        // TODO: Add observer to the list
        this.observers.push(observer);
    }

    removeObserver(observer: WeatherObserver): void {
        // TODO: Remove observer from the list
        this.observers = this.observers.filter(o => o !== observer);
    }

    private notifyObservers(): void {
        // TODO: Call update on each observer, passing this
        this.observers.forEach(observer => {
            observer.update(this);
        })
    }

    setMeasurements(temperature: number, humidity: number, pressure: number): void {
        // TODO: Update fields and notify observers
        this.temperature = temperature;
        this.humidity = humidity;
        this.pressure = pressure;
        this.notifyObservers();
    }

    getTemperature(): number { return this.temperature; }
    getHumidity(): number { return this.humidity; }
    getPressure(): number { return this.pressure; }
}

class CurrentConditionsDisplay implements WeatherObserver {
    update(station: WeatherStation): void {
        // TODO: Print "Current Conditions -> Temp: X, Humidity: Y%, Pressure: Z hPa"
        console.log(`Current Conditions -> Temp: ${station.getTemperature().toFixed(1)}, Humidity: ${station.getHumidity().toFixed(1)}%, Pressure: ${station.getPressure().toFixed(1)} hPa`);
    }
}

class StatisticsDisplay implements WeatherObserver {
    private readings: number[] = [];

    update(station: WeatherStation): void {
        // TODO: Add temperature to readings, compute average, print "Statistics -> Avg Temperature: X"
        this.readings.push(station.getTemperature());
        const sum = this.readings.reduce((acc, reading) => {
            acc += reading;
            return acc;
        }, 0);
        const avg = sum / this.readings.length;
        console.log("Statistics -> Avg Temperature:", avg.toFixed(1));
    }
}

const station = new WeatherStation();
const current = new CurrentConditionsDisplay();
const stats = new StatisticsDisplay();
station.registerObserver(current);
station.registerObserver(stats);
station.setMeasurements(25.0, 65.0, 1013.0);
station.setMeasurements(28.0, 70.0, 1012.0);
station.setMeasurements(22.0, 90.0, 1011.0);
