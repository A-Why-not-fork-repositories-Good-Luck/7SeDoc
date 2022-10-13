package aufgabe4;

import aufgabe4.Ausstattung.*;
import aufgabe4.Modelle.*;

public class prog {
    public static void main(String[] args) {
        Grundmodell auto1 = new Navigationssystem(new Klimaanlage(new Lederausstattung(new Modell_A())));

        System.out.println(auto1.getPreis());
        System.out.println(auto1.getBeschreibung());
    }
}
