package aufgabe3;

import aufgabe3.objects.*;
import aufgabe3.generator.*;

public class Park {
    private String type;
    private Boden boden;
    private Pflanze pflanze;
    private Umrandung umrandung;

    public Park(String type) throws Exception {
        this.type = type;

        AbstractWorldGenerator world;
        switch (type) {
            case "Klosterpark":
                world = new KlosterparkGenerator();
                break;
            case "Stadtpark":
                world = new StadtparkGenerator();
                break;
            default:
                throw new Exception("Typ nicht gefunden!");
        }

        boden = world.bodenLegen();
        pflanze = world.pflanzeSetzen();
        umrandung = world.umranden();
    }
}
