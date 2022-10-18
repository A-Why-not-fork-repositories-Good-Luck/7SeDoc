package aufgabe3_1;

public class ProxyWeather implements IWeather {
    WeatherStation station;

    public ProxyWeather(WeatherStation station) {
        this.station = station;
    }

    @Override
    public double getTemperature() {
        double tempInFahrenheit = station.getTemperature();
        return Math.floor((tempInFahrenheit - 32) * 5/9);
    }
}
