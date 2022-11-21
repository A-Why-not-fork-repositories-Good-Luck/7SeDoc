package Aufgabe7_2.types;

import Aufgabe7_2.Garment_2;

public class Normal implements Discount_type {
    @Override
    public double getPrice(Garment_2 g) {
        return g.getNormalPrice();
    }
}
