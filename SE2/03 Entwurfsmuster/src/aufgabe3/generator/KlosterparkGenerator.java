package aufgabe3.generator;

import aufgabe3.objects.*;

public class KlosterparkGenerator extends AbstractWorldGenerator{
    @Override
    public Boden bodenLegen() {
        return new Steinplatte();
    }

    @Override
    public Pflanze pflanzeSetzen() {
        return new Kräuter();
    }

    @Override
    public Umrandung umranden() {
        return new Steinmauer();
    }
}
