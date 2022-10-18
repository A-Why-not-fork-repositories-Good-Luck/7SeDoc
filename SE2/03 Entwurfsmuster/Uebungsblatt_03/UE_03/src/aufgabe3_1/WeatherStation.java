package aufgabe3_1;

public class WeatherStation implements IWeather {
    public WeatherStation() {
    }

    @Override
    public double getTemperature() {
        return Math.floor(Math.random()*120);
    }
}
