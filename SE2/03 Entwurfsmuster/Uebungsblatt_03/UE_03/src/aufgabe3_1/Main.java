package aufgabe3_1;

public class Main {
    public static void main(String[] args) {
        WeatherStation station = new WeatherStation();
        ProxyWeather proxyWeather = new ProxyWeather(station);

        System.out.println("Fahrenheit: " + proxyWeather.getTemperature());
        System.out.println("Celsius: " + station.getTemperature());
    }
}