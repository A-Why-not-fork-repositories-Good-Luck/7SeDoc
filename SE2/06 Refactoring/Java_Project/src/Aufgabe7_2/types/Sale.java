package Aufgabe7_2.types;

import Aufgabe7_2.Garment_2;

public class Sale implements Discount_type {
    @Override
    public double getPrice(Garment_2 g) {
        return g.getNormalPrice() * g.getDiscount();
    }
}
