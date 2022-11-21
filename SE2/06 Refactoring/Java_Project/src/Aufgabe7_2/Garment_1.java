package Aufgabe7_2;

public class Garment_1 {
    public enum Type {
        NORMAL, SALE, SUPERSALE;
    }

    private final Type _type;

    private double price;
    private double discount;

    public Garment_1(Type type, double price, double discount) {
        this._type = type;
    }

    double getNormalPrice() {
        return price;
    }

    double getDiscount() {
        return discount;
    }

    double getPrice() {
        return switch (_type) {
            case NORMAL -> getNormalPrice();
            case SALE -> getNormalPrice() * getDiscount();
            case SUPERSALE -> getNormalPrice() * 0.5;
        };
    }
}

