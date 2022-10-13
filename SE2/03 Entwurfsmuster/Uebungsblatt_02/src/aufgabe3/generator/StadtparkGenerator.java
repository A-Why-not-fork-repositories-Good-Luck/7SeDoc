package aufgabe3.generator;

import aufgabe3.objects.*;

public class StadtparkGenerator extends AbstractWorldGenerator{
    @Override
    public Boden bodenLegen() {
        return new Gras();
    }

    @Override
    public Pflanze pflanzeSetzen() {
        return new Rosen();
    }

    @Override
    public Umrandung umranden() {
        return new Hecke();
    }
}
