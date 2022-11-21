package Aufgabe7_2;

import Aufgabe7_2.types.Discount_type;

public class Garment_2 {
    private double price;
    private double discount;
    private Discount_type type;

    public Garment_2(double price, double discount) {
        this.price = price;
        this.discount = discount;
    }

    public double getPrice() {
        return type.getPrice(this);
    }

    public double getDiscount() {
        return discount;
    }

    public double getNormalPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public void setDiscount(double discount) {
        this.discount = discount;
    }

    public void setType(Discount_type type) {
        this.type = type;
    }
}
